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

#include "test_config.hpp"
#include "breaking_environment.hpp"
#include "util.hpp"

#include <metashell/shell.hpp>
#include <metashell/metashell.hpp>
#include <metashell/path_builder.hpp>
#include <metashell/null_displayer.hpp>
#include <metashell/null_history.hpp>
#include <metashell/in_memory_displayer.hpp>
#include <metashell/in_memory_history.hpp>
#include <metashell/null_executable.hpp>
#include <metashell/engine_constant.hpp>
#include <metashell/engine_clang.hpp>

#include <just/test.hpp>

using namespace metashell;

JUST_TEST_CASE(test_accept_empty_input)
{
  in_memory_displayer d;
  null_executable clang_binary;
  shell sh(test_config(), clang_binary, create_failing_engine());
  sh.line_available("", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_accept_space_input)
{
  in_memory_displayer d;
  null_executable clang_binary;
  shell sh(test_config(), clang_binary, create_failing_engine());
  sh.line_available(" ", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_accept_tab_input)
{
  in_memory_displayer d;
  null_executable clang_binary;
  shell sh(test_config(), clang_binary, create_failing_engine());
  sh.line_available("\t", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_accept_vertical_tab_input)
{
  in_memory_displayer d;
  null_executable clang_binary;
  shell sh(test_config(), clang_binary, create_failing_engine());
  sh.line_available("\v", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_accept_new_line_input)
{
  in_memory_displayer d;
  null_executable clang_binary;
  shell sh(test_config(), clang_binary, create_failing_engine());
  sh.line_available("\n", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_accept_carrige_return_input)
{
  in_memory_displayer d;
  null_executable clang_binary;
  shell sh(test_config(), clang_binary, create_failing_engine());
  sh.line_available("\r", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_accept_two_space_input)
{
  in_memory_displayer d;
  null_executable clang_binary;
  shell sh(test_config(), clang_binary, create_failing_engine());
  sh.line_available("  ", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_history_is_stored)
{
  null_displayer d;
  in_memory_history h;
  null_executable clang_binary;
  shell sh(test_config(), clang_binary, create_failing_engine());

  sh.line_available("int", d, h);

  JUST_ASSERT_EQUAL_CONTAINER({"int"}, h.commands());
}

JUST_TEST_CASE(test_empty_line_is_not_stored_in_history)
{
  null_displayer d;
  in_memory_history h;
  null_executable clang_binary;
  shell sh(test_config(), clang_binary, create_failing_engine());

  sh.line_available("", d, h);

  JUST_ASSERT_EMPTY_CONTAINER(h.commands());
}

JUST_TEST_CASE(
  test_line_containing_just_whitespace_is_not_stored_in_history
)
{
  null_displayer d;
  in_memory_history h;
  null_executable clang_binary;
  shell sh(test_config(), clang_binary, create_failing_engine());

  sh.line_available(" ", d, h);

  JUST_ASSERT_EMPTY_CONTAINER(h.commands());
}

JUST_TEST_CASE(
  test_the_same_thing_following_each_other_is_not_added_to_history_twice
)
{
  null_displayer d;
  in_memory_history h;
  null_executable clang_binary;
  shell sh(test_config(), clang_binary, create_failing_engine());

  sh.line_available("int", d, h);
  sh.line_available("int", d, h);

  JUST_ASSERT_EQUAL_CONTAINER({"int"}, h.commands());
}

JUST_TEST_CASE(test_accept_c_comment_input)
{
  in_memory_displayer d;
  null_executable clang_binary;
  shell sh(test_config(), clang_binary, create_failing_engine());
  sh.line_available("/* some comment */", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_accept_cpp_comment_input)
{
  in_memory_displayer d;
  null_executable clang_binary;
  shell sh(test_config(), clang_binary, create_failing_engine());
  sh.line_available("// some comment", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_comment_is_stored_in_history)
{
  null_displayer d;
  in_memory_history h;
  null_executable clang_binary;
  shell sh(test_config(), clang_binary, create_failing_engine());

  sh.line_available("// some comment", d, h);

  JUST_ASSERT_EQUAL(1u, h.commands().size());
}

JUST_TEST_CASE(test_throwing_environment_update_not_breaking_shell)
{
  data::config cfg;
  breaking_environment* e = new breaking_environment();
  in_memory_displayer d;
  null_history h;
  command_processor_queue cpq;
  cpq.history(h);
  null_executable clang_binary;
  shell
    sh(
      cfg,
      std::unique_ptr<breaking_environment>(e),
      cpq,
      clang_binary,
      create_failing_engine()
    );
  e->append_throw_from_now();

  sh.store_in_buffer("typedef int foo;", d);

  JUST_ASSERT(!d.errors().empty());
}

JUST_TEST_CASE(test_throwing_environment_not_breaking_validate)
{
  data::config cfg;
  breaking_environment e;
  e.get_appended_throw_from_now();
  null_executable clang_binary;
  const data::result r =
    create_clang_engine(clang_binary, nullptr)
      ->validate_code("typedef int foo;", cfg, e);

  JUST_ASSERT(!r.successful);
  JUST_ASSERT(!r.error.empty());
}

JUST_TEST_CASE(test_variable_definition)
{
  using data::command;
  JUST_ASSERT(is_environment_setup_command(command("int x;")));
}

JUST_TEST_CASE(test_function_definition)
{
  using data::command;
  JUST_ASSERT(is_environment_setup_command(command("void f() {}")));
}

JUST_TEST_CASE(test_is_environment_setup_with_leading_whitespace)
{
  using data::command;
  JUST_ASSERT(!is_environment_setup_command(command(" int")));
}

JUST_TEST_CASE(test_is_environment_setup_without_leading_whitespace)
{
  using data::command;
  JUST_ASSERT(!is_environment_setup_command(command("int")));
}

JUST_TEST_CASE(test_prompt_is_different_in_multiline_input)
{
  null_displayer d;
  null_executable clang_binary;
  shell sh(test_config(), clang_binary, create_failing_engine());
  sh.line_available("const \\", d);

  JUST_ASSERT_EQUAL("...>", sh.prompt());
}

JUST_TEST_CASE(test_command_macro_usage_with_semicolon_is_environment_setup)
{
  using data::command;

  JUST_ASSERT(is_environment_setup_command(command("SOME_MACRO(13);")));
}

