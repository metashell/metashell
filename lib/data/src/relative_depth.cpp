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

#include <metashell/data/relative_depth.hpp>

#include <cassert>
#include <iostream>

namespace metashell
{
  namespace data
  {
    std::string to_string(relative_depth d_)
    {
      switch (d_)
      {
      case relative_depth::open:
        return "open";
      case relative_depth::flat:
        return "flat";
      case relative_depth::close:
        return "close";
      case relative_depth::end:
        return "end";
      }
      assert(!"Invalid relative_depth value");
      return "";
    }

    std::ostream& operator<<(std::ostream& out_, relative_depth d_)
    {
      return out_ << to_string(d_);
    }
  } // namespace data
} // namespace metashell
