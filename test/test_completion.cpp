// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/in_memory_displayer.hpp>
#include <metashell/shell.hpp>

#include "string_set.hpp"
#include "test_config.hpp"

#include <just/test.hpp>

JUST_TEST_CASE(test_simple_completion)
{
  metashell::in_memory_displayer d;
  metashell::shell sh(metashell::test_config(), d);

  JUST_ASSERT_EQUAL(string_set(), string_set(sh, "no_completion_for_this"));
  JUST_ASSERT_EQUAL(string_set("e"), string_set(sh, "doubl"));
  JUST_ASSERT_EQUAL(string_set("line", "t"), string_set(sh, "in"));
  JUST_ASSERT_EQUAL(string_set(), string_set(sh, "decltype(\"in"));
  JUST_ASSERT_EQUAL(string_set(), string_set(sh, "int"));
}

JUST_TEST_CASE(test_member_completion)
{
  metashell::in_memory_displayer d;
  metashell::shell sh(metashell::test_config(), d);
  sh.store_in_buffer(
    "struct foo"
    "{"
      "typedef foo type;"
      "typedef int bar;"

      "double mem1;"
      "char mem2;"
    "};"
  );

  const string_set members_of_foo("~foo", "mem2", "mem1", "foo", "operator=");

  JUST_ASSERT_EQUAL(members_of_foo, string_set(sh, "decltype(foo()."));
  JUST_ASSERT_EQUAL(members_of_foo, string_set(sh, "decltype(((foo*)0)->"));

  JUST_ASSERT_EQUAL(
    members_of_foo + string_set("type", "bar"),
    string_set(sh, "foo::")
  );

  JUST_ASSERT_EQUAL(
    string_set("em1", "em2"),
    string_set(sh, "decltype(foo().m")
  );
  JUST_ASSERT_EQUAL(
    string_set("em1", "em2"),
    string_set(sh, "decltype(((foo*)0)->m")
  );
}

JUST_TEST_CASE(test_template_member_completion)
{
  metashell::in_memory_displayer d;
  metashell::shell sh(metashell::test_config(), d);
  sh.store_in_buffer(
    "template <class T>"
    "struct foo"
    "{"
      "typedef T type;"
      "typedef int bar;"

      "double mem1;"
      "char mem2;"
    "};"
  );

  const string_set members_of_foo("~foo", "mem2", "mem1", "foo", "operator=");

  JUST_ASSERT_EQUAL(members_of_foo, string_set(sh, "decltype(foo<int>()."));
  JUST_ASSERT_EQUAL(
    members_of_foo,
    string_set(sh, "decltype(((foo<int>*)0)->")
  );

  JUST_ASSERT_EQUAL(
    members_of_foo + string_set("type", "bar"),
    string_set(sh, "foo<int>::")
  );

  JUST_ASSERT_EQUAL(
    string_set("em1", "em2"),
    string_set(sh, "decltype(foo<int>().m")
  );
  JUST_ASSERT_EQUAL(
    string_set("em1", "em2"),
    string_set(sh, "decltype(((foo<int>*)0)->m")
  );
}

JUST_TEST_CASE(test_included_completion)
{
  metashell::in_memory_displayer d;
  metashell::shell sh(metashell::test_config(), d);
  sh.store_in_buffer("#include <vector>");

  JUST_ASSERT_EQUAL(string_set("r"), string_set(sh, "std::vecto"));
}

