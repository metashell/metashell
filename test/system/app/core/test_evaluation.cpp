// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/error.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/type.hpp>

#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/path_builder.hpp>

#include <just/test.hpp>

using namespace metashell::system_test;

using pattern::_;

namespace
{
  void test_definition_and_query(const std::string& definition_,
                                 const std::string& query_,
                                 const type& expected_result_)
  {
    metashell_instance mi;

    JUST_ASSERT_EQUAL_CONTAINER(
        {to_json_string(prompt(">"))}, mi.command(definition_));
    JUST_ASSERT_EQUAL(expected_result_, mi.command(query_).front());
  }

  std::string generate_warning() { return "#warning hello"; }
}

JUST_TEST_CASE(test_evaluating_int)
{
  JUST_ASSERT_EQUAL(type("int"), metashell_instance().command("int").front());
}

JUST_TEST_CASE(test_non_existing_class)
{
  metashell_instance mi;

  JUST_ASSERT_EQUAL(error(_), mi.command("hello").front());
}

JUST_TEST_CASE(test_macro_in_config)
{
  metashell_instance mi({"--", "-DFOO=int"});

  JUST_ASSERT_EQUAL(type("int"), mi.command("FOO").front());
}

JUST_TEST_CASE(test_typedef_in_the_middle_of_a_line)
{
  test_definition_and_query("bool typedef * x;", "x", type("bool *"));
  test_definition_and_query("char typedef * x;", "x", type("char *"));
  test_definition_and_query("const typedef int x;", "x", type("const int"));
  test_definition_and_query("double typedef * x;", "x", type("double *"));
  test_definition_and_query("float typedef * x;", "x", type("float *"));
  test_definition_and_query("int typedef * x;", "x", type("int *"));
  test_definition_and_query("long typedef int x;", "x", type("long"));
  test_definition_and_query("short typedef int x;", "x", type("short"));
  test_definition_and_query("signed typedef int x;", "x", type("int"));
  test_definition_and_query(
      "unsigned typedef int x;", "x", type("unsigned int"));
  test_definition_and_query("void typedef * x;", "x", type("void *"));
  test_definition_and_query(
      "volatile typedef int x;", "x", type("volatile int"));
  test_definition_and_query("wchar_t typedef * x;", "x", type("wchar_t *"));
}

JUST_TEST_CASE(test_defining_constexpr_function)
{
  const std::string scalar_hpp = path_builder() / "metashell" / "scalar.hpp";

  metashell_instance mi;
  mi.command("#include <" + scalar_hpp + ">");
  mi.command("constexpr int f() { return 13; }");

  const json_string ic = mi.command("SCALAR(f())").front();
  JUST_ASSERT(type("std::integral_constant<int, 13>") == ic ||
              type("std::__1::integral_constant<int, 13>") == ic);
}

JUST_TEST_CASE(test_typedef_in_the_middle_of_a_line_starting_with_an_identifier)
{
  metashell_instance mi;
  mi.command("struct y;");
  mi.command("y typedef * x;");

  JUST_ASSERT_EQUAL(type("y *"), mi.command("x").front());
}

JUST_TEST_CASE(test_warnings)
{
  metashell_instance mi;

  JUST_ASSERT_EQUAL(error(_), mi.command(generate_warning()).front());
}

JUST_TEST_CASE(test_disabled_warnings)
{
  metashell_instance mi({"--", "-w"});

  JUST_ASSERT_EQUAL(prompt(">"), mi.command(generate_warning()).front());
}

JUST_TEST_CASE(test_multiline_input)
{
  metashell_instance mi;

  JUST_ASSERT_EQUAL(prompt("...>"), mi.command("const \\").front());
  JUST_ASSERT_EQUAL(type("const int"), mi.command("int").front());
}

JUST_TEST_CASE(test_three_line_input)
{
  metashell_instance mi;

  JUST_ASSERT_EQUAL(prompt("...>"), mi.command("const \\").front());
  JUST_ASSERT_EQUAL(prompt("...>"), mi.command("int \\").front());
  JUST_ASSERT_EQUAL(type("const int *"), mi.command("*").front());
}

JUST_TEST_CASE(test_dealing_with_crashing_clang)
{
  const std::string command_breaking_clang = "decltype(&foo<>::~foo)";

  metashell_instance mi;
  mi.command("template <class = void> struct foo { ~foo() {} };");

  JUST_ASSERT_EQUAL(error(_), mi.command(command_breaking_clang).front());
}
