// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <just/test.hpp>

#include "util.hpp"

JUST_TEST_CASE(test_integral_constant)
{
  JUST_ASSERT(!is_integral_constant("int", "13", ""));

  JUST_ASSERT(
    is_integral_constant("int", "13", "std::integral_constant<int, 13>")
  );

  JUST_ASSERT(!is_integral_constant("int", "13", "int"));

  JUST_ASSERT(
    is_integral_constant("int", "21", "std::integral_constant<int, 21>")
  );

  JUST_ASSERT(
    is_integral_constant(
      "unsigned int",
      "21",
      "std::integral_constant<unsigned int, 21>"
    )
  );

  JUST_ASSERT(
    is_integral_constant("int", "13", "std::_1::integral_constant<int, 13>")
  );

}

