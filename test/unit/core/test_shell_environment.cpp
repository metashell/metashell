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

#include "test_config.hpp"

#include <metashell/core/engine_constant.hpp>
#include <metashell/core/in_memory_displayer.hpp>
#include <metashell/core/shell.hpp>

#include <gtest/gtest.h>

#include <cassert>
#include <string>

namespace
{
  std::vector<metashell::data::text> text(const std::string& text_)
  {
    return std::vector<metashell::data::text>{metashell::data::text(text_)};
  }
}

TEST(shell_environment, popping_environment_from_empty_queue)
{
  metashell::core::shell sh(metashell::test_config(), "", "",
                            metashell::core::create_failing_engine());

  ASSERT_ANY_THROW(sh.pop_environment());
}

TEST(shell_environment, env_pop_reverts_changes_since_push)
{
  metashell::core::in_memory_displayer d;
  metashell::core::shell sh(metashell::test_config(), "", "",
                            metashell::core::create_failing_engine());

  sh.push_environment();
  const metashell::data::cpp_code old_env = sh.env().get_all();
  sh.store_in_buffer(metashell::data::cpp_code("typedef int x;"), d);
  sh.pop_environment();

  ASSERT_EQ(old_env, sh.env().get_all());
}

TEST(shell_environment, more_pops_than_pushes_throws)
{
  metashell::core::shell sh(metashell::test_config(), "", "",
                            metashell::core::create_failing_engine());

  sh.push_environment();
  sh.pop_environment();

  ASSERT_ANY_THROW(sh.pop_environment());
}

TEST(shell_environment, env_two_level_environment_stack)
{
  metashell::core::in_memory_displayer d;
  metashell::core::shell sh(metashell::test_config(), "", "",
                            metashell::core::create_failing_engine());

  sh.push_environment();
  const metashell::data::cpp_code old_env = sh.env().get_all();

  sh.store_in_buffer(metashell::data::cpp_code("typedef int x;"), d);
  sh.push_environment();
  sh.store_in_buffer(metashell::data::cpp_code("typedef int y;"), d);

  sh.pop_environment();
  sh.pop_environment();

  ASSERT_EQ(old_env, sh.env().get_all());
}

TEST(shell_environment, displaying_the_size_of_the_empty_environment_stack)
{
  metashell::core::in_memory_displayer d;
  metashell::core::shell sh(metashell::test_config(), "", "",
                            metashell::core::create_failing_engine());
  sh.display_environment_stack_size(d);

  ASSERT_EQ(text("Environment stack is empty"), d.comments());
}

TEST(shell_environment, displaying_the_size_of_one_element_stack)
{
  metashell::core::in_memory_displayer d;
  metashell::core::shell sh(metashell::test_config(), "", "",
                            metashell::core::create_failing_engine());
  sh.push_environment();
  sh.display_environment_stack_size(d);

  ASSERT_EQ(text("Environment stack has 1 entry"), d.comments());
}

TEST(shell_environment, displaying_the_size_of_two_element_stack)
{
  metashell::core::in_memory_displayer d;
  metashell::core::shell sh(metashell::test_config(), "", "",
                            metashell::core::create_failing_engine());
  sh.push_environment();
  sh.push_environment();
  sh.display_environment_stack_size(d);

  ASSERT_EQ(text("Environment stack has 2 entries"), d.comments());
}
