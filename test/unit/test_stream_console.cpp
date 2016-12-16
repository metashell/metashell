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

#include <metashell/stream_console.hpp>

#include <gtest/gtest.h>

#include <iostream>
#include <limits>
#include <sstream>

using namespace metashell;

TEST(stream_console, width_is_max_int)
{
  std::ostringstream s;

  ASSERT_EQ(std::numeric_limits<int>::max(), stream_console(s).width());
}

TEST(stream_console, displaying_new_line)
{
  std::ostringstream s1;
  stream_console(s1).new_line();

  std::ostringstream s2;
  s2 << std::endl;

  ASSERT_EQ(s1.str(), s2.str());
}

TEST(stream_console, text_is_displayed_without_color)
{
  std::ostringstream s;
  stream_console(s).show(data::colored_string("hello", data::color::red));

  ASSERT_EQ("hello", s.str());
}
