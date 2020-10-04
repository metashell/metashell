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

#include <metashell/data/console_type.hpp>

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace metashell::data;

namespace
{
  std::string display_on_stream(console_type t_)
  {
    std::ostringstream s;
    s << t_;
    return s.str();
  }
} // namespace

TEST(console_type, parsing)
{
  ASSERT_EQ(console_type::plain, parse_console_type("plain"));
  ASSERT_EQ(console_type::readline, parse_console_type("readline"));
  ASSERT_EQ(console_type::json, parse_console_type("json"));
  ASSERT_THROW(parse_console_type("foo"), std::exception);
}

TEST(console_type, displaying)
{
  ASSERT_EQ("plain", display_on_stream(console_type::plain));
  ASSERT_EQ("readline", display_on_stream(console_type::readline));
  ASSERT_EQ("json", display_on_stream(console_type::json));
}
