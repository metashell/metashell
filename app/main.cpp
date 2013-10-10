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

#include "readline_shell.hpp"

#include <metashell/config.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include <vector>
#include <string>

namespace
{
  enum parse_config_result
  {
    run_shell,
    exit_with_error,
    exit_without_error
  };

  parse_config_result parse_config(
    metashell::config& cfg_,
    int argc_,
    char* argv_[]
  )
  {
    using boost::program_options::options_description;
    using boost::program_options::variables_map;
    using boost::program_options::store;
    using boost::program_options::notify;
    using boost::program_options::parse_command_line;
    using boost::program_options::value;

    std::string cppstd("c++0x");

    options_description desc("Options");
    desc.add_options()
      ("help", "Display help")
      ("include,I", value(&cfg_.include_path), "Additional include directory")
      ("verbose,V", "Verbose mode")
      ("no_highlight,H", "Disable syntax highlighting")
      ("indent", "Enable indenting (experimental)")
      (
        "std", value(&cppstd),
        "C++ standard to use. Possible values: c++0x/c++11, c++1y/c++14."
      )
      ;

    try
    {
      variables_map vm;
      store(parse_command_line(argc_, argv_, desc), vm);
      notify(vm);

      cfg_.verbose = vm.count("verbose") || vm.count("V");
      cfg_.syntax_highlight = !(vm.count("no_highlight") || vm.count("H"));
      cfg_.indent = vm.count("indent");
      cfg_.standard_to_use = metashell::parse(cppstd);

      if (vm.count("help"))
      {
        std::cout << desc << std::endl;
        return exit_without_error;
      }
      else
      {
        return run_shell;
      }
    }
    catch (const std::exception& e_)
    {
      std::cerr << e_.what() << "\n\n" << desc << std::endl;
      return exit_with_error;
    }
  }
}

int main(int argc_, char* argv_[])
{
  metashell::config cfg;

  const parse_config_result r = parse_config(cfg, argc_, argv_);

  if (r == run_shell)
  {
    readline_shell shell(cfg);
    shell.display_splash();
    shell.run();
  }
  return r == exit_with_error ? 1 : 0;
}

