#ifndef METASHELL_GET_FILE_SECTION_HPP
#define METASHELL_GET_FILE_SECTION_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <istream>
#include <string>
#include <vector>

#include <boost/filesystem/path.hpp>

namespace metashell
{

  struct indexed_line
  {
    int line_index;
    std::string line;
  };

  using file_section = std::vector<indexed_line>;

  // If there are enough lines in the stream, this will return that line and
  // +/- offset lines before and after that line
  file_section
  get_file_section(std::istream& stream, int middle_line, int offset);

  file_section get_file_section_from_file(
      const boost::filesystem::path& file_name, int middle_line, int offset);

  file_section get_file_section_from_buffer(const std::string& buffer,
                                            int middle_line,
                                            int offset);

} // namespace metashell

#endif
