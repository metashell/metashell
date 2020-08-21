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

#include <metashell/data/arg_parser.hpp>
#include <metashell/data/exception.hpp>
#include <metashell/data/markdown_string.hpp>

#include <metashell/core/engine_entry.hpp>
#include <metashell/core/parse_config.hpp>
#include <metashell/core/rapid_compile_commands_parser.hpp>
#include <metashell/core/rapid_shell_config_parser.hpp>
#include <metashell/core/stdout_console.hpp>

#include <rapidjson/reader.h>

#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <just/file.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <optional>
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

      void show_help(std::ostream* out_, const data::markdown_string& desc_)
      {
        if (out_)
        {
          *out_ << "Usage:\n"
                << "  metashell <options> [-- <extra Clang options>]\n"
                << "\n"
                << unformat(desc_) << "\n";
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

      void load_compile_commands(const boost::filesystem::path& path_,
                                 data::config& cfg_)
      {
        rapid_compile_commands_parser handler{
            cfg_.default_shell_config().use_precompiled_headers,
            cfg_.default_shell_config().preprocessor_mode};
        {
          const std::string json =
              just::file::read<std::string>(path_.string());
          rapidjson::StringStream string_stream(json.c_str());
          rapidjson::Reader reader;
          if (!reader.Parse(string_stream, handler))
          {
            throw data::exception{"Invalid JSON document: " + path_.string()};
          }
        }

        for (data::shell_config scfg : handler.configs())
        {
          scfg.name = find_unique_name(cfg_, std::move(scfg.name));
          cfg_.push_back(scfg);
        };
      }

      void load_config(const boost::filesystem::path& path_, data::config& cfg_)
      {
        const std::string json = just::file::read<std::string>(path_.string());
        rapidjson::StringStream string_stream{json.c_str()};
        rapidjson::Reader reader;
        rapid_shell_config_parser handler;
        handler.parsed_config_callback = [&cfg_](data::shell_config config_) {
          if (cfg_.exists(config_.name))
          {
            throw json_parsing_error{
                "More than one config provided with the name " + config_.name};
          }
          else
          {
            cfg_.push_back(std::move(config_));
          }
        };
        if (!reader.Parse(string_stream, handler))
        {
          throw data::exception{"Invalid JSON document: " + path_.string()};
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
      using data::no_short_name;
      using data::arg_number;

      data::config cfg;

      const char** args_end = argv_ + argc_;

      const char** const minus_minus =
          std::find(argv_, args_end, std::string("--"));

      const char** extra_args_begin =
          minus_minus == args_end ? args_end : minus_minus + 1;

      const int argc = minus_minus - argv_;

      cfg.default_shell_config().engine.default_value().args =
          data::command_line_argument_list{extra_args_begin, args_end};

      cfg.default_shell_config().use_precompiled_headers = true;
      cfg.default_shell_config().preprocessor_mode = false;

      cfg.con_type = data::console_type::readline;
      cfg.log_mode = data::logging_mode::none;

      std::optional<data::engine_name> help_engine;

      bool display_help = false;

      constexpr char default_suggestion[] =
          "Please provide it as a compiler argument after --";

      data::arg_parser parser{[](const data::command_line_argument& arg_) {
                                return "Invalid argument: " + arg_;
                              },
                              [&cfg](const data::command_line_argument& arg_) {
                                try
                                {
                                  load_compile_commands(arg_.value(), cfg);
                                  return true;
                                }
                                catch (...)
                                {
                                }

                                try
                                {
                                  load_config(arg_.value(), cfg);
                                  return true;
                                }
                                catch (...)
                                {
                                }

                                return false;
                              }};

      // clang-format off
      parser
        .flag<false>("-h", "--help", "Display help", display_help)
        .flag<false>("-V", "--verbose", "Verbose mode", cfg.verbose)
        .flag<true>(
          "-H", "--no_highligt",
          "Disable syntax highlighting",
          cfg.syntax_highlight
        )
        .flag<false>("--indent", "Enable indenting (experimental)", cfg.indent)
        .flag<arg_number::at_most_once>(
          "--no_precompiled_headers",
          "Disable precompiled header usage."
          " (It needs clang++ to be available and writes to the local disc.)",
          [&cfg] { cfg.default_shell_config().use_precompiled_headers = false; }
        )
        .flag<true>(
          "--disable_saving",
          "Disable saving the environment using the #msh environment save",
          cfg.saving_enabled
        )
        .flag<true>(
          "--nosplash",
          "Disable the splash messages",
          cfg.splash_enabled
        )
        .flag<arg_number::at_most_once>(
          "--preprocessor",
          "Starts the shell in preprocessor mode",
          [&cfg] { cfg.default_shell_config().preprocessor_mode = true; }
        )

        .decommissioned_flag("-w", default_suggestion)
        .decommissioned_flag("--no_warnings", default_suggestion)
        .decommissioned_flag(
          "--enable_saving",
          "Saving is enabled by default. To disable it, use --disable_saving.",
          "Saving is enabled by default."
        )

        .with_value<arg_number::at_most_once>(
          no_short_name, "--console",
          "Console type. Possible values: plain, readline, json."
          " Default value: readline.",
          [&cfg](const data::command_line_argument& arg_)
          {
            cfg.con_type = data::parse_console_type(arg_.value());
          }
        )
        .with_value<arg_number::at_most_once>(
          no_short_name, "--log",
          "Log into a file. When it is set to -, it logs into the console.",
          [&cfg](const data::command_line_argument& arg_)
          {
            if (arg_ == data::command_line_argument{"-"})
            {
              cfg.log_mode = data::logging_mode::console;
            }
            else
            {
              cfg.log_mode = data::logging_mode::file;
              cfg.log_file = arg_.value();
            }
          }
        )
        .with_value<arg_number::at_most_once>(
          no_short_name, "--engine",
          "The engine (C++ compiler) to use. Available engines: auto, " +
          boost::algorithm::join(engines_ | boost::adaptors::map_keys |
                                     boost::adaptors::transformed([](
                                         const data::real_engine_name& name_) {
                                       return to_string(name_);
                                     }),
                                 ", ") +
          ". Default: " + data::shell_config_data().engine->name,
          [&cfg](const data::command_line_argument& arg_)
          {
            cfg.default_shell_config().engine.default_value().name =
              data::parse_engine_name(arg_.value());
          }
        )
        .with_value<arg_number::at_most_once>(
          no_short_name, "--help_engine",
          "Display help about the engine",
          [&help_engine](const data::command_line_argument& arg_)
          {
            help_engine = data::parse_engine_name(arg_.value());
          }
        )
        .with_value<arg_number::any>(
          no_short_name, "--load_compile_commands",
          "Generate configs for the compilation units of the"
          " compile_commands.json file",
          [&cfg](const data::command_line_argument& arg_)
          {
            load_compile_commands(arg_.value(), cfg);
          }
        )
        .with_value<arg_number::any>(
          no_short_name, "--load_configs",
          "Load configs from a file.",
          [&cfg](const data::command_line_argument& arg_)
          {
            load_config(arg_.value(), cfg);
          }
        )

        .decommissioned_with_value(
          no_short_name, "-I",
          default_suggestion
        )
        .decommissioned_with_value(
          no_short_name, "--include",
          default_suggestion
        )
        .decommissioned_with_value(
          no_short_name, "-D",
          default_suggestion
        )
        .decommissioned_with_value(
          no_short_name, "--define",
          default_suggestion
        )
        .decommissioned_with_value(
          no_short_name, "--std",
          default_suggestion
        )
        .decommissioned_with_value(
          no_short_name, "-f",
          default_suggestion
        )
        .decommissioned_with_value(
          no_short_name, "-s",
          default_suggestion
        )
        .decommissioned_with_value(
          no_short_name,
          "--clang",
          "Please use \"--engine clang\" with the custom clang binary instead.",
          "{NAME} has been decommissioned. You can specify the clang binary to"
          " use by using the clang engine. For example:\n"
          "\n" +
              std::string{ argv_[0] } +
              " --engine clang -- {VALUE} -std=c++0x -ftemplate-depth=256"
              " -Wfatal-errors" +
              (env_detector_.on_windows() ?
                   " -fno-ms-compatibility -U_MSC_VER" :
                   "")
        )
      ;
      // clang-format on

      try
      {
        parser.parse(argc > 0 ? data::command_line_argument_list{argv_ + 1,
                                                                 argv_ + argc} :
                                data::command_line_argument_list{});

        if (display_help)
        {
          show_help(out_, parser.description(stdout_console{}.width()));
          return exit{false};
        }
        else if (help_engine)
        {
          show_engine_help(engines_, *help_engine, out_, argv_[0]);
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
        show_help(err_, parser.description(stdout_console{}.width()));
        return exit{true};
      }
    }
  }
}
