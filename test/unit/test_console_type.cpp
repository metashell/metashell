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

#include <metashell/console_type.hpp>

#include <just/test.hpp>

#include <stdexcept>
#include <iostream>
#include <sstream>

using namespace metashell;

namespace
{
  std::string display_on_stream(console_type t_)
  {
    std::ostringstream s;
    s << t_;
    return s.str();
  }
}

JUST_TEST_CASE(test_parsing_console_type)
{
  JUST_ASSERT_EQUAL(console_type::plain, parse_console_type("plain"));
  JUST_ASSERT_EQUAL(console_type::readline, parse_console_type("readline"));
  JUST_ASSERT_EQUAL(console_type::json, parse_console_type("json"));
  JUST_ASSERT_THROWS(std::exception, parse_console_type("foo"));
}

JUST_TEST_CASE(test_displaying_console_type)
{
  JUST_ASSERT_EQUAL("plain", display_on_stream(console_type::plain));
  JUST_ASSERT_EQUAL("readline", display_on_stream(console_type::readline));
  JUST_ASSERT_EQUAL("json", display_on_stream(console_type::json));
}

