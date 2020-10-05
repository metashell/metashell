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

#include <metashell/data/macro_definition.hpp>

#include <iostream>

namespace metashell
{
  namespace data
  {
    macro_definition::macro_definition(const command_line_argument& arg_)
      : _value(arg_.value())
    {
    }

    macro_definition::macro_definition(std::string value_)
      : _value(std::move(value_))
    {
    }

    const std::string& macro_definition::value() const { return _value; }

    command_line_argument clang_argument(const macro_definition& def_)
    {
      return command_line_argument("-D" + def_.value());
    }

    std::string to_string(const macro_definition& def_) { return def_.value(); }

    std::ostream& operator<<(std::ostream& out_, const macro_definition& def_)
    {
      return out_ << to_string(def_);
    }
  } // namespace data
} // namespace metashell
