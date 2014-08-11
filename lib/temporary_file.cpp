
// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/temporary_file.hpp>

namespace metashell {

temporary_file::temporary_file(
    const boost::filesystem::path& model)
{
  boost::filesystem::path temporary_file_model =
    boost::filesystem::temp_directory_path();
  temporary_file_model /= model;

  path = boost::filesystem::unique_path(temporary_file_model);
}

temporary_file::~temporary_file() {
  if (boost::filesystem::exists(path)) {
    boost::filesystem::remove(path);
  }
}

boost::filesystem::path temporary_file::get_path() const {
  return path;
}

}

