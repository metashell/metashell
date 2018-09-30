// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/pop_frame.hpp>

#include <iostream>

namespace metashell
{
  namespace data
  {
    bool operator==(pop_frame, pop_frame) { return true; }

    std::string to_string(pop_frame) { return "pop_frame{}"; }

    std::ostream& operator<<(std::ostream& out_, pop_frame f_)
    {
      return out_ << to_string(f_);
    }
  }
}
