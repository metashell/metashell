#ifndef METASHELL_DATA_MACRO_UNDEFINITION_HPP
#define METASHELL_DATA_MACRO_UNDEFINITION_HPP

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

#include <metashell/data/command_line_argument.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    class macro_undefinition
    {
    public:
      explicit macro_undefinition(const command_line_argument&);

      const std::string& value() const;

    private:
      std::string _name;
    };

    command_line_argument clang_argument(const macro_undefinition&);

    std::string to_string(const macro_undefinition&);
    std::ostream& operator<<(std::ostream&, const macro_undefinition&);
  } // namespace data
} // namespace metashell

#endif
