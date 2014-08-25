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

#include <metashell/metadebugger_command.hpp>

#include <just/test.hpp>

using namespace metashell;

JUST_TEST_CASE(mdb_command_constructor_test)
{
  metadebugger_command x("asdf", nullptr);

  JUST_ASSERT_EQUAL(x.get_key(), "asdf");
}

JUST_TEST_CASE(mdb_command_comparsion_with_mdb_command_1)
{
  metadebugger_command x("aaaa", nullptr), y("bbbb", nullptr);

  JUST_ASSERT(x < y);
  JUST_ASSERT(!(y < x));
}

JUST_TEST_CASE(mdb_command_comparsion_with_mdb_command_2)
{
  metadebugger_command x("aaaa", nullptr), y("aa", nullptr);

  JUST_ASSERT(y < x);
  JUST_ASSERT(!(x < y));
}

JUST_TEST_CASE(mdb_command_comparsion_with_string_1)
{
  metadebugger_command x("aaaa", nullptr);

  JUST_ASSERT(x < "bbbb");
}

JUST_TEST_CASE(mdb_command_comparsion_with_string_2)
{
  metadebugger_command x("aaaa", nullptr);

  JUST_ASSERT(!(x < "aa"));
}
