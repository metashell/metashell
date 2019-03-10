// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/token_type.hpp>

#include <cassert>
#include <iostream>

namespace metashell
{
  namespace data
  {
    std::string to_string(token_type t_)
    {
      switch (t_)
      {
#ifdef METASHELL_TOKEN_TYPE
#error METASHELL_TOKEN_TYPE already defined
#endif
#define METASHELL_TOKEN_TYPE(name, cat, format)                                \
  case token_type::name:                                                       \
    return #name;

#include <metashell/data/impl/token_type_list.hpp>

#undef METASHELL_TOKEN_TYPE
      }
      assert(false);
      return "";
    }

    std::ostream& operator<<(std::ostream& out_, token_type t_)
    {
      return out_ << to_string(t_);
    }
  }
}
