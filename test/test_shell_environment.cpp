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

#include "test_shell.hpp"

#include <metashell/path_builder.hpp>

#include <just/test.hpp>

#include <string>
#include <cassert>

namespace
{
  std::string appended_since(const std::string& old_, const std::string& new_)
  {
    assert(new_.size() >= old_.size());
    assert(new_.substr(0, old_.size()) == old_);
    return new_.substr(old_.size());
  }
}

JUST_TEST_CASE(test_popping_environment_from_empty_queue)
{
  test_shell sh;

  JUST_ASSERT_THROWS_SOMETHING(sh.pop_environment());
}

JUST_TEST_CASE(test_env_pop_reverts_changes_since_push)
{
  test_shell sh;

  sh.push_environment();
  const std::string old_env = sh.env().get_all();
  sh.store_in_buffer("typedef int x;");
  sh.pop_environment();

  JUST_ASSERT_EQUAL(old_env, sh.env().get_all());
}

JUST_TEST_CASE(test_more_pops_than_pushes_throws)
{
  test_shell sh;

  sh.push_environment();
  sh.pop_environment();
  JUST_ASSERT_THROWS_SOMETHING(sh.pop_environment());
}

JUST_TEST_CASE(test_env_two_level_environment_stack)
{
  test_shell sh;

  sh.push_environment();
  const std::string old_env = sh.env().get_all();

  sh.store_in_buffer("typedef int x;");
  sh.push_environment();
  sh.store_in_buffer("typedef int y;");

  sh.pop_environment();
  sh.pop_environment();

  JUST_ASSERT_EQUAL(old_env, sh.env().get_all());
}

JUST_TEST_CASE(test_displaying_the_size_of_the_empty_environment_stack)
{
  test_shell sh;
  sh.display_environment_stack_size();

  JUST_ASSERT_EQUAL("// Environment stack is empty\n", sh.output());
}

JUST_TEST_CASE(test_displaying_the_size_of_one_element_stack)
{
  test_shell sh;
  sh.push_environment();
  sh.display_environment_stack_size();

  JUST_ASSERT_EQUAL("// Environment stack has 1 entry\n", sh.output());
}

JUST_TEST_CASE(test_displaying_the_size_of_two_element_stack)
{
  test_shell sh;
  sh.push_environment();
  sh.push_environment();
  sh.display_environment_stack_size();

  JUST_ASSERT_EQUAL("// Environment stack has 2 entries\n", sh.output());
}

JUST_TEST_CASE(test_appended_since_when_nothing_appended)
{
  JUST_ASSERT_EQUAL("", appended_since("", ""));
}

JUST_TEST_CASE(test_appended_since_when_something_appended)
{
  JUST_ASSERT_EQUAL(" world", appended_since("hello", "hello world"));
}

JUST_TEST_CASE(test_extending_environment_with_pragma)
{
  test_shell sh;
  const std::string original_env = sh.env().get_all();

  sh.line_available("#pragma metashell environment add typedef int x;");
  sh.line_available("#pragma metashell environment");

  JUST_ASSERT_EQUAL(
    "\ntypedef int x;",
    appended_since(original_env, sh.env().get_all())
  );
}

JUST_TEST_CASE(test_environment_add_invalid_code_does_not_change_environment)
{
  test_shell sh;
  const std::string original_env = sh.env().get_all();

  sh.line_available(
    "#pragma metashell environment add typedef nonexisting_type x;"
  );
  sh.line_available("#pragma metashell environment");

  JUST_ASSERT_EQUAL(original_env, sh.env().get_all());
}

JUST_TEST_CASE(test_environment_add_invalid_code_displays_error)
{
  test_shell sh;
  sh.line_available(
    "#pragma metashell environment add typedef nonexisting_type x;"
  );

  JUST_ASSERT(!sh.error().empty());
}

JUST_TEST_CASE(test_extending_environment_with_pragma_warns)
{
  test_shell sh(metashell::empty_config(), 1024);
  sh.line_available("#pragma metashell environment add typedef int x;");

  JUST_ASSERT_EQUAL(
    "// You don't need the environment add pragma to add this to the"
    " environment. The following command does this as well:\n"
    "typedef int x;\n",
    sh.output()
  );
}

JUST_TEST_CASE(test_resetting_the_environment)
{
  test_shell sh;
  sh.line_available("typedef int foo;");
  sh.line_available("#pragma metashell environment reset");
  sh.line_available("foo");

  JUST_ASSERT(!sh.error().empty());
}

JUST_TEST_CASE(test_resetting_the_environment_does_not_remove_built_in_macros)
{
  const std::string scalar_hpp =
    metashell::path_builder() / "metashell" / "scalar.hpp";

  test_shell sh;
  sh.line_available("#pragma metashell environment reset");
  sh.line_available("#include <" + scalar_hpp + ">");
  sh.line_available("SCALAR(__METASHELL_MAJOR)");

  JUST_ASSERT_EQUAL("", sh.error());
}

JUST_TEST_CASE(test_restoring_after_environment_reset_from_environment_stack)
{
  test_shell sh;
  sh.line_available("typedef int foo;");
  sh.line_available("#pragma metashell environment push");
  sh.line_available("#pragma metashell environment reset");
  sh.line_available("#pragma metashell environment pop");
  sh.line_available("foo");

  JUST_ASSERT(sh.error().empty());
}

