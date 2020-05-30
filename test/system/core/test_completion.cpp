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

namespace
{
  template <class... Ts>
  code_completion_result res(Ts... ts_)
  {
    return code_completion_result{ts_...};
  }
}

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

TEST(completion, preprocessor_directives)
{
  code_completer c;
  const std::string spaces[] = {"", "  "};

  for (const auto& prefix : spaces)
  {
    for (const auto& middle : spaces)
    {
      const auto complete = [&prefix, &middle, &c](const std::string& s_) {
        return c(prefix + "#" + middle + s_);
      };

      ASSERT_EQ(complete("d"), res("efine"));
      ASSERT_EQ(complete("de"), res("fine"));
      ASSERT_EQ(complete("def"), res("ine"));
      ASSERT_EQ(complete("defi"), res("ne"));
      ASSERT_EQ(complete("defin"), res("e"));
      ASSERT_EQ(complete("define"), res(""));
      ASSERT_EQ(complete("e"), res("lif", "lse", "ndif", "rror"));
      ASSERT_EQ(complete("el"), res("if", "se"));
      ASSERT_EQ(complete("eli"), res("f"));
      ASSERT_EQ(complete("elif"), res(""));
      ASSERT_EQ(complete("els"), res("e"));
      ASSERT_EQ(complete("else"), res(""));
      ASSERT_EQ(complete("en"), res("dif"));
      ASSERT_EQ(complete("end"), res("if"));
      ASSERT_EQ(complete("endi"), res("f"));
      ASSERT_EQ(complete("endif"), res(""));
      ASSERT_EQ(complete("er"), res("ror"));
      ASSERT_EQ(complete("err"), res("or"));
      ASSERT_EQ(complete("erro"), res("r"));
      ASSERT_EQ(complete("error"), res(""));
      ASSERT_EQ(complete("i"), res("f", "fdef", "fndef", "nclude"));
      ASSERT_EQ(complete("if"), res("", "def", "ndef"));
      ASSERT_EQ(complete("ifd"), res("ef"));
      ASSERT_EQ(complete("ifde"), res("f"));
      ASSERT_EQ(complete("ifdef"), res(""));
      ASSERT_EQ(complete("ifn"), res("def"));
      ASSERT_EQ(complete("ifnd"), res("ef"));
      ASSERT_EQ(complete("ifnde"), res("f"));
      ASSERT_EQ(complete("ifndef"), res(""));
      ASSERT_EQ(complete("l"), res("ine"));
      ASSERT_EQ(complete("li"), res("ne"));
      ASSERT_EQ(complete("lin"), res("e"));
      ASSERT_EQ(complete("line"), res(""));
      ASSERT_EQ(complete("m"), res("sh"));
      ASSERT_EQ(complete("ms"), res("h"));
      ASSERT_EQ(complete("msh"), res(""));
      ASSERT_EQ(complete("p"), res("ragma"));
      ASSERT_EQ(complete("pr"), res("agma"));
      ASSERT_EQ(complete("pra"), res("gma"));
      ASSERT_EQ(complete("prag"), res("ma"));
      ASSERT_EQ(complete("pragm"), res("a"));
      ASSERT_EQ(complete("pragma"), res(""));
      ASSERT_EQ(complete("u"), res("ndef"));
      ASSERT_EQ(complete("un"), res("def"));
      ASSERT_EQ(complete("und"), res("ef"));
      ASSERT_EQ(complete("unde"), res("f"));
      ASSERT_EQ(complete("undef"), res(""));
      ASSERT_EQ(complete("w"), res("arning"));
      ASSERT_EQ(complete("wa"), res("rning"));
      ASSERT_EQ(complete("war"), res("ning"));
      ASSERT_EQ(complete("warn"), res("ing"));
      ASSERT_EQ(complete("warni"), res("ng"));
      ASSERT_EQ(complete("warnin"), res("g"));
      ASSERT_EQ(complete("warning"), res(""));
    }
  }
}
