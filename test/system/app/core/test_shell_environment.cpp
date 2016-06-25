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

#include <metashell/system_test/comment.hpp>
#include <metashell/system_test/cpp_code.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/type.hpp>

#include <metashell/system_test/json_generator.hpp>
#include <metashell/system_test/path_builder.hpp>
#include <metashell/system_test/run_metashell.hpp>

#include <just/test.hpp>

using namespace metashell::system_test;

using pattern::_;

JUST_TEST_CASE(test_extending_environment_with_pragma_warns)
{
  const auto r = run_metashell(
      {command("#pragma metashell environment add typedef int x;")});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;

  JUST_ASSERT_EQUAL(
      comment({paragraph(
          "You don't need the environment add pragma to add this to the"
          " environment. The following command does this as well:")}),
      *i);
  ++i;

  JUST_ASSERT_EQUAL(cpp_code("typedef int x;"), *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_resetting_the_environment)
{
  const auto r = run_metashell({command("typedef int foo;"),
                                command("#pragma metashell environment reset"),
                                command("foo")});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(error(_), *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_resetting_the_environment_does_not_remove_built_in_macros)
{
  const std::string scalar_hpp = path_builder() / "metashell" / "scalar.hpp";

  const auto r = run_metashell({command("#pragma metashell environment reset"),
                                command("#include <" + scalar_hpp + ">"),
                                command("SCALAR(__METASHELL_MAJOR)")});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(type(_), *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_restoring_after_environment_reset_from_environment_stack)
{
  const auto r = run_metashell({command("typedef int foo;"),
                                command("#pragma metashell environment push"),
                                command("#pragma metashell environment reset"),
                                command("#pragma metashell environment pop"),
                                command("foo")});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;

  JUST_ASSERT_EQUAL(comment({paragraph("Environment stack has 1 entry")}), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;

  JUST_ASSERT_EQUAL(comment({paragraph("Environment stack is empty")}), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(type("int"), *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(
    test_environment_add_invalid_code_does_not_change_environment_and_displays_error)
{
  const cpp_code breaking("typedef nonexisting_type x;");

  const auto r = run_metashell(
      {command("#pragma metashell environment"),
       command("#pragma metashell environment add " + *breaking.code().value()),
       command("#pragma metashell environment")});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;

  const auto original_env = *i;
  ++i;

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(error(_), *i);
  ++i;
  JUST_ASSERT_EQUAL(comment(_), *i);
  ++i;
  JUST_ASSERT_EQUAL(breaking, *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(original_env, *i);
}
