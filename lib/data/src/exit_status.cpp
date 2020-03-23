// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/exit_status.hpp>

#include <iostream>

namespace metashell
{
  namespace data
  {
    exit_status::exit_status(unsigned long value_) : _value(value_) {}

    unsigned long exit_status::value() const { return _value; }

    bool operator==(const exit_status& lhs_, const exit_status& rhs_)
    {
      return lhs_.value() == rhs_.value();
    }

    std::string to_string(const exit_status& e_)
    {
      return std::to_string(e_.value());
    }

    std::ostream& operator<<(std::ostream& out_, const exit_status& e_)
    {
      return out_ << e_.value();
    }
  }
}
