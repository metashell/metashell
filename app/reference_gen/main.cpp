// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include "cmd_t.hpp"
#include "commands.hpp"
#include "file_cache.hpp"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <boost/filesystem.hpp>

#include <iostream>
#include <stdexcept>

int main(int argc_, const char* argv_[])
{
  boost::filesystem::path docs_dir;

  boost::program_options::options_description desc("Options");
  // clang-format off
  desc.add_options()
      ("help,h", "Display usage")
      ("test,t", "Don't update, just validate the generated content.")
      (
        "docs_dir,d",
        boost::program_options::value(&docs_dir),
        "Path to the docs dir of the source code."
      );
  // clang-format on

  try
  {
    boost::program_options::variables_map vm;
    store(parse_command_line(argc_, argv_, desc), vm);
    notify(vm);

    if (vm.count("help"))
    {
      std::cout << desc << std::endl;
      return 0;
    }

    if (docs_dir.empty())
    {
      throw std::runtime_error("Please specify the -d <docs dir> argument");
    }

    const std::vector<cmd_t> commands = get_commands(docs_dir);

    file_cache files(commands);

    for (const auto& cmd : commands)
    {
      files[cmd.path()] = cmd.generate(files[cmd.path()]);
    }

    if (files.changed())
    {
      if (vm.count("test"))
      {
        std::cerr << "Automatically generated documentation is out of date. "
                     "Please run the following command:"
                  << std::endl
                  << canonical(boost::filesystem::path(argv_[0])) << " -d "
                  << canonical(docs_dir) << std::endl;
        return 2;
      }
      else
      {
        files.write_changes();
      }
    }

    return 0;
  }
  catch (std::exception& e_)
  {
    std::cerr << "Error: " << e_.what() << std::endl << desc << std::endl;
    return 1;
  }
}
