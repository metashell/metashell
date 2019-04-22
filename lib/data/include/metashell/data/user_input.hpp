#ifndef METASHELL_DATA_USER_INPUT_HPP
#define METASHELL_DATA_USER_INPUT_HPP

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

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/mdb_command.hpp>
#include <metashell/data/string.hpp>

namespace metashell
{
  namespace data
  {
    class user_input : string<user_input>
    {
    public:
      using string<user_input>::string;
      using string<user_input>::value;

      explicit user_input(const cpp_code&);

      explicit operator cpp_code() const;
      explicit operator mdb_command() const;
    };

    bool has_non_whitespace(const user_input&);
  }
}

#endif
