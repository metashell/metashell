// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "editline_shell.hpp"

#include <metashell/config.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include <vector>
#include <string>

int main(int argc_, char* argv_[])
{
  using boost::program_options::options_description;
  using boost::program_options::variables_map;
  using boost::program_options::store;
  using boost::program_options::notify;
  using boost::program_options::parse_command_line;
  using boost::program_options::value;

  metashell::config cfg;

  options_description desc("Options");
  desc.add_options()
    ("help", "Display help")
    ("include,I", value(&cfg.include_path), "Additional include directory")
    ("verbose,V", "Verbose mode")
    ;

  variables_map vm;
  store(parse_command_line(argc_, argv_, desc), vm);
  notify(vm);

  cfg.verbose = vm.count("verbose") || vm.count("V");

  if (vm.count("help"))
  {
    std::cout << desc << std::endl;
  }
  else
  {
    editline_shell shell(cfg);

    shell.display_splash();

    shell.run();
  }
}

