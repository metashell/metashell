// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/replace_part/replace_part.hpp>

#include <gtest/gtest.h>

using replace_part::replace;

TEST(replace_part, no_replacing)
{
  ASSERT_EQ("", replace("", "NOT THERE", "x"));

  ASSERT_EQ("hello", replace("hello", "NOT THERE", "x"));

  ASSERT_EQ("hello\nworld\n", replace("hello\nworld\n", "NOT THERE", "x"));
  ASSERT_EQ("hello\nworld", replace("hello\nworld", "NOT THERE", "x"));
  ASSERT_EQ("hello\rworld", replace("hello\rworld", "NOT THERE", "x"));
  ASSERT_EQ("hello\r\nworld", replace("hello\r\nworld", "NOT THERE", "x"));
  ASSERT_EQ("hello\rworld\n", replace("hello\rworld\n", "NOT THERE", "x"));
  ASSERT_EQ("hello\r\nworld\n", replace("hello\r\nworld\n", "NOT THERE", "x"));

  ASSERT_EQ("\n", replace("\n", "NOT THERE", "x"));
  ASSERT_EQ("\r", replace("\r", "NOT THERE", "x"));
  ASSERT_EQ("\r\n", replace("\r\n", "NOT THERE", "x"));

  ASSERT_EQ("\n\n", replace("\n\n", "NOT THERE", "x"));
  ASSERT_EQ("\r\r", replace("\r\r", "NOT THERE", "x"));
  ASSERT_EQ("\r\n\r\n", replace("\r\n\r\n", "NOT THERE", "x"));

  ASSERT_EQ("\n\r\n", replace("\n\r\n", "NOT THERE", "x"));
}

TEST(replace_part, replacing)
{
  ASSERT_EQ("helloworld\n", replace("hello", "hello", "world"));
  ASSERT_EQ("hello\nworld\n", replace("hello\nworld", "hello", "world"));
  ASSERT_EQ("xhelloz\nworld\n", replace("xhelloz\nfoo", "hello", "world"));
  ASSERT_EQ("xhelloz\nworld\nyhellor\nbar",
            replace("xhelloz\nfoo\nyhellor\nbar", "hello", "world"));
  ASSERT_EQ("xhelloz\rworld\nyhellor\rbar",
            replace("xhelloz\rfoo\ryhellor\rbar", "hello", "world"));
  ASSERT_EQ("xhelloz\r\nworld\nyhellor\r\nbar",
            replace("xhelloz\r\nfoo\r\nyhellor\r\nbar", "hello", "world"));
}
