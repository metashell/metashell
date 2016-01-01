// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell_system_test/cpp_code.hpp>
#include <metashell_system_test/error.hpp>
#include <metashell_system_test/type.hpp>
#include <metashell_system_test/json_generator.hpp>
#include <metashell_system_test/run_metashell.hpp>

#include <just/test.hpp>

using namespace metashell_system_test;

JUST_TEST_CASE(test_echoing_definition)
{
  const auto r = run_metashell(
      {command("#msh preprocessed echo on"), command("void f() {}")});

  auto i = r.begin() + 3;

  JUST_ASSERT_EQUAL(cpp_code("void f() {}"), *i);
}

JUST_TEST_CASE(test_echoing_definition_with_macro)
{
  const auto r =
      run_metashell({command("#msh preprocessed echo on"),
                     command("#define FOO f"), command("void FOO() {}")});

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(cpp_code("void f() {}"), *i);
}

JUST_TEST_CASE(test_echoing_evaluation)
{
  const auto r = run_metashell({command("#msh preprocessed echo on"),
                                command("#define FOO int"), command("FOO")});

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(cpp_code("int"), *i);
  ++i;
  JUST_ASSERT(i != r.end());
  JUST_ASSERT_EQUAL(type("int"), *i);
}

JUST_TEST_CASE(test_echoing_include)
{
  const auto r = run_metashell(
      {command("#msh preprocessed echo on"), command("#include <string>")});

  auto i = r.begin() + 3;

  JUST_ASSERT_EQUAL(cpp_code(_), *i);
}

JUST_TEST_CASE(test_echoing_invalid_cpp_code)
{
  const auto r = run_metashell({command("#define FOO bar"),
                                command("#msh preprocessed echo on"),
                                command("void FOO() { return 13; }")});

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(cpp_code("void bar() { return 13; }"), *i);
  ++i;
  JUST_ASSERT(i != r.end());
  JUST_ASSERT_EQUAL(error(_), *i);
}

JUST_TEST_CASE(test_when_echoing_code_displaying_pp_errors_only_once)
{
  const auto r = run_metashell({command("#define FOO(x) bar"),
                                command("#msh preprocessed echo on"),
                                command("FOO(foo;")});

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(error(_), *i);
  ++i;
  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_echoing_invalid_cpp_code_during_evaluation)
{
  const auto r = run_metashell({command("#define FOO int"),
                                command("#msh preprocessed echo on"),
                                command("void FOO")});

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(cpp_code("void int"), *i);
  ++i;
  JUST_ASSERT(i != r.end());
  JUST_ASSERT_EQUAL(error(_), *i);
}

JUST_TEST_CASE(
    test_when_echoing_code_displaying_pp_errors_only_once_during_evaluation)
{
  const auto r =
      run_metashell({command("#define FOO(x) bar"),
                     command("#msh preprocessed echo on"), command("FOO(foo")});

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(error(_), *i);
  ++i;
  JUST_ASSERT(i == r.end());
}
