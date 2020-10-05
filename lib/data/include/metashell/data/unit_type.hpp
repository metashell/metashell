#ifndef METASHELL_DATA_UNIT_TYPE_HPP
#define METASHELL_DATA_UNIT_TYPE_HPP

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

#include <iostream>
#include <string>

namespace metashell
{
  namespace data
  {
    template <class Derived>
    struct unit_type
    {
      friend bool operator==(Derived, Derived) { return true; }
      friend bool operator!=(Derived, Derived) { return false; }

      friend std::string to_string(Derived) { return Derived::name(); }

      friend std::ostream& operator<<(std::ostream& out_, Derived)
      {
        out_ << Derived::name();
        return out_;
      }
    };
  } // namespace data
} // namespace metashell

#endif
