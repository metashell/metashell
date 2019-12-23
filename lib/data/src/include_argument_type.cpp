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

#include <metashell/data/include_argument_type.hpp>

#include <cassert>
#include <iostream>

namespace metashell
{
  namespace data
  {
    std::string to_string(include_argument_type arg_)
    {
      switch (arg_)
      {
      case include_argument_type::iquote:
        return "-iquote";
      case include_argument_type::capital_i:
        return "-I";
      case include_argument_type::isystem:
        return "-isystem";
      case include_argument_type::idirafter:
        return "-idirafter";
      }
      assert(!"Invalid include argument type");
      return std::string();
    }

    std::ostream& operator<<(std::ostream& out_, include_argument_type arg_)
    {
      return out_ << to_string(arg_);
    }

    std::array<include_argument_type, 5> all_include_argument_types()
    {
      return {{include_argument_type::iquote, include_argument_type::capital_i,
               include_argument_type::isystem,
               include_argument_type::idirafter}};
    }
  }
}
