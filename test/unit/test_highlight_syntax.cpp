// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/core/highlight_syntax.hpp>

#include <gtest/gtest.h>

#include <algorithm>

using namespace metashell;

TEST(highlight_syntax, comment_without_linebreak)
{
  // wave can't lex this string, because there is no new line at the end
  data::colored_string cs =
      highlight_syntax(data::cpp_code("int x; // some comment"));

  // Still, we get back the original string (without syntax highlighting)
  ASSERT_EQ("int x; // some comment", cs.get_string());

  for (auto opt_color : cs.get_colors())
  {
    ASSERT_FALSE(bool(opt_color));
  }
}

TEST(highlight_syntax, comment_with_linebreak)
{
  data::colored_string cs =
      highlight_syntax(data::cpp_code("int x; // some comment\n"));

  ASSERT_EQ("int x; // some comment\n", cs.get_string());

  // keyword "int" is definietly colored
  ASSERT_TRUE(bool(cs.get_colors()[0]));
}
