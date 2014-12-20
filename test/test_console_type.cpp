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

using namespace metashell;

JUST_TEST_CASE(test_parsing_console_type)
{
  JUST_ASSERT_EQUAL(console_type::plain, parse_console_type("plain"));
  JUST_ASSERT_EQUAL(console_type::readline, parse_console_type("readline"));
  JUST_ASSERT_THROWS(std::exception, parse_console_type("foo"));
}

