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

#include <metashell/data/macro_undefinition.hpp>

#include <iostream>

namespace metashell
{
  namespace data
  {
    macro_undefinition::macro_undefinition(const command_line_argument& name_)
      : _name(name_.value())
    {
    }

    command_line_argument clang_argument(const macro_undefinition& undef_)
    {
      return command_line_argument("-U" + undef_.value());
    }

    const std::string& macro_undefinition::value() const { return _name; }

    std::string to_string(const macro_undefinition& undef_)
    {
      return undef_.value();
    }

    std::ostream& operator<<(std::ostream& out_,
                             const macro_undefinition& undef_)
    {
      return out_ << to_string(undef_);
    }
  }
}
