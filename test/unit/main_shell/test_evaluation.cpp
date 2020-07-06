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

#include <metashell/mock/environment.hpp>
#include <metashell/mock/environment_detector.hpp>

#include <metashell/data/empty_container.hpp>

#include "test_config.hpp"

#include <metashell/main_shell/shell.hpp>

#include <metashell/engine/constant/builder.hpp>

#include <metashell/core/command.hpp>
#include <metashell/core/in_memory_displayer.hpp>
#include <metashell/core/in_memory_history.hpp>
#include <metashell/core/is_environment_setup_command.hpp>
#include <metashell/core/null_displayer.hpp>

#include <gtest/gtest.h>

using namespace metashell;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Throw;
using ::testing::_;

TEST(evaluation, accept_empty_input)
{
  core::in_memory_displayer d;
  main_shell::shell sh(
      test_config(), "", "", engine::constant::create_failing());
  sh.line_available(data::user_input(), d);

  ASSERT_EQ(data::empty_container(), d.types());
  ASSERT_EQ(data::empty_container(), d.errors());
}

TEST(evaluation, accept_space_input)
{
  core::in_memory_displayer d;
  main_shell::shell sh(
      test_config(), "", "", engine::constant::create_failing());
  sh.line_available(data::user_input(" "), d);

  ASSERT_EQ(data::empty_container(), d.types());
  ASSERT_EQ(data::empty_container(), d.errors());
}

TEST(evaluation, accept_tab_input)
{
  core::in_memory_displayer d;
  main_shell::shell sh(
      test_config(), "", "", engine::constant::create_failing());
  sh.line_available(data::user_input("\t"), d);

  ASSERT_EQ(data::empty_container(), d.types());
  ASSERT_EQ(data::empty_container(), d.errors());
}

TEST(evaluation, accept_vertical_tab_input)
{
  core::in_memory_displayer d;
  main_shell::shell sh(
      test_config(), "", "", engine::constant::create_failing());
  sh.line_available(data::user_input("\v"), d);

  ASSERT_EQ(data::empty_container(), d.types());
  ASSERT_EQ(data::empty_container(), d.errors());
}

TEST(evaluation, accept_new_line_input)
{
  core::in_memory_displayer d;
  main_shell::shell sh(
      test_config(), "", "", engine::constant::create_failing());
  sh.line_available(data::user_input("\n"), d);

  ASSERT_EQ(data::empty_container(), d.types());
  ASSERT_EQ(data::empty_container(), d.errors());
}

TEST(evaluation, accept_carrige_return_input)
{
  core::in_memory_displayer d;
  main_shell::shell sh(
      test_config(), "", "", engine::constant::create_failing());
  sh.line_available(data::user_input("\r"), d);

  ASSERT_EQ(data::empty_container(), d.types());
  ASSERT_EQ(data::empty_container(), d.errors());
}

TEST(evaluation, accept_two_space_input)
{
  core::in_memory_displayer d;
  main_shell::shell sh(
      test_config(), "", "", engine::constant::create_failing());
  sh.line_available(data::user_input("  "), d);

  ASSERT_EQ(data::empty_container(), d.types());
  ASSERT_EQ(data::empty_container(), d.errors());
}

TEST(evaluation, history_is_stored)
{
  core::null_displayer d;
  core::in_memory_history h;
  main_shell::shell sh(
      test_config(), "", "", engine::constant::create_failing());

  sh.line_available(data::user_input("int"), d, h);

  ASSERT_EQ(
      std::vector<data::user_input>{data::user_input("int")}, h.commands());
}

TEST(evaluation, empty_line_is_not_stored_in_history)
{
  core::null_displayer d;
  core::in_memory_history h;
  main_shell::shell sh(
      test_config(), "", "", engine::constant::create_failing());

  sh.line_available(data::user_input(), d, h);

  ASSERT_EQ(data::empty_container(), h.commands());
}

TEST(evaluation, line_containing_just_whitespace_is_not_stored_in_history)
{
  core::null_displayer d;
  core::in_memory_history h;
  main_shell::shell sh(
      test_config(), "", "", engine::constant::create_failing());

  sh.line_available(data::user_input(" "), d, h);

  ASSERT_EQ(data::empty_container(), h.commands());
}

TEST(evaluation,
     the_same_thing_following_each_other_is_not_added_to_history_twice)
{
  core::null_displayer d;
  core::in_memory_history h;
  main_shell::shell sh(
      test_config(), "", "", engine::constant::create_failing());

  sh.line_available(data::user_input("int"), d, h);
  sh.line_available(data::user_input("int"), d, h);

  ASSERT_EQ(
      std::vector<data::user_input>{data::user_input("int")}, h.commands());
}

TEST(evaluation, accept_c_comment_input)
{
  core::in_memory_displayer d;
  main_shell::shell sh(
      test_config(), "", "", engine::constant::create_failing());
  sh.line_available(data::user_input("/* some comment */"), d);

  ASSERT_EQ(data::empty_container(), d.types());
  ASSERT_EQ(data::empty_container(), d.errors());
}

TEST(evaluation, accept_cpp_comment_input)
{
  core::in_memory_displayer d;
  main_shell::shell sh(
      test_config(), "", "", engine::constant::create_failing());
  sh.line_available(data::user_input("// some comment"), d);

  ASSERT_EQ(data::empty_container(), d.types());
  ASSERT_EQ(data::empty_container(), d.errors());
}

TEST(evaluation, comment_is_stored_in_history)
{
  core::null_displayer d;
  core::in_memory_history h;
  main_shell::shell sh(
      test_config(), "", "", engine::constant::create_failing());

  sh.line_available(data::user_input("// some comment"), d, h);

  ASSERT_EQ(1u, h.commands().size());
}

TEST(evaluation, throwing_environment_update_not_breaking_shell)
{
  data::config cfg;
  cfg.push_back(data::shell_config(
      data::shell_config_name("test"), data::shell_config_data()));

  auto e = new NiceMock<mock::environment>();
  core::in_memory_displayer d;
  main_shell::shell sh(cfg, "", "", engine::constant::create_failing(), {}, {},
                       nullptr, std::unique_ptr<iface::environment>(e));
  ON_CALL(*e, append(_)).WillByDefault(Throw(std::runtime_error("some error")));

  sh.store_in_buffer(data::cpp_code("typedef int foo;"), d);

  ASSERT_FALSE(d.errors().empty());
}

TEST(evaluation, variable_definition)
{
  using data::command;
  ASSERT_TRUE(core::is_environment_setup_command(
      core::to_command(data::cpp_code("int x;"))));
}

TEST(evaluation, function_definition)
{
  using data::command;
  ASSERT_TRUE(core::is_environment_setup_command(
      core::to_command(data::cpp_code("void f() {}"))));
}

TEST(evaluation, is_environment_setup_with_leading_whitespace)
{
  using data::command;
  ASSERT_FALSE(core::is_environment_setup_command(
      core::to_command(data::cpp_code(" int"))));
}

TEST(evaluation, is_environment_setup_without_leading_whitespace)
{
  using data::command;
  ASSERT_FALSE(core::is_environment_setup_command(
      core::to_command(data::cpp_code("int"))));
}

TEST(evaluation, prompt_is_different_in_multiline_input)
{
  core::null_displayer d;
  main_shell::shell sh(
      test_config(), "", "", engine::constant::create_failing());
  sh.line_available(data::user_input("const \\"), d);

  ASSERT_EQ("...>", sh.prompt());
}

TEST(evaluation, command_macro_usage_with_semicolon_is_environment_setup)
{
  using data::command;

  ASSERT_TRUE(core::is_environment_setup_command(
      core::to_command(data::cpp_code("SOME_MACRO(13);"))));
}
