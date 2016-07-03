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

#include <metashell/system_test/cpp_code.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/type.hpp>

#include <just/test.hpp>

using namespace metashell::system_test;

using pattern::_;

JUST_TEST_CASE(test_echoing_definition)
{
  metashell_instance mi;
  mi.command("#msh preprocessed echo on");

  JUST_ASSERT_EQUAL(cpp_code("void f() {}"), mi.command("void f() {}").front());
}

JUST_TEST_CASE(test_echoing_definition_with_macro)
{
  metashell_instance mi;
  mi.command("#msh preprocessed echo on");
  mi.command("#define FOO f");

  JUST_ASSERT_EQUAL(
      cpp_code("void f() {}"), mi.command("void FOO() {}").front());
}

JUST_TEST_CASE(test_echoing_evaluation)
{
  metashell_instance mi;
  mi.command("#msh preprocessed echo on");
  mi.command("#define FOO int");

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(cpp_code("int")), to_json_string(type("int")),
       to_json_string(prompt(">"))},
      mi.command("FOO"));
}

JUST_TEST_CASE(test_echoing_include)
{
  metashell_instance mi;
  mi.command("#msh preprocessed echo on");

  JUST_ASSERT_EQUAL(cpp_code(_), mi.command("#include <string>").front());
}

JUST_TEST_CASE(test_echoing_invalid_cpp_code)
{
  metashell_instance mi;
  mi.command("#define FOO bar");
  mi.command("#msh preprocessed echo on");

  const std::vector<json_string> r = mi.command("void FOO() { return 13; }");
  JUST_ASSERT_EQUAL(cpp_code("void bar() { return 13; }"), r[0]);
  JUST_ASSERT_EQUAL(error(_), r[1]);
}

JUST_TEST_CASE(test_when_echoing_code_displaying_pp_errors_only_once)
{
  metashell_instance mi;
  mi.command("#define FOO(x) bar");
  mi.command("#msh preprocessed echo on");

  JUST_ASSERT_EQUAL(error(_), mi.command("FOO(foo;").front());
}

JUST_TEST_CASE(test_echoing_invalid_cpp_code_during_evaluation)
{
  metashell_instance mi;
  mi.command("#define FOO int");
  mi.command("#msh preprocessed echo on");

  const std::vector<json_string> r = mi.command("void FOO");
  JUST_ASSERT_EQUAL(cpp_code("void int"), r[0]);
  JUST_ASSERT_EQUAL(error(_), r[1]);
}

JUST_TEST_CASE(
    test_when_echoing_code_displaying_pp_errors_only_once_during_evaluation)
{
  metashell_instance mi;
  mi.command("#define FOO(x) bar");
  mi.command("#msh preprocessed echo on");

  JUST_ASSERT_EQUAL(error(_), mi.command("FOO(foo").front());
}
