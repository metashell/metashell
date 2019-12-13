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

#include <metashell/system_test/code_completion_result.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/prompt.hpp>

#include <metashell/system_test/code_completer.hpp>
#include <metashell/system_test/metashell_instance.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

using pattern::_;

TEST(completion, simple)
{
  code_completer c;

  ASSERT_EQ(code_completion_result{}, c("no_completion_for_this"));

  ASSERT_EQ(code_completion_result{"e"}, c("doubl"));
  ASSERT_EQ((code_completion_result{"line", "t"}), c("in"));
  ASSERT_EQ(code_completion_result{}, c("decltype(\"in"));
  ASSERT_EQ(code_completion_result{}, c("int"));
}

TEST(completion, member)
{
  code_completer c(
      "struct foo"
      "{"
      "typedef foo type;"
      "typedef int bar;"

      "double mem1;"
      "char mem2;"
      "};");

  const code_completion_result members_of_foo{
      "~foo", "mem2", "mem1", "foo", "operator="};

  ASSERT_EQ(members_of_foo, c("decltype(foo()."));
  ASSERT_EQ(members_of_foo, c("decltype(((foo*)0)->"));

  ASSERT_EQ(members_of_foo.with({"type", "bar"}), c("foo::"));

  ASSERT_EQ((code_completion_result{"em1", "em2"}), c("decltype(foo().m"));

  ASSERT_EQ((code_completion_result{"em1", "em2"}), c("decltype(((foo*)0)->m"));
}

TEST(completion, template_member)
{
  code_completer c(
      "template <class T>"
      "struct foo"
      "{"
      "typedef T type;"
      "typedef int bar;"

      "double mem1;"
      "char mem2;"
      "};");

  const code_completion_result members_of_foo{
      "~foo", "mem2", "mem1", "foo", "operator="};

  ASSERT_EQ(members_of_foo, c("decltype(foo<int>()."));
  ASSERT_EQ(members_of_foo, c("decltype(((foo<int>*)0)->"));
  ASSERT_EQ(members_of_foo.with({"type", "bar"}), c("foo<int>::"));

  ASSERT_EQ((code_completion_result{"em1", "em2"}), c("decltype(foo<int>().m"));
  ASSERT_EQ(
      (code_completion_result{"em1", "em2"}), c("decltype(((foo<int>*)0)->m"));
}

TEST(completion, included)
{
  code_completer c("#include <vector>");

  const auto result = c("std::vecto");
  if (code_completion_result{"r", "r<_Ty,"} != result)
  {
    ASSERT_EQ((code_completion_result{"r", "r<_Tp,"}), result);
  }
}

TEST(completion, code_completion_cpp_can_not_be_included)
{
  metashell_instance mi;
  mi.code_completion("doubl");

  ASSERT_EQ(error(_), mi.command("#include <code_complete.cpp>").front());
}
