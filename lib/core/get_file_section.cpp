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

#include <metashell/core/get_file_section.hpp>

#include <fstream>
#include <sstream>

namespace metashell
{

  file_section
  get_file_section(std::istream& stream, int middle_line, int offset)
  {
    if (middle_line <= 0 || offset < 0)
    {
      return {};
    }

    file_section result;

    std::string line;
    int i = 1;
    for (; std::getline(stream, line); ++i)
    {
      if (i > middle_line + offset)
      {
        break;
      }
      if (i < middle_line - offset)
      {
        continue;
      }
      result.push_back({i, line});
    }
    if (i - 1 < middle_line)
    {
      return {};
    }
    return result;
  }

  file_section get_file_section_from_file(
      const boost::filesystem::path& file_name, int middle_line, int offset)
  {
    std::ifstream in(file_name.string());
    if (!in)
    {
      return {};
    }
    return get_file_section(in, middle_line, offset);
  }

  file_section get_file_section_from_buffer(const std::string& buffer,
                                            int middle_line,
                                            int offset)
  {
    std::stringstream ss(buffer);
    return get_file_section(ss, middle_line, offset);
  }

} // namespace metashell
