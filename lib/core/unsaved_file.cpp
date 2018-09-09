// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/core/exception.hpp>
#include <metashell/core/unsaved_file.hpp>

#include <boost/filesystem.hpp>

#include <fstream>

namespace metashell
{
  namespace core
  {
    void generate(const data::unsaved_file& f_)
    {
      boost::filesystem::path p(f_.filename());
      p.remove_filename();
      create_directories(p); // Throws when fails to create the directory
      const std::string filename = f_.filename().string();
      std::ofstream f(filename.c_str());
      if (f)
      {
        f << f_.content();
      }
      else
      {
        throw exception("Error creating file " + filename);
      }
    }
  }
}
