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

#include <metashell/data/type.hpp>

#include <gtest/gtest.h>

#include "util.hpp"

using namespace metashell::data;

TEST(test_type, test_integral_constant)
{
  const type int_("int");

  ASSERT_FALSE(type("").is_integral_constant(int_, "13"));

  ASSERT_TRUE(
      type("std::integral_constant<int, 13>").is_integral_constant(int_, "13"));

  ASSERT_FALSE(type("int").is_integral_constant(int_, "13"));

  ASSERT_TRUE(
      type("std::integral_constant<int, 21>").is_integral_constant(int_, "21"));

  ASSERT_TRUE(type("std::integral_constant<unsigned int, 21>")
                  .is_integral_constant(type("unsigned int"), "21"));

  ASSERT_TRUE(type("std::_1::integral_constant<int, 13>")
                  .is_integral_constant(int_, "13"));
}
