// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/data/file_location.hpp>

#include <gtest/gtest.h>

using namespace metashell::data;

TEST(file_location, empty)
{
  file_location f;
  ASSERT_EQ(f.name, "");
  ASSERT_EQ(f.row, -1);
  ASSERT_EQ(f.column, -1);
}

TEST(file_location, construction)
{
  file_location f("foo.cpp", 10, 20);
  ASSERT_EQ(f.name, "foo.cpp");
  ASSERT_EQ(f.row, 10);
  ASSERT_EQ(f.column, 20);
}

TEST(file_location, equality)
{
  ASSERT_EQ(file_location("foo.cpp", 10, 20), file_location("foo.cpp", 10, 20));
  ASSERT_EQ(file_location(), file_location());

  ASSERT_NE(file_location(), file_location("foo.cpp", 10, 20));

  ASSERT_NE(file_location("foo.cpp", 10, 20), file_location("foo.cpp", 10, 21));

  ASSERT_NE(file_location("foo.cpp", 10, 20), file_location("foo.cpp", 11, 20));

  ASSERT_NE(file_location("foo.cpp", 10, 20), file_location("bar.cpp", 10, 20));
}
