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

#include <metashell/parse_config.hpp>
#include <metashell/config.hpp>
#include <metashell/metashell.hpp>
#include <metashell/pragma_handler_map.hpp>
#include <metashell/shell.hpp>
#include <metashell/default_environment_detector.hpp>
#include <metashell/mdb_shell.hpp>
#include <metashell/mdb_command_handler_map.hpp>
#include <metashell/null_libclang.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <stdexcept>

using namespace metashell;

namespace
{
  void show_help(
    std::ostream& out_,
    const boost::program_options::options_description& desc_
  )
  {
    out_
      << "Usage:\n"
      << "  metashell <options> [-- <extra Clang options>]\n"
      << "\n"
      << desc_ << std::endl;
  }

  void show_markdown(
    const std::vector<std::string>& name_,
    const pragma_handler& h_,
    std::ostream& out_
  )
  {
    using boost::algorithm::join;

    const std::string args = h_.arguments();

    out_
      << "* __`#msh " << join(name_, " ")
      << (args.empty() ? "" : " ") << args << "`__ <br />\n"
      << h_.description() << "\n"
      << std::endl;
  }

  void show_pragma_help()
  {
    const config cfg;
    null_libclang lclang;
    command_processor_queue cpq;
    shell sh(cfg, cpq, lclang);
    const pragma_handler_map
      m = pragma_handler_map::build_default(sh, &cpq, nullptr);

    typedef std::pair<std::vector<std::string>, pragma_handler> sp;
    for (const sp& p : m)
    {
      show_markdown(p.first, p.second, std::cout);
    }
  }

  void show_mdb_help()
  {
    using boost::algorithm::join;
    using boost::algorithm::replace_all_copy;

    mdb_command_handler_map::commands_t
      commands = mdb_shell::command_handler.get_commands();

    for (const mdb_command& cmd : commands)
    {
      std::cout
        << "* __`" << join(cmd.get_keys(), "|") << " "
        << cmd.get_usage() <<  "`__ <br />\n"
        << cmd.get_short_description();
      if (!cmd.get_long_description().empty()) {
        std::cout << " <br />" << '\n' <<
          replace_all_copy(cmd.get_long_description(), "\n", "\n  ");
      }
      std::cout << '\n' << std::endl;
    }
  }

  int parse_max_template_depth(const std::string& key_value_)
  {
    const auto eq = key_value_.find('=');
    if (eq == std::string::npos || key_value_.substr(0, eq) != "template-depth")
    {
      throw
        std::runtime_error("-f has to be followed by template-depth=<value>");
    }
    else if (eq == key_value_.size() - 1)
    {
      throw std::runtime_error("The value of -ftemplate-depth is missing.");
    }
    else
    {
      std::istringstream s(key_value_.substr(eq + 1));
      int v;
      if (!(s >> v))
      {
        throw std::runtime_error("Invalid value for -ftemplate-depth.");
      }
      else if (v < 0)
      {
        throw std::runtime_error("Template depth can not be negative.");
      }
      else
      {
        return v;
      }
    }
  }
}

parse_config_result metashell::parse_config(
  int argc_,
  const char* argv_[],
  std::ostream* out_,
  std::ostream* err_
)
{
  using boost::program_options::options_description;
  using boost::program_options::variables_map;
  using boost::program_options::store;
  using boost::program_options::notify;
  using boost::program_options::parse_command_line;
  using boost::program_options::value;

  user_config ucfg;

  const char** const
    minus_minus = std::find(argv_, argv_ + argc_, std::string("--"));
  if (minus_minus != argv_ + argc_)
  {
    ucfg.extra_clang_args.insert(
      ucfg.extra_clang_args.end(),
      minus_minus + 1,
      argv_ + argc_
    );
  }
  const int argc = minus_minus - argv_;

  std::string cppstd("c++0x");
  std::string con_type("readline");
  ucfg.use_precompiled_headers = !ucfg.clang_path.empty();
  std::string fvalue;

  options_description desc("Options");
  desc.add_options()
    ("help", "Display help")
    ("include,I", value(&ucfg.include_path), "Additional include directory")
    ("define,D", value(&ucfg.macros), "Additional macro definitions")
    ("verbose,V", "Verbose mode")
    ("no_highlight,H", "Disable syntax highlighting")
    ("indent", "Enable indenting (experimental)")
    (
      "std", value(&cppstd)->default_value(cppstd),
      "C++ standard to use. Possible values: c++0x/c++11, c++1y/c++14."
    )
    ("no_warnings,w", "Disable warnings")
    (
      "no_precompiled_headers",
      "Disable precompiled header usage."
      " (It needs clang++ to be available and writes to the local disc.)"
    )
    (
      "clang", value(&ucfg.clang_path),
      "The path of the clang++ binary to use for"
      " generating precompiled headers."
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
      ",f",
      value(&fvalue),
      "Feature option. Currently supported: -ftemplate-depth=<value>"
    )
    (
      "enable_saving",
      "Enable saving the environment using the #msh environment save"
    )
    (
      "console", value(&con_type)->default_value(con_type),
      "Console type. Possible values: plain, readline, json"
    )
    ("nosplash", "Disable the splash messages")
    (
      "log", value(&ucfg.log_file),
      "Log into a file. When it is set to -, it logs into the console."
    )
    ;

  try
  {
    variables_map vm;
    store(parse_command_line(argc, argv_, desc), vm);
    notify(vm);

    ucfg.verbose = vm.count("verbose") || vm.count("V");
    ucfg.syntax_highlight = !(vm.count("no_highlight") || vm.count("H"));
    ucfg.indent = vm.count("indent") != 0;
    ucfg.standard_to_use = metashell::parse_standard(cppstd);
    ucfg.con_type = metashell::parse_console_type(con_type);
    ucfg.warnings_enabled = !(vm.count("no_warnings") || vm.count("w"));
    ucfg.use_precompiled_headers = !vm.count("no_precompiled_headers");
    ucfg.saving_enabled = vm.count("enable_saving");
    ucfg.splash_enabled = vm.count("nosplash") == 0;
    if (vm.count("log") == 0)
    {
      ucfg.log_mode = logging_mode::none;
    }
    else
    {
      ucfg.log_mode =
        (ucfg.log_file == "-") ? logging_mode::console : logging_mode::file;
    }

    if (!fvalue.empty())
    {
      ucfg.max_template_depth = parse_max_template_depth(fvalue);
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
      show_mdb_help();
      return parse_config_result::exit(false);
    }
    else
    {
      return parse_config_result::start_shell(ucfg);
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
  r.action = with_error_ ? exit_with_error : exit_without_error;
  return r;
}

parse_config_result parse_config_result::start_shell(const user_config& cfg_)
{
  parse_config_result r;
  r.action = run_shell;
  r.cfg = cfg_;
  return r;
}

bool parse_config_result::should_run_shell() const
{
  return action == run_shell;
}

bool parse_config_result::should_error_at_exit() const
{
  return action == exit_with_error;
}

