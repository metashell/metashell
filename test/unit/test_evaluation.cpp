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

#include "breaking_environment.hpp"
#include "mock_environment_detector.hpp"
#include "test_config.hpp"
#include "util.hpp"

#include <metashell/engine_clang.hpp>
#include <metashell/engine_constant.hpp>
#include <metashell/in_memory_displayer.hpp>
#include <metashell/in_memory_history.hpp>
#include <metashell/metashell.hpp>
#include <metashell/null_displayer.hpp>
#include <metashell/null_history.hpp>
#include <metashell/shell.hpp>

#include "empty_container.hpp"

#include <gtest/gtest.h>

using namespace metashell;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;

TEST(evaluation, accept_empty_input)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());
  sh.line_available("", d);

  ASSERT_EQ(empty_container, d.types());
  ASSERT_EQ(empty_container, d.errors());
}

TEST(evaluation, accept_space_input)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());
  sh.line_available(" ", d);

  ASSERT_EQ(empty_container, d.types());
  ASSERT_EQ(empty_container, d.errors());
}

TEST(evaluation, accept_tab_input)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());
  sh.line_available("\t", d);

  ASSERT_EQ(empty_container, d.types());
  ASSERT_EQ(empty_container, d.errors());
}

TEST(evaluation, accept_vertical_tab_input)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());
  sh.line_available("\v", d);

  ASSERT_EQ(empty_container, d.types());
  ASSERT_EQ(empty_container, d.errors());
}

TEST(evaluation, accept_new_line_input)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());
  sh.line_available("\n", d);

  ASSERT_EQ(empty_container, d.types());
  ASSERT_EQ(empty_container, d.errors());
}

TEST(evaluation, accept_carrige_return_input)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());
  sh.line_available("\r", d);

  ASSERT_EQ(empty_container, d.types());
  ASSERT_EQ(empty_container, d.errors());
}

TEST(evaluation, accept_two_space_input)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());
  sh.line_available("  ", d);

  ASSERT_EQ(empty_container, d.types());
  ASSERT_EQ(empty_container, d.errors());
}

TEST(evaluation, history_is_stored)
{
  null_displayer d;
  in_memory_history h;
  shell sh(test_config(), "", "", "", create_failing_engine());

  sh.line_available("int", d, h);

  ASSERT_EQ(std::vector<std::string>{"int"}, h.commands());
}

TEST(evaluation, empty_line_is_not_stored_in_history)
{
  null_displayer d;
  in_memory_history h;
  shell sh(test_config(), "", "", "", create_failing_engine());

  sh.line_available("", d, h);

  ASSERT_EQ(empty_container, h.commands());
}

TEST(evaluation, line_containing_just_whitespace_is_not_stored_in_history)
{
  null_displayer d;
  in_memory_history h;
  shell sh(test_config(), "", "", "", create_failing_engine());

  sh.line_available(" ", d, h);

  ASSERT_EQ(empty_container, h.commands());
}

TEST(evaluation,
     the_same_thing_following_each_other_is_not_added_to_history_twice)
{
  null_displayer d;
  in_memory_history h;
  shell sh(test_config(), "", "", "", create_failing_engine());

  sh.line_available("int", d, h);
  sh.line_available("int", d, h);

  ASSERT_EQ(std::vector<std::string>{"int"}, h.commands());
}

TEST(evaluation, accept_c_comment_input)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());
  sh.line_available("/* some comment */", d);

  ASSERT_EQ(empty_container, d.types());
  ASSERT_EQ(empty_container, d.errors());
}

TEST(evaluation, accept_cpp_comment_input)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());
  sh.line_available("// some comment", d);

  ASSERT_EQ(empty_container, d.types());
  ASSERT_EQ(empty_container, d.errors());
}

TEST(evaluation, comment_is_stored_in_history)
{
  null_displayer d;
  in_memory_history h;
  shell sh(test_config(), "", "", "", create_failing_engine());

  sh.line_available("// some comment", d, h);

  ASSERT_EQ(1u, h.commands().size());
}

TEST(evaluation, throwing_environment_update_not_breaking_shell)
{
  data::config cfg;
  cfg.push_back(data::shell_config());

  breaking_environment* e = new breaking_environment();
  in_memory_displayer d;
  null_history h;
  command_processor_queue cpq;
  cpq.history(h);
  shell sh(cfg, std::unique_ptr<breaking_environment>(e), cpq, "", "", "",
           create_failing_engine());
  e->append_throw_from_now();

  sh.store_in_buffer(data::cpp_code("typedef int foo;"), d);

  ASSERT_FALSE(d.errors().empty());
}

TEST(evaluation, throwing_environment_not_breaking_validate)
{
  data::config cfg;
  cfg.push_back(data::shell_config());

  NiceMock<mock_environment_detector> det;
  breaking_environment e;
  e.get_appended_throw_from_now();
  null_displayer d;

  ON_CALL(det, on_windows()).WillByDefault(Return(false));
  ON_CALL(det, on_osx()).WillByDefault(Return(false));
  ON_CALL(det, directory_of_executable()).WillByDefault(Return(""));
  ON_CALL(det, file_exists(_)).WillByDefault(Return(true));

  const data::result r =
      get_internal_templight_entry()
          .build(cfg, "", "", "env.hpp", det, d, nullptr)
          ->cpp_validator()
          .validate_code(data::cpp_code("typedef int foo;"), cfg, e, false);

  ASSERT_FALSE(r.successful);
  ASSERT_FALSE(r.error.empty());
}

TEST(evaluation, variable_definition)
{
  using data::command;
  ASSERT_TRUE(is_environment_setup_command(command(data::cpp_code("int x;"))));
}

TEST(evaluation, function_definition)
{
  using data::command;
  ASSERT_TRUE(
      is_environment_setup_command(command(data::cpp_code("void f() {}"))));
}

TEST(evaluation, is_environment_setup_with_leading_whitespace)
{
  using data::command;
  ASSERT_FALSE(is_environment_setup_command(command(data::cpp_code(" int"))));
}

TEST(evaluation, is_environment_setup_without_leading_whitespace)
{
  using data::command;
  ASSERT_FALSE(is_environment_setup_command(command(data::cpp_code("int"))));
}

TEST(evaluation, prompt_is_different_in_multiline_input)
{
  null_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());
  sh.line_available("const \\", d);

  ASSERT_EQ("...>", sh.prompt());
}

TEST(evaluation, command_macro_usage_with_semicolon_is_environment_setup)
{
  using data::command;

  ASSERT_TRUE(
      is_environment_setup_command(command(data::cpp_code("SOME_MACRO(13);"))));
}
