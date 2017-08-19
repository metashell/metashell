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

#include <metashell/default_environment_detector.hpp>
#include <metashell/engine_constant.hpp>
#include <metashell/mdb_command_handler_map.hpp>
#include <metashell/mdb_shell.hpp>
#include <metashell/metashell.hpp>
#include <metashell/parse_config.hpp>
#include <metashell/pragma_handler_map.hpp>
#include <metashell/shell.hpp>

#include <metashell/data/config.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <boost/range/adaptor/map.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <boost/optional.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/xpressive/xpressive.hpp>

#include <boost/range/combine.hpp>

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

using namespace metashell;

namespace
{
  std::string remove_markdown(std::string s_)
  {
    using boost::xpressive::sregex;
    using boost::xpressive::regex_replace;
    using boost::xpressive::as_xpr;
    using boost::xpressive::s1;
    using boost::xpressive::s2;

    boost::replace_all(s_, "`", "");
    boost::replace_all(s_, "<br />", "\n");
    boost::replace_all(s_, "&nbsp;", " ");

    return regex_replace(s_, sregex('[' >> (s1 = *~as_xpr(']')) >> "](" >>
                                    (s2 = *~as_xpr(')')) >> ')'),
                         "$1 (see $2)");
  }

  void show_help(std::ostream& out_,
                 const boost::program_options::options_description& desc_)
  {
    out_ << "Usage:\n"
         << "  metashell <options> [-- <extra Clang options>]\n"
         << "\n"
         << desc_ << std::endl;
  }

  void show_markdown(const std::vector<std::string>& name_,
                     const pragma_handler& h_,
                     std::ostream& out_)
  {
    using boost::algorithm::join;

    const std::string args = h_.arguments();

    out_ << "* __`#msh " << join(name_, " ") << (args.empty() ? "" : " ")
         << args << "`__ <br />\n"
         << h_.description() << "\n"
         << std::endl;
  }

  void show_pragma_help()
  {
    const data::config cfg{};
    command_processor_queue cpq;
    const std::string internal_dir;
    const boost::filesystem::path mdb_temp_dir;
    shell sh(cfg, cpq, internal_dir, "", mdb_temp_dir, create_failing_engine());
    const pragma_handler_map m =
        pragma_handler_map::build_default(sh, &cpq, mdb_temp_dir, nullptr);

    for (const auto& p : m)
    {
      show_markdown(p.first, p.second, std::cout);
    }
  }

  std::string supported_features(const engine_entry& engine_)
  {
    return boost::algorithm::join(
        engine_.features() | boost::adaptors::transformed([](data::feature f_) {
          return to_string(f_);
        }),
        ", ");
  }

  std::string make_id(const std::string& value_)
  {
    return boost::algorithm::replace_all_copy(value_, " ", "_");
  }

  std::string self_id(const std::string& value_)
  {
    return "<strong id=\"" + make_id(value_) + "\">" + value_ + "</strong>";
  }

  std::string self_reference(const std::string& value_)
  {
    return "<a href=\"#" + make_id(value_) + "\">" + value_ + "</a>";
  }

  void show_engine_help(const std::map<std::string, engine_entry>& engines_,
                        const boost::filesystem::path& app_name_)
  {
    for (const auto& engine : engines_)
    {
      std::cout << "* " << self_id(engine.first) << "<br />\n"
                << "Usage: `" << app_name_.filename().string() << " --engine "
                << engine.first;
      const auto args = engine.second.args();
      if (!args.empty())
      {
        std::cout << " -- " << engine.second.args();
      }
      std::cout << "`<br />\n<br />\n"
                << engine.second.description()
                << "<br /><br />Supported features: "
                << supported_features(engine.second) << "<br />\n<br />\n";
    }
  }

  template <class Size>
  void adopt_widths(const std::vector<std::string>& row_,
                    std::vector<Size>& widths_)
  {
    assert(widths_.empty() || widths_.size() == row_.size());
    widths_.resize(row_.size(), 0);

    for (auto p : boost::combine(widths_, row_))
    {
      Size& width = boost::get<0>(p);
      const std::string& cell = boost::get<1>(p);

      width = std::max(width, cell.size());
    }
  }

  template <class Size>
  void md_format_row(const std::vector<std::string>& row_,
                     const std::vector<Size>& widths_,
                     std::ostream& out_)
  {
    assert(row_.size() == widths_.size());

    for (auto p : boost::combine(widths_, row_))
    {
      const Size& width = boost::get<0>(p);
      const std::string& cell = boost::get<1>(p);

      assert(cell.size() <= width);

      out_ << '|' << cell << std::string(width - cell.size(), ' ');
    }
    out_ << "|\n";
  }

  template <class Size>
  void md_format_header_separator(const std::vector<Size>& widths_,
                                  std::ostream& out_)
  {
    bool first_column = true;
    for (int width : widths_)
    {
      if (first_column)
      {
        assert(width >= 1);
        out_ << "|:" << std::string(width - 1, '-');
        first_column = false;
      }
      else
      {
        assert(width >= 2);
        out_ << "|:" << std::string(width - 2, '-') << ":";
      }
    }
    out_ << "|\n";
  }

  void md_format_table(const std::vector<std::string>& header_,
                       const std::vector<std::vector<std::string>>& table_,
                       std::ostream& out_)
  {
    std::vector<std::vector<std::string>::size_type> widths;
    adopt_widths(header_, widths);
    for (const auto& row : table_)
    {
      adopt_widths(row, widths);
    }

    md_format_row(header_, widths, out_);
    md_format_header_separator(widths, out_);
    for (const auto& row : table_)
    {
      md_format_row(row, widths, out_);
    }
  }

  void show_engine_features(const std::map<std::string, engine_entry>& engines_)
  {
    const auto features = data::feature::all();

    std::vector<std::string> header{"Feature"};
    for (const auto& engine : engines_)
    {
      header.push_back(self_reference(engine.first));
    }

    std::vector<std::vector<std::string>> table;
    for (data::feature f : features)
    {
      table.push_back({self_reference(to_string(f))});

      for (const auto& engine : engines_)
      {
        const auto& engine_features = engine.second.features();

        table.back().push_back(
            std::find(engine_features.begin(), engine_features.end(), f) ==
                    engine_features.end() ?
                "<img src=\"../../img/no.png\" width=\"20px\" />" :
                "<img src=\"../../img/yes.png\" width=\"20px\" />");
      }
    }

    md_format_table(header, table, std::cout);
  }

  void show_feature_help()
  {
    for (data::feature f : data::feature::all())
    {
      std::cout << "* " << self_id(to_string(f)) << "<br />\n"
                << f.description() << "<br />\n<br />\n";
    }
  }

  void show_mdb_help(bool preprocessor_)
  {
    using boost::algorithm::join;
    using boost::algorithm::replace_all_copy;

    mdb_command_handler_map::commands_t commands =
        mdb_shell::build_command_handler(preprocessor_).get_commands();

    for (const mdb_command& cmd : commands)
    {
      std::cout << "* __`" << join(cmd.get_keys(), "|") << " "
                << cmd.get_usage() << "`__ <br />\n"
                << cmd.get_short_description();
      if (!cmd.get_long_description().empty())
      {
        std::cout << " <br />" << '\n'
                  << replace_all_copy(cmd.get_long_description(), "\n", "\n  ");
      }
      std::cout << '\n' << std::endl;
    }
  }

  void show_engine_help(const std::map<std::string, engine_entry>& engines_,
                        const std::string& engine_,
                        std::ostream* out_,
                        const std::string& app_name_)
  {
    const auto e = engines_.find(engine_);
    if (e == engines_.end())
    {
      throw std::runtime_error("Engine " + engine_ + " not found.");
    }
    else if (out_)
    {
      *out_ << "Usage: " << std::endl
            << std::endl
            << "  " << app_name_ << " --engine " << e->first;
      const auto args = e->second.args();
      if (!args.empty())
      {
        *out_ << " -- " << e->second.args();
      }
      *out_ << std::endl
            << std::endl
            << remove_markdown(e->second.description()) << std::endl
            << std::endl
            << "Supported features: " << supported_features(e->second)
            << std::endl
            << std::endl;
    }
  }

  std::string replace_all(std::string s_,
                          const std::string& pattern_,
                          const std::string& replacement_)
  {
    for (size_t p; (p = s_.find(pattern_)) != std::string::npos;)
    {
      s_.replace(p, pattern_.length(), replacement_);
    }
    return s_;
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
        throw std::runtime_error(
            replace_all(replace_all(_msg, "{NAME}", name), "{VALUE}", _ignore));
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
}

parse_config_result
metashell::parse_config(int argc_,
                        const char* argv_[],
                        const std::map<std::string, engine_entry>& engines_,
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

  if (argc_ > 0)
  {
    cfg.metashell_binary = argv_[0];
  }

  const char** const minus_minus =
      std::find(argv_, argv_ + argc_, std::string("--"));
  if (minus_minus != argv_ + argc_)
  {
    cfg.extra_clang_args.insert(
        cfg.extra_clang_args.end(), minus_minus + 1, argv_ + argc_);
  }
  const int argc = minus_minus - argv_;

  std::string con_type("readline");
  cfg.use_precompiled_headers = true;

  std::string help_engine;

  const std::string engine_info =
      "The engine (C++ compiler) to use. Available engines: " +
      boost::algorithm::join(engines_ | boost::adaptors::map_keys, ", ") +
      ". Default: " + cfg.engine;

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
      "show_pragma_help",
      "Display help for pragmas in MarkDown format and exit."
    )
    (
      "show_mdb_help",
      "Display help for mdb commands in MarkDown format and exit"
    )
    (
      "show_pdb_help",
      "Display help for pdb commands in MarkDown format and exit"
    )
    (
      "show_engine_help",
      "Display help for all engines in MarkDown format and exit"
    )
    (
      "show_engine_features",
      "Display a table showing which engine supports which features"
      " in MarkDown format and exit"
    )
    (
      "show_feature_help",
      "Display help for all features in MarkDown format and exit"
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
    ("engine", value(&cfg.engine), engine_info.c_str())
    ("help_engine", value(&help_engine), "Display help about the engine")
    ("preprocessor", "Starts the shell in preprocessor mode");
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
          "Please use \"--engine clang\" with the custom clang binary instead.",
          "{NAME} has been decommissioned. You can specify the clang binary to"
          " use by using the clang engine. For example:\n"
          "\n" +
              std::string(argv_[0]) +
              " --engine clang -- {VALUE} -std=c++0x -ftemplate-depth=256"
              " -Wfatal-errors" +
              (env_detector_.on_windows() ?
                   " -fno-ms-compatibility -U_MSC_VER" :
                   "")},
      dec_arg{
          "enable_saving", dec_type::flag,
          "Saving is enabled by default. To disable it, use --disable_saving.",
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
    cfg.con_type = metashell::data::parse_console_type(con_type);
    cfg.use_precompiled_headers = !vm.count("no_precompiled_headers");
    cfg.saving_enabled = !vm.count("disable_saving");
    cfg.splash_enabled = vm.count("nosplash") == 0;
    cfg.preprocessor_mode = vm.count("preprocessor");
    if (vm.count("log") == 0)
    {
      cfg.log_mode = data::logging_mode::none;
    }
    else
    {
      cfg.log_mode = (cfg.log_file == "-") ? data::logging_mode::console :
                                             data::logging_mode::file;
    }

    if (vm.count("help"))
    {
      if (out_)
      {
        show_help(*out_, desc);
      }
      return parse_config_result::exit(false);
    }
    else if (vm.count("show_pragma_help"))
    {
      show_pragma_help();
      return parse_config_result::exit(false);
    }
    else if (vm.count("show_mdb_help"))
    {
      show_mdb_help(false);
      return parse_config_result::exit(false);
    }
    else if (vm.count("show_pdb_help"))
    {
      show_mdb_help(true);
      return parse_config_result::exit(false);
    }
    else if (vm.count("show_engine_help"))
    {
      show_engine_help(engines_, argv_[0]);
      return parse_config_result::exit(false);
    }
    else if (vm.count("show_engine_features"))
    {
      show_engine_features(engines_);
      return parse_config_result::exit(false);
    }
    else if (vm.count("show_feature_help"))
    {
      show_feature_help();
      return parse_config_result::exit(false);
    }
    else if (vm.count("help_engine"))
    {
      show_engine_help(engines_, help_engine, out_, argv_[0]);
      return parse_config_result::exit(false);
    }
    else
    {
      return parse_config_result::start_shell(cfg);
    }
  }
  catch (const std::exception& e_)
  {
    if (err_)
    {
      *err_ << e_.what() << "\n\n";
      show_help(*err_, desc);
    }
    return parse_config_result::exit(true);
  }
}

parse_config_result parse_config_result::exit(bool with_error_)
{
  parse_config_result r;
  r.action =
      with_error_ ? action_t::exit_with_error : action_t::exit_without_error;
  return r;
}

parse_config_result parse_config_result::start_shell(const data::config& cfg_)
{
  parse_config_result r;
  r.action = action_t::run_shell;
  r.cfg = cfg_;
  return r;
}

bool parse_config_result::should_run_shell() const
{
  return action == action_t::run_shell;
}

bool parse_config_result::should_error_at_exit() const
{
  return action == action_t::exit_with_error;
}

std::ostream& metashell::operator<<(std::ostream& out_,
                                    parse_config_result::action_t a_)
{
  switch (a_)
  {
  case parse_config_result::action_t::run_shell:
    return out_ << "run_shell";
  case parse_config_result::action_t::exit_with_error:
    return out_ << "exit_with_error";
  case parse_config_result::action_t::exit_without_error:
    return out_ << "exit_without_error";
  default:
    assert(!"Invalid action");
    std::abort();
  }
}
