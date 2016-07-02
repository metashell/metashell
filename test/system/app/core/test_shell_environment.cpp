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

#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/path_builder.hpp>

#include <just/test.hpp>

using namespace metashell::system_test;

using pattern::_;

JUST_TEST_CASE(test_extending_environment_with_pragma_warns)
{
  metashell_instance mi;

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(comment({paragraph(
           "You don't need the environment add pragma to add this to the"
           " environment. The following command does this as well:")})),
       to_json_string(cpp_code("typedef int x;")), to_json_string(prompt(">"))},
      mi.command("#pragma metashell environment add typedef int x;"));
}

JUST_TEST_CASE(test_resetting_the_environment)
{
  metashell_instance mi;
  mi.command("typedef int foo;");
  mi.command("#pragma metashell environment reset");

  JUST_ASSERT_EQUAL(error(_), mi.command("foo").front());
}

JUST_TEST_CASE(test_resetting_the_environment_does_not_remove_built_in_macros)
{
  const std::string scalar_hpp = path_builder() / "metashell" / "scalar.hpp";

  metashell_instance mi;
  mi.command("#pragma metashell environment reset");
  mi.command("#include <" + scalar_hpp + ">");

  JUST_ASSERT_EQUAL(type(_), mi.command("SCALAR(__METASHELL_MAJOR)").front());
}

JUST_TEST_CASE(test_restoring_after_environment_reset_from_environment_stack)
{
  metashell_instance mi;
  mi.command("typedef int foo;");

  JUST_ASSERT_EQUAL(comment({paragraph("Environment stack has 1 entry")}),
                    mi.command("#pragma metashell environment push").front());

  mi.command("#pragma metashell environment reset");

  JUST_ASSERT_EQUAL(comment({paragraph("Environment stack is empty")}),
                    mi.command("#pragma metashell environment pop").front());

  JUST_ASSERT_EQUAL(type("int"), mi.command("foo").front());
}

JUST_TEST_CASE(
    test_environment_add_invalid_code_does_not_change_environment_and_displays_error)
{
  const cpp_code breaking("typedef nonexisting_type x;");

  metashell_instance mi;

  const std::vector<json_string> original_env =
      mi.command("#pragma metashell environment");

  const std::vector<json_string> br = mi.command(
      "#pragma metashell environment add " + *breaking.code().value());
  JUST_ASSERT_EQUAL(error(_), br[0]);
  JUST_ASSERT_EQUAL(comment(_), br[1]);
  JUST_ASSERT_EQUAL(breaking, br[2]);

  JUST_ASSERT_EQUAL_CONTAINER(
      original_env, mi.command("#pragma metashell environment"));
}
