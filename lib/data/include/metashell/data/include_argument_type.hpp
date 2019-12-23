#ifndef METASHELL_DATA_INCLUDE_ARGUMENT_TYPE_HPP
#define METASHELL_DATA_INCLUDE_ARGUMENT_TYPE_HPP

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

#include <array>
#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    enum class include_argument_type
    {
      iquote,
      capital_i,
      isystem,
      idirafter
    };

    std::string to_string(include_argument_type);
    std::ostream& operator<<(std::ostream&, include_argument_type);

    std::array<include_argument_type, 5> all_include_argument_types();
  }
}

#endif
