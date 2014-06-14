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

#include <just/test.hpp>

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

