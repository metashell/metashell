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

#include <just/test.hpp>

#include <sstream>
#include <iostream>
#include <limits>

using namespace metashell;

JUST_TEST_CASE(test_width_of_stream_console_is_max_int)
{
  std::ostringstream s;

  JUST_ASSERT_EQUAL(std::numeric_limits<int>::max(), stream_console(s).width());
}

JUST_TEST_CASE(test_displaying_new_line_with_stream_console)
{
  std::ostringstream s1;
  stream_console(s1).new_line();

  std::ostringstream s2;
  s2 << std::endl;

  JUST_ASSERT_EQUAL(s1.str(), s2.str());
}

JUST_TEST_CASE(test_text_is_displayed_on_stream_console_without_color)
{
  std::ostringstream s;
  stream_console(s).show(data::colored_string("hello", data::color::red));

  JUST_ASSERT_EQUAL("hello", s.str());
}

