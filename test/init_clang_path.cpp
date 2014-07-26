// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/config.hpp>

#include <boost/filesystem/path.hpp>

#include <just/environment.hpp>

namespace
{
  std::string directory_of_file(const std::string& path_)
  {
    boost::filesystem::path p(path_);
    p.remove_filename();
    return p.string();
  }

  struct init_clang_path
  {
    init_clang_path()
    {
#ifdef _WIN32
       const metashell::config cfg = metashell::config::default_config();

      // To find libclang.dll
      if (!cfg.clang_path.empty())
      {
        just::environment::append_to_path(directory_of_file(cfg.clang_path));
      }
#endif

    }
  };

  init_clang_path do_init;
}

