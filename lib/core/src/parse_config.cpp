// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <metashell/data/markdown_string.hpp>

#include <metashell/core/engine_entry.hpp>
#include <metashell/core/parse_config.hpp>
#include <metashell/core/rapid_compile_commands_parser.hpp>
#include <metashell/core/rapid_shell_config_parser.hpp>

#include <rapidjson/reader.h>

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <just/file.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

namespace metashell
{
  namespace core
  {
    namespace
    {
      void show_error(std::ostream* out_, const std::exception& err_)
      {
        if (out_)
        {
          *out_ << err_.what() << "\n\n";
        }
      }

      void show_help(std::ostream* out_,
                     const boost::program_options::options_description& desc_)
      {
        if (out_)
        {
          *out_ << "Usage:\n"
                << "  metashell <options> [-- <extra Clang options>]\n"
                << "\n"
                << desc_ << std::endl;
        }
      }

      void show_engine_help(
          const std::map<data::real_engine_name, engine_entry>& engines_,
          const data::engine_name& engine_,
          std::ostream* out_,
          const std::string& app_name_)
      {
        if (auto engine = mpark::get_if<data::real_engine_name>(&engine_))
        {
          const auto e = engines_.find(*engine);
          if (e == engines_.end())
          {
            throw std::runtime_error("Engine " + *engine + " not found.");
          }
          else if (out_)
          {
            *out_ << "Usage: \n\n  " << app_name_ << " --engine " << e->first;
            const auto args = e->second.args();
            if (!args.empty())
            {
              *out_ << " -- " << e->second.args();
            }
            *out_ << "\n\n"
                  << unformat(e->second.description())
                  << "\n\nSupported features: " << list_features(e->second)
                  << "\n\n";
          }
        }
        else if (out_)
        {
          *out_ << "Usage: \n\n  " << app_name_ << " --engine auto\n\n"
                << unformat(data::auto_engine_description()) << "\n\n";
        }
      }

      class decommissioned_argument
      {
      public:
        enum class type
        {
          flag,
          one_value,
          multiple_values
        };

        decommissioned_argument(const std::string& long_form_,
                                char short_form_,
                                type type_)
          : _long_form(long_form_),
            _short_form(std::string(1, short_form_)),
            _type(type_)
        {
        }

        decommissioned_argument(const std::string& long_form_,
                                type type_,
                                const std::string& suggestion_,
                                const std::string& msg_)
          : _long_form(long_form_),
            _short_form(boost::none),
            _type(type_),
            _suggestion(suggestion_),
            _msg(msg_)
        {
        }

        decommissioned_argument(const std::string& long_form_, type type_)
          : _long_form(long_form_), _short_form(boost::none), _type(type_)
        {
        }

        decommissioned_argument(char short_form_, type type_)
          : _long_form(boost::none),
            _short_form(std::string(1, short_form_)),
            _type(type_)
        {
        }

        void add_to(boost::program_options::options_description& desc_)
        {
          using boost::program_options::value;

          const std::string desc = "DECOMMISSIONED argument. " + _suggestion;

          const std::string fmt = (_long_form ? *_long_form : "") +
                                  (_short_form ? "," + *_short_form : "");

          switch (_type)
          {
          case type::flag:
            desc_.add_options()(fmt.c_str(), desc.c_str());
            break;
          case type::one_value:
            desc_.add_options()(fmt.c_str(), value(&_ignore), desc.c_str());
            break;
          case type::multiple_values:
            desc_.add_options()(fmt.c_str(), value(&_ignores), desc.c_str());
            break;
          }
        }

        void check(const boost::program_options::variables_map& vm_) const
        {
          if ((_long_form && vm_.count(*_long_form)) ||
              (_short_form && vm_.count(*_short_form)) || !_ignore.empty() ||
              !_ignores.empty())
          {
            const std::string name =
                _short_form ? "-" + *_short_form : "--" + *_long_form;
            throw std::runtime_error(boost::replace_all_copy(
                boost::replace_all_copy(_msg, "{NAME}", name), "{VALUE}",
                _ignore));
          }
        }

      private:
        boost::optional<std::string> _long_form;
        boost::optional<std::string> _short_form;
        type _type;

        std::string _ignore;
        std::vector<std::string> _ignores;
        std::string _suggestion =
            "Please provide it as a compiler argument after --";
        std::string _msg =
            "Argument {NAME} has been decommissioned. Please provide it as a "
            "compiler"
            " argument after --";
      };

      data::shell_config parse_default_shell_config(
          const boost::program_options::variables_map& vm_,
          const char** extra_args_begin_,
          const char** extra_args_end_,
          const boost::optional<data::engine_name>& engine_)
      {
        data::shell_config result(
            data::shell_config_name("default"), data::shell_config_data());

        result.engine.args = data::command_line_argument_list(
            extra_args_begin_, extra_args_end_);

        if (engine_)
        {
          result.engine.name = *engine_;
        }
        result.use_precompiled_headers = !vm_.count("no_precompiled_headers");
        result.preprocessor_mode = vm_.count("preprocessor");

        return result;
      }

      data::shell_config_name find_unique_name(const data::config& cfg_,
                                               data::shell_config_name prefix_)
      {
        if (!cfg_.exists(prefix_))
        {
          return prefix_;
        }

        for (;;)
        {
          prefix_ += data::shell_config_name(":");

          // Relying on unsigned integer overflow
          for (unsigned long int i = 1; i != 0; ++i)
          {
            const data::shell_config_name name =
                prefix_ + data::shell_config_name(std::to_string(i));
            if (!cfg_.exists(name))
            {
              return name;
            }
          }
        }
      }
    }

    parse_config_result
    parse_config(int argc_,
                 const char* argv_[],
                 const std::map<data::real_engine_name, engine_entry>& engines_,
                 iface::environment_detector& env_detector_,
                 std::ostream* out_,
                 std::ostream* err_)
    {
      using boost::program_options::options_description;
      using boost::program_options::variables_map;
      using boost::program_options::store;
      using boost::program_options::notify;
      using boost::program_options::parse_command_line;
      using boost::program_options::value;

      data::config cfg;

      const char** args_end = argv_ + argc_;

      const char** const minus_minus =
          std::find(argv_, args_end, std::string("--"));

      const char** extra_args_begin =
          minus_minus == args_end ? args_end : minus_minus + 1;

      const int argc = minus_minus - argv_;

      std::string con_type("readline");

      std::string help_engine;
      std::string engine;

      const std::string engine_info =
          "The engine (C++ compiler) to use. Available engines: auto, " +
          boost::algorithm::join(engines_ | boost::adaptors::map_keys |
                                     boost::adaptors::transformed([](
                                         const data::real_engine_name& name_) {
                                       return to_string(name_);
                                     }),
                                 ", ") +
          ". Default: " + data::shell_config_data().engine.name;

      std::vector<boost::filesystem::path> configs_to_load;
      std::vector<boost::filesystem::path> compile_commands_to_load;

      options_description desc("Options");
      // clang-format off
      desc.add_options()
        ("help", "Display help")
        ("verbose,V", "Verbose mode")
        ("no_highlight,H", "Disable syntax highlighting")
        ("indent", "Enable indenting (experimental)")
        (
          "no_precompiled_headers",
          "Disable precompiled header usage."
          " (It needs clang++ to be available and writes to the local disc.)"
        )
        (
          "disable_saving",
          "Disable saving the environment using the #msh environment save"
        )
        (
          "console", value(&con_type)->default_value(con_type),
          "Console type. Possible values: plain, readline, json"
        )
        ("nosplash", "Disable the splash messages")
        (
          "log", value(&cfg.log_file),
          "Log into a file. When it is set to -, it logs into the console."
        )
        ("engine", value(&engine), engine_info.c_str())
        ("help_engine", value(&help_engine), "Display help about the engine")
        ("preprocessor", "Starts the shell in preprocessor mode")
        (
          "load_compile_commands", value(&compile_commands_to_load),
          "Generate configs for the compilation units of the"
          " compile_commands.json file"
        )
        ("load_configs", value(&configs_to_load), "Load configs from a file.");
      // clang-format on

      using dec_arg = decommissioned_argument;
      using dec_type = decommissioned_argument::type;
      std::vector<dec_arg> dec_args{
          dec_arg{"include", 'I', dec_type::multiple_values},
          dec_arg{"define", 'D', dec_type::multiple_values},
          dec_arg{"std", dec_type::one_value},
          dec_arg{"no_warnings", 'w', dec_type::flag},
          dec_arg{'f', dec_type::one_value}, dec_arg{'s', dec_type::one_value},
          dec_arg{
              "clang", dec_type::one_value,
              "Please use \"--engine clang\" with the custom clang binary "
              "instead.",
              "{NAME} has been decommissioned. You can specify the clang "
              "binary to"
              " use by using the clang engine. For example:\n"
              "\n" +
                  std::string(argv_[0]) +
                  " --engine clang -- {VALUE} -std=c++0x -ftemplate-depth=256"
                  " -Wfatal-errors" +
                  (env_detector_.on_windows() ?
                       " -fno-ms-compatibility -U_MSC_VER" :
                       "")},
          dec_arg{"enable_saving", dec_type::flag,
                  "Saving is enabled by default. To disable it, use "
                  "--disable_saving.",
                  "Saving is enabled by default."}};

      for (auto& a : dec_args)
      {
        a.add_to(desc);
      }

      try
      {
        variables_map vm;
        store(parse_command_line(argc, argv_, desc), vm);
        notify(vm);

        for (const auto& a : dec_args)
        {
          a.check(vm);
        }

        cfg.verbose = vm.count("verbose") || vm.count("V");
        cfg.syntax_highlight = !(vm.count("no_highlight") || vm.count("H"));
        cfg.indent = vm.count("indent") != 0;
        cfg.con_type = data::parse_console_type(con_type);
        cfg.saving_enabled = !vm.count("disable_saving");
        cfg.splash_enabled = vm.count("nosplash") == 0;
        if (vm.count("log") == 0)
        {
          cfg.log_mode = data::logging_mode::none;
        }
        else
        {
          cfg.log_mode = (cfg.log_file == "-") ? data::logging_mode::console :
                                                 data::logging_mode::file;
        }

        const auto default_shell_config = parse_default_shell_config(
            vm, extra_args_begin, args_end,
            vm.count("engine") ?
                boost::make_optional(data::parse_engine_name(engine)) :
                boost::none);

        cfg.push_back(default_shell_config);

        for (const boost::filesystem::path& config_path : configs_to_load)
        {
          const std::string json =
              just::file::read<std::string>(config_path.string());
          rapidjson::StringStream string_stream(json.c_str());
          rapidjson::Reader reader;
          rapid_shell_config_parser handler;
          handler.parsed_config_callback = [&cfg](data::shell_config cfg_) {
            if (cfg.exists(cfg_.name))
            {
              throw json_parsing_error(
                  "More than one config provided with the name " + cfg_.name);
            }
            else
            {
              cfg.push_back(std::move(cfg_));
            }
          };
          reader.Parse(string_stream, handler);
        }

        for (const boost::filesystem::path& compile_commands_path :
             compile_commands_to_load)
        {
          rapid_compile_commands_parser handler(
              default_shell_config.use_precompiled_headers,
              default_shell_config.preprocessor_mode);
          {
            const std::string json =
                just::file::read<std::string>(compile_commands_path.string());
            rapidjson::StringStream string_stream(json.c_str());
            rapidjson::Reader reader;
            reader.Parse(string_stream, handler);
          }

          for (data::shell_config scfg : handler.configs())
          {
            scfg.name = find_unique_name(cfg, std::move(scfg.name));
            cfg.push_back(scfg);
          };
        }

        if (vm.count("help"))
        {
          show_help(out_, desc);
          return exit{false};
        }
        else if (vm.count("help_engine"))
        {
          show_engine_help(
              engines_, data::parse_engine_name(help_engine), out_, argv_[0]);
          return exit{false};
        }
        else
        {
          return cfg;
        }
      }
      catch (const json_parsing_error& e_)
      {
        show_error(err_, e_);
        return exit{true};
      }
      catch (const std::exception& e_)
      {
        show_error(err_, e_);
        show_help(err_, desc);
        return exit{true};
      }
    }
  }
}
