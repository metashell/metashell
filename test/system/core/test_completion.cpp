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
  auto res = code_completion_result::create;

  std::vector<std::string> cpp_code_prefixes()
  {
    return {"",
            "#msh evaluate ",
            "#msh environment add ",
            "#msh pp ",
            "#msh included headers ",
            "#msh mdb ",
            "#msh mdb -nocache ",
            "#msh mdb -profile ",
            "#msh mdb -full ",
            "#msh mdb -full -nocache ",
            "#msh mdb -nocache -full ",
            "#msh mdb -nocache -profile ",
            "#msh mdb -profile -nocache ",
            "#msh pdb ",
            "#msh pdb -nocache ",
            "#msh pdb -profile ",
            "#msh pdb -nocache -profile ",
            "#msh pdb -profile -nocache "};
  }
}

TEST(completion, simple)
{
  code_completer c;

  for (const std::string& prefix : cpp_code_prefixes())
  {
    ASSERT_EQ(c(prefix + "no_completion_for_this"), res());

    ASSERT_EQ(c(prefix + "doubl"), res("e"));
    ASSERT_EQ(c(prefix + "in"), res("line", "t"));
    ASSERT_EQ(c("decltype(\"in"), res());
    ASSERT_EQ(c("int"), res());
  }
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

  for (const std::string& prefix : cpp_code_prefixes())
  {
    ASSERT_EQ(c(prefix + "decltype(foo()."), members_of_foo);
    ASSERT_EQ(c(prefix + "decltype(((foo*)0)->"), members_of_foo);

    ASSERT_EQ(c(prefix + "foo::"), res("type", "bar") + members_of_foo);

    ASSERT_EQ(c(prefix + "decltype(foo().m"), res("em1", "em2"));

    ASSERT_EQ(c(prefix + "decltype(((foo*)0)->m"), res("em1", "em2"));
  }
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

  for (const std::string& prefix : cpp_code_prefixes())
  {
    ASSERT_EQ(c(prefix + "decltype(foo<int>()."), members_of_foo);
    ASSERT_EQ(c(prefix + "decltype(((foo<int>*)0)->"), members_of_foo);
    ASSERT_EQ(c(prefix + "foo<int>::"), res("type", "bar") + members_of_foo);

    ASSERT_EQ(c(prefix + "decltype(foo<int>().m"), res("em1", "em2"));
    ASSERT_EQ(c(prefix + "decltype(((foo<int>*)0)->m"), res("em1", "em2"));
  }
}

TEST(completion, included)
{
  code_completer c("#include <vector>");

  for (const std::string& prefix : cpp_code_prefixes())
  {
    const auto result = c(prefix + "std::vecto");
    if (result != res("r", "r<_Ty,"))
    {
      ASSERT_EQ(result, res("r", "r<_Tp,"));
    }
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
  for (const std::string& cpp_prefix : cpp_code_prefixes())
  {
    const std::string spaces[] = {"", "  "};

    for (const auto& prefix : spaces)
    {
      for (const auto& middle : spaces)
      {
        const auto complete = [&cpp_prefix, &prefix, &middle,
                               &c](const std::string& s_) {
          return c(cpp_prefix + prefix + "#" + middle + s_);
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
        if (cpp_prefix.empty())
        {
          ASSERT_EQ(complete("m"), res("sh"));
          ASSERT_EQ(complete("ms"), res("h"));
          ASSERT_EQ(complete("msh"), res(""));
        }
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
}

TEST(completion, msh)
{
  code_completer c;

  for (const std::string prefix :
       {"#msh ", "  #msh ", " # pragma metashell ", "#msh help "})
  {
    ASSERT_EQ(c(prefix + "all"), res(" config"));
    ASSERT_EQ(c(prefix + "all "), res("config"));
    ASSERT_EQ(c(prefix + "env p "), res());
    ASSERT_EQ(c(prefix + "en"),
              res("gine show", "gine switch", "vironment ", "vironment add",
                  "vironment pop", "vironment push", "vironment reload",
                  "vironment reset", "vironment save", "vironment stack"));
  }

  ASSERT_EQ(c("#pragma m"), res("etashell"));
}

TEST(completion, pragma_without_argument)
{
  code_completer c;

  for (const std::string& pragma :
       {"all config", "engine show", "environment pop", "environment push",
        "environment reload", "environment reset", "environment stack",
        "macro names", "macros", "metaprogram mode", "preprocessor mode",
        "quit", "quoteincludes", "sysincludes"})
  {
    ASSERT_EQ(c("#msh " + pragma), res(""));
    ASSERT_EQ(c("#msh " + pragma + " "), res());
  }
}
