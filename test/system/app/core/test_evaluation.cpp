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

#include <metashell_system_test/error.hpp>
#include <metashell_system_test/prompt.hpp>
#include <metashell_system_test/type.hpp>

#include <metashell_system_test/json_generator.hpp>
#include <metashell_system_test/path_builder.hpp>
#include <metashell_system_test/run_metashell.hpp>

#include <just/test.hpp>

using namespace metashell_system_test;

using pattern::_;

namespace
{
  void test_definition_and_query(const json_string& init_line_,
                                 const json_string& definition_,
                                 const json_string& query_,
                                 const type& expected_result_)
  {
    const auto r = run_metashell({init_line_, definition_, query_});

    auto i = r.begin();

    JUST_ASSERT_EQUAL(prompt(">"), *i);
    ++i;
    JUST_ASSERT_EQUAL(prompt(">"), *i);
    ++i;
    JUST_ASSERT_EQUAL(prompt(">"), *i);
    ++i;
    JUST_ASSERT_EQUAL(expected_result_, *i);
    ++i;

    JUST_ASSERT(i == r.end());
  }

  void test_definition_and_query(const json_string& definition_,
                                 const json_string& query_,
                                 const type& expected_result_)
  {
    test_definition_and_query(
        json_string(""), definition_, query_, expected_result_);
  }

  json_string generate_warning() { return command("#warning hello"); }
}

JUST_TEST_CASE(test_evaluating_int)
{
  const auto r = run_metashell_command("int");
  JUST_ASSERT_EQUAL(type("int"), r);
}

JUST_TEST_CASE(test_non_existing_class)
{
  const auto r = run_metashell({command("hello")});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(error(_), *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_macro_in_config)
{
  const auto r = run_metashell({command("FOO")}, {"--", "-DFOO=int"});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(type("int"), *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_typedef_in_the_middle_of_a_line)
{
  test_definition_and_query(
      command("bool typedef * x;"), command("x"), type("bool *"));
  test_definition_and_query(
      command("char typedef * x;"), command("x"), type("char *"));
  test_definition_and_query(
      command("const typedef int x;"), command("x"), type("const int"));
  test_definition_and_query(
      command("double typedef * x;"), command("x"), type("double *"));
  test_definition_and_query(
      command("float typedef * x;"), command("x"), type("float *"));
  test_definition_and_query(
      command("int typedef * x;"), command("x"), type("int *"));
  test_definition_and_query(
      command("long typedef int x;"), command("x"), type("long"));
  test_definition_and_query(
      command("short typedef int x;"), command("x"), type("short"));
  test_definition_and_query(
      command("signed typedef int x;"), command("x"), type("int"));
  test_definition_and_query(
      command("unsigned typedef int x;"), command("x"), type("unsigned int"));
  test_definition_and_query(
      command("void typedef * x;"), command("x"), type("void *"));
  test_definition_and_query(
      command("volatile typedef int x;"), command("x"), type("volatile int"));
  test_definition_and_query(
      command("wchar_t typedef * x;"), command("x"), type("wchar_t *"));
}

JUST_TEST_CASE(test_defining_constexpr_function)
{
  const std::string scalar_hpp = path_builder() / "metashell" / "scalar.hpp";

  const auto r = run_metashell({command("#include <" + scalar_hpp + ">"),
                                command("constexpr int f() { return 13; }"),
                                command("SCALAR(f())")});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT(type("std::integral_constant<int, 13>") == *i ||
              type("std::__1::integral_constant<int, 13>") == *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_typedef_in_the_middle_of_a_line_starting_with_an_identifier)
{
  const auto r = run_metashell(
      {command("struct y;"), command("y typedef * x;"), command("x")});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(type("y *"), *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_warnings)
{
  const auto r = run_metashell({generate_warning()});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(error(_), *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_disabled_warnings)
{
  const auto r = run_metashell({generate_warning()}, {"--", "-w"});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_multiline_input)
{
  const auto r = run_metashell({command("const \\"), command("int")});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("...>"), *i);
  ++i;
  JUST_ASSERT_EQUAL(type("const int"), *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_three_line_input)
{
  const auto r =
      run_metashell({command("const \\"), command("int \\"), command("*")});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("...>"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("...>"), *i);
  ++i;
  JUST_ASSERT_EQUAL(type("const int *"), *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_dealing_with_crashing_clang)
{
  const auto r = run_metashell(
      {command("template <class = void> struct foo { ~foo() {} };"),
       // This test assumes, that Clang breaks for the following command:
       command("decltype(&foo<>::~foo)")});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(error(_), *i);
  ++i;

  JUST_ASSERT(i == r.end());
}
