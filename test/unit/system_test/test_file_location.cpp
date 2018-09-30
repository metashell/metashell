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

#include <metashell/system_test/file_location.hpp>

#include <gtest/gtest.h>

using metashell::system_test::file_location;
using metashell::system_test::json_string;

TEST(file_location, parsing_unix_filename)
{
  const file_location loc(json_string("\"/bar/foo.hpp\""));

  ASSERT_TRUE(loc.filename_specified());
  ASSERT_EQ("/bar/foo.hpp", loc.filename());

  ASSERT_FALSE(loc.row_specified());
  ASSERT_FALSE(loc.column_specified());
}

TEST(file_location, parsing_unix_filename_with_line)
{
  const file_location loc(json_string("\"/bar/foo.hpp:13\""));

  ASSERT_TRUE(loc.filename_specified());
  ASSERT_EQ("/bar/foo.hpp", loc.filename());

  ASSERT_TRUE(loc.row_specified());
  ASSERT_EQ(13, loc.row());

  ASSERT_FALSE(loc.column_specified());
}

TEST(file_location, parsing_unix_filename_with_line_and_col)
{
  const file_location loc(json_string("\"/bar/foo.hpp:13:8\""));

  ASSERT_TRUE(loc.filename_specified());
  ASSERT_EQ("/bar/foo.hpp", loc.filename());

  ASSERT_TRUE(loc.row_specified());
  ASSERT_EQ(13, loc.row());

  ASSERT_TRUE(loc.column_specified());
  ASSERT_EQ(8, loc.column());
}

TEST(file_location, parsing_win_filename)
{
  const file_location loc(json_string("\"C:\\\\foo.hpp\""));

  ASSERT_TRUE(loc.filename_specified());
  ASSERT_EQ("C:\\foo.hpp", loc.filename());

  ASSERT_FALSE(loc.row_specified());
  ASSERT_FALSE(loc.column_specified());
}

TEST(file_location, parsing_win_filename_with_line)
{
  const file_location loc(json_string("\"C:\\\\foo.hpp:13\""));

  ASSERT_TRUE(loc.filename_specified());
  ASSERT_EQ("C:\\foo.hpp", loc.filename());

  ASSERT_TRUE(loc.row_specified());
  ASSERT_EQ(13, loc.row());

  ASSERT_FALSE(loc.column_specified());
}

TEST(file_location, parsing_win_filename_with_line_and_col)
{
  const file_location loc(json_string("\"C:\\\\foo.hpp:13:8\""));

  ASSERT_TRUE(loc.filename_specified());
  ASSERT_EQ("C:\\foo.hpp", loc.filename());

  ASSERT_TRUE(loc.row_specified());
  ASSERT_EQ(13, loc.row());

  ASSERT_TRUE(loc.column_specified());
  ASSERT_EQ(8, loc.column());
}
