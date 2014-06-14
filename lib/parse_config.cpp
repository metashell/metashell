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
#include <metashell/shell_stub.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include <boost/algorithm/string/join.hpp>

#include <boost/foreach.hpp>

#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>

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

  const char* default_clang_search_path[] =
    {
      ""
      #include "default_clang_search_path.hpp"
    };

  void show_markdown(
    const std::vector<std::string>& name_,
    const pragma_handler& h_,
    std::ostream& out_
  )
  {
    using boost::algorithm::join;
    using std::endl;

    const std::string args = h_.arguments();

    out_
      << "`#pragma metashell " << join(name_, " ") << (args.empty() ? "" : " ")
      << args << "`" << endl
      << endl
      << h_.description() << endl
      << endl;
  }

  void show_pragma_help()
  {
    shell_stub sh;
    const pragma_handler_map m = pragma_handler_map::build_default(sh);

    typedef std::pair<std::vector<std::string>, pragma_handler> sp;
    BOOST_FOREACH(const sp& p, m)
    {
      show_markdown(p.first, p.second, std::cout);
    }
  }
}

parse_config_result metashell::parse_config(
  config& cfg_,
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

  const char** const
    minus_minus = std::find(argv_, argv_ + argc_, std::string("--"));
  if (minus_minus != argv_ + argc_)
  {
    cfg_.extra_clang_args.insert(
      cfg_.extra_clang_args.end(),
      minus_minus + 1,
      argv_ + argc_
    );
  }
  const int argc = minus_minus - argv_;

  std::string cppstd("c++0x");
  cfg_.use_precompiled_headers = !cfg_.clang_path.empty();

  options_description desc("Options");
  desc.add_options()
    ("help", "Display help")
    ("include,I", value(&cfg_.include_path), "Additional include directory")
    ("define,D", value(&cfg_.macros), "Additional macro definitions")
    ("verbose,V", "Verbose mode")
    ("no_highlight,H", "Disable syntax highlighting")
    ("indent", "Enable indenting (experimental)")
    (
      "std", value(&cppstd),
      "C++ standard to use. Possible values: c++0x/c++11, c++1y/c++14."
    )
    ("no_warnings,w", "Disable warnings")
    (
      "no_precompiled_headers",
      "Disable precompiled header usage."
      " (It needs clang++ to be available and writes to the local disc.)"
    )
    (
      "clang", value(&cfg_.clang_path),
      "The path of the clang++ binary to use for"
      " generating precompiled headers."
    )
    (
      "show_pragma_help",
      "Display help for pragmas in MarkDown format and exit."
    )
    ;

  try
  {
    variables_map vm;
    store(parse_command_line(argc, argv_, desc), vm);
    notify(vm);

    cfg_.verbose = vm.count("verbose") || vm.count("V");
    cfg_.syntax_highlight = !(vm.count("no_highlight") || vm.count("H"));
    cfg_.indent = vm.count("indent");
    cfg_.standard_to_use = metashell::parse(cppstd);
    cfg_.warnings_enabled = !(vm.count("no_warnings") || vm.count("w"));
    cfg_.use_precompiled_headers = !vm.count("no_precompiled_headers");

    if (vm.count("help"))
    {
      if (out_)
      {
        show_help(*out_, desc);
      }
      return exit_without_error;
    }
    else if (vm.count("show_pragma_help"))
    {
      show_pragma_help();
      return exit_without_error;
    }
    else
    {
      if (cfg_.use_precompiled_headers && !file_exists(cfg_.clang_path))
      {
        std::cerr << "Error: clang++ not found. Checked:" << std::endl;
        if (vm.count("clang"))
        {
          std::cerr << cfg_.clang_path << std::endl;
        }
        else
        {
          std::copy(
            default_clang_search_path + 1,
            default_clang_search_path
              + sizeof(default_clang_search_path) / sizeof(const char*),
            std::ostream_iterator<std::string>(std::cerr, "\n")
          );
        }
        std::cerr << "Disabling precompiled headers" << std::endl;
        cfg_.use_precompiled_headers = false;
      }
      return run_shell;
    }
  }
  catch (const std::exception& e_)
  {
    if (err_)
    {
      *err_ << e_.what() << "\n\n";
      show_help(*err_, desc);
    }
    return exit_with_error;
  }
}

