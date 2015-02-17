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

#include <just/test.hpp>

using namespace metashell::data;

JUST_TEST_CASE(test_empty_file_location)
{
  file_location f;
  JUST_ASSERT_EQUAL(f.name, "");
  JUST_ASSERT_EQUAL(f.row, -1);
  JUST_ASSERT_EQUAL(f.column, -1);
}

JUST_TEST_CASE(test_file_location_construction)
{
  file_location f("foo.cpp", 10, 20);
  JUST_ASSERT_EQUAL(f.name, "foo.cpp");
  JUST_ASSERT_EQUAL(f.row, 10);
  JUST_ASSERT_EQUAL(f.column, 20);
}

JUST_TEST_CASE(test_file_location_equality)
{
  JUST_ASSERT_EQUAL(
      file_location("foo.cpp", 10, 20),
      file_location("foo.cpp", 10, 20));
  JUST_ASSERT_EQUAL(
      file_location(),
      file_location());

  JUST_ASSERT_NOT_EQUAL(
      file_location(),
      file_location("foo.cpp", 10, 20));

  JUST_ASSERT_NOT_EQUAL(
      file_location("foo.cpp", 10, 20),
      file_location("foo.cpp", 10, 21));

  JUST_ASSERT_NOT_EQUAL(
      file_location("foo.cpp", 10, 20),
      file_location("foo.cpp", 11, 20));

  JUST_ASSERT_NOT_EQUAL(
      file_location("foo.cpp", 10, 20),
      file_location("bar.cpp", 10, 20));
}
