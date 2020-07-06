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

#include <metashell/data/user_input.hpp>

#include <cctype>

namespace metashell
{
  namespace data
  {
    user_input::user_input(const cpp_code& c_)
      : string<user_input, true, constraint::code>(c_.value())
    {
    }

    user_input::operator cpp_code() const { return cpp_code(value()); }

    user_input::operator std::optional<mdb_command>() const
    {
      return mdb_command::parse(value());
    }

    bool has_non_whitespace(const user_input& s_)
    {
      for (char c : s_)
      {
        if (!std::isspace(c))
        {
          return true;
        }
      }
      return false;
    }
  }
}
