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

#include <metashell/data/standard_headers_allowed.hpp>

#include <cassert>
#include <iostream>

namespace metashell
{
  namespace data
  {
    standard_headers_allowed disable_c(standard_headers_allowed value_)
    {
      switch (value_)
      {
      case standard_headers_allowed::all:
        return standard_headers_allowed::cpp;
      case standard_headers_allowed::none:
      case standard_headers_allowed::c:
      case standard_headers_allowed::cpp:
        return standard_headers_allowed::none;
      }
      assert(!"Invalid standard_headers_allowed value");
      return standard_headers_allowed::none;
    }

    standard_headers_allowed disable_cpp(standard_headers_allowed value_)
    {
      switch (value_)
      {
      case standard_headers_allowed::all:
        return standard_headers_allowed::c;
      case standard_headers_allowed::none:
      case standard_headers_allowed::c:
      case standard_headers_allowed::cpp:
        return standard_headers_allowed::none;
      }
      assert(!"Invalid standard_headers_allowed value");
      return standard_headers_allowed::none;
    }

    std::string to_string(standard_headers_allowed value_)
    {
      switch (value_)
      {
      case standard_headers_allowed::none:
        return "no standard headers allowed";
      case standard_headers_allowed::c:
        return "no C++ standard headers allowed";
      case standard_headers_allowed::cpp:
        return "no C standard headers allowed";
      case standard_headers_allowed::all:
        return "all standard headers allowed";
      }
      assert(!"Invalid standard_headers_allowed value");
      return "";
    }

    std::ostream& operator<<(std::ostream& out_,
                             standard_headers_allowed value_)
    {
      return out_ << to_string(value_);
    }
  } // namespace data
} // namespace metashell
