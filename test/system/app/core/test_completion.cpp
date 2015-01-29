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

#include <metashell_system_test/prompt.hpp>
#include <metashell_system_test/code_completion_result.hpp>

#include <metashell_system_test/json_generator.hpp>
#include <metashell_system_test/code_completer.hpp>

#include <just/test.hpp>

using namespace metashell_system_test;

JUST_TEST_CASE(test_simple_completion)
{
  code_completer c;

  JUST_ASSERT_EQUAL(code_completion_result{}, c("no_completion_for_this"));

  JUST_ASSERT_EQUAL(code_completion_result{"e"}, c("doubl"));
  JUST_ASSERT_EQUAL(code_completion_result{"line", "t"}, c("in"));
  JUST_ASSERT_EQUAL(code_completion_result{}, c("decltype(\"in"));
  JUST_ASSERT_EQUAL(code_completion_result{}, c("int"));
}

JUST_TEST_CASE(test_member_completion)
{
  code_completer
    c(
      "struct foo"
      "{"
        "typedef foo type;"
        "typedef int bar;"

        "double mem1;"
        "char mem2;"
      "};"
    );

  const code_completion_result
    members_of_foo{"~foo", "mem2", "mem1", "foo", "operator="};

  JUST_ASSERT_EQUAL(members_of_foo, c("decltype(foo()."));
  JUST_ASSERT_EQUAL(members_of_foo, c("decltype(((foo*)0)->"));

  JUST_ASSERT_EQUAL(members_of_foo.with({"type", "bar"}), c("foo::"));

  JUST_ASSERT_EQUAL(
    code_completion_result{"em1", "em2"},
    c("decltype(foo().m")
  );

  JUST_ASSERT_EQUAL(
    code_completion_result{"em1", "em2"},
    c("decltype(((foo*)0)->m")
  );
}

JUST_TEST_CASE(test_template_member_completion)
{
  code_completer
    c(
      "template <class T>"
      "struct foo"
      "{"
        "typedef T type;"
        "typedef int bar;"

        "double mem1;"
        "char mem2;"
      "};"
    );

  const code_completion_result
    members_of_foo{"~foo", "mem2", "mem1", "foo", "operator="};

  JUST_ASSERT_EQUAL(members_of_foo, c("decltype(foo<int>()."));
  JUST_ASSERT_EQUAL(members_of_foo, c( "decltype(((foo<int>*)0)->"));
  JUST_ASSERT_EQUAL(members_of_foo.with({"type", "bar"}), c("foo<int>::"));

  JUST_ASSERT_EQUAL(
    code_completion_result{"em1", "em2"},
    c("decltype(foo<int>().m")
  );
  JUST_ASSERT_EQUAL(
    code_completion_result{"em1", "em2"},
    c("decltype(((foo<int>*)0)->m")
  );
}

JUST_TEST_CASE(test_included_completion)
{
  code_completer c("#include <vector>");

  JUST_ASSERT_EQUAL(code_completion_result{"r"}, c("std::vecto"));
}

