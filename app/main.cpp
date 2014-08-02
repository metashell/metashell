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

#include <metashell/parse_config.hpp>
#include <metashell/config.hpp>
#include <metashell/clang_binary.hpp>

#include <just/environment.hpp>

#include <boost/filesystem/path.hpp>

#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#  include <windows.h>
#endif

namespace
{
  std::string directory_of_file(const std::string& path_)
  {
    boost::filesystem::path p(path_);
    p.remove_filename();
    return p.string();
  }
  
#ifdef _WIN32
  std::string path_of_executable()
  {
    char path[MAX_PATH];
    GetModuleFileName(GetModuleHandle(NULL), path, sizeof(path));
    return path;
  }
#endif
}

int main(int argc_, const char* argv_[])
{
  try
  {
    using metashell::parse_config;
    using metashell::parse_config_result;

    metashell::config cfg = metashell::config::default_config();

    const parse_config_result
      r = parse_config(cfg, argc_, argv_, &std::cout, &std::cerr);

    if (!cfg.clang_path.empty())
    {
#ifdef _WIN32
      // To find libclang.dll
      just::environment::append_to_path(directory_of_file(cfg.clang_path));
#endif

      // clang's headers
      std::vector<std::string> si =
        default_sysinclude(metashell::clang_binary(cfg.clang_path));

#ifdef _WIN32
      // mingw headers shipped with Metashell
      const std::string mingw_headers =
        directory_of_file(path_of_executable()) + "\\windows_headers";
      si.push_back(mingw_headers);
      si.push_back(mingw_headers + "\\mingw32");
#endif

      cfg.include_path.insert(cfg.include_path.begin(), si.begin(), si.end());
    }

    if (r == metashell::run_shell)
    {
      readline_shell shell(cfg);
      shell.display_splash();
      shell.run();
    }
    return r == metashell::exit_with_error ? 1 : 0;
  }
  catch (std::exception& e_)
  {
    std::cerr << "Error: " << e_.what() << std::endl;
  }
}

