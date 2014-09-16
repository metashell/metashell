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

#include <metashell/mdb_command.hpp>

#include <just/test.hpp>

#include "util.hpp"

using namespace metashell;

JUST_TEST_CASE(test_mdb_command_repeatable_constructor_test)
{
  mdb_command x({"asdf"}, repeatable, nullptr, "[asd]", "fdsa", "xxyy");

  JUST_ASSERT(equal(x.get_keys(), {"asdf"}));
  JUST_ASSERT(x.is_repeatable());
  JUST_ASSERT_EQUAL(x.get_usage(), "[asd]");
  JUST_ASSERT_EQUAL(x.get_short_description(), "fdsa");
  JUST_ASSERT_EQUAL(x.get_long_description(), "xxyy");
}

JUST_TEST_CASE(test_mdb_command_non_repeatable_constructor_test)
{
  mdb_command x({"asdf"}, non_repeatable, nullptr, "[asd]", "fdsa", "xxyy");

  JUST_ASSERT(equal(x.get_keys(), {"asdf"}));
  JUST_ASSERT(!x.is_repeatable());
  JUST_ASSERT_EQUAL(x.get_short_description(), "fdsa");
  JUST_ASSERT_EQUAL(x.get_long_description(), "xxyy");
}

JUST_TEST_CASE(test_mdb_command_multiple_keys_constructor_test)
{
  mdb_command x({"asdf", "xxx"}, non_repeatable, nullptr, "[asd]", "fd", "xx");

  JUST_ASSERT(equal(x.get_keys(), {"asdf", "xxx"}));
  JUST_ASSERT(!x.is_repeatable());
  JUST_ASSERT_EQUAL(x.get_usage(), "[asd]");
  JUST_ASSERT_EQUAL(x.get_short_description(), "fd");
  JUST_ASSERT_EQUAL(x.get_long_description(), "xx");
}

JUST_TEST_CASE(test_mdb_command_full_description_empty_long_description)
{
  mdb_command x({"asdf"}, non_repeatable, nullptr, "[asd]", "fd", "");

  JUST_ASSERT_EQUAL(x.get_usage(), "[asd]");
  JUST_ASSERT_EQUAL(x.get_short_description(), "fd");
  JUST_ASSERT_EQUAL(x.get_long_description(), "");
  JUST_ASSERT_EQUAL(x.get_full_description(), "fd");
}

JUST_TEST_CASE(test_mdb_command_full_description_non_empty_long_description)
{
  mdb_command x({"asdf"}, non_repeatable, nullptr, "[asd]", "fd", "xx");

  JUST_ASSERT_EQUAL(x.get_usage(), "[asd]");
  JUST_ASSERT_EQUAL(x.get_short_description(), "fd");
  JUST_ASSERT_EQUAL(x.get_long_description(), "xx");
  JUST_ASSERT_EQUAL(x.get_full_description(), "fd\nxx");
}

