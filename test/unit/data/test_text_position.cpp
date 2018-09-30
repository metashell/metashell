// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/text_position.hpp>

#include <gtest/gtest.h>

using namespace metashell::data;

TEST(text_position, tests)
{
  const text_position s;

  ASSERT_EQ(text_position(1, 1), s);
  ASSERT_EQ(text_position(1, 2), s + "x");
  ASSERT_EQ(text_position(2, 1), s + "x\n");
  ASSERT_EQ(text_position(2, 1), s + "x\r");
  ASSERT_EQ(text_position(2, 1), s + "x\r\n");
  ASSERT_EQ(text_position(3, 1), s + "x\n\r");
  ASSERT_EQ(text_position(3, 1), s + "x\r\r");
  ASSERT_EQ(text_position(3, 1), s + "x\n\n");
  ASSERT_EQ(text_position(3, 1), s + "x\r\n\n");
  ASSERT_EQ(text_position(3, 1), s + "x\r\n\r");
}
