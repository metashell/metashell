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

#include "string_reader.hpp"

#include <gtest/gtest.h>

TEST(string_reader, empty)
{
  string_reader r{};

  const auto s = r(">");

  ASSERT_TRUE(std::nullopt == s);
}

TEST(string_reader, one_element)
{
  string_reader r{metashell::data::user_input("foo")};

  const auto s1 = r(">");
  const auto s2 = r(">");

  ASSERT_TRUE(std::nullopt != s1);
  ASSERT_EQ(metashell::data::user_input("foo"), *s1);
  ASSERT_TRUE(std::nullopt == s2);
}

TEST(string_reader, two_elements)
{
  string_reader r{
      metashell::data::user_input("foo"), metashell::data::user_input("bar")};

  const auto s1 = r(">");
  const auto s2 = r(">");
  const auto s3 = r(">");

  ASSERT_TRUE(std::nullopt != s1);
  ASSERT_EQ(metashell::data::user_input("foo"), *s1);
  ASSERT_TRUE(std::nullopt != s2);
  ASSERT_EQ(metashell::data::user_input("bar"), *s2);
  ASSERT_TRUE(std::nullopt == s3);
}
