#ifndef METASHELL_DATA_COMMAND_LINE_HPP
#define METASHELL_DATA_COMMAND_LINE_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/command_line_argument_list.hpp>
#include <metashell/data/executable_path.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    class command_line
    {
    public:
      command_line(executable_path, command_line_argument_list);

      std::vector<const char*> argv() const;

      const executable_path& executable() const;
      const command_line_argument_list& arguments() const;

    private:
      executable_path _exe;
      command_line_argument_list _args;
    };

    std::ostream& operator<<(std::ostream&, const command_line&);
    std::string to_string(const command_line&);
  } // namespace data
} // namespace metashell

#endif
