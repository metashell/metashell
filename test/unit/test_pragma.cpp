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

#include <metashell/metashell_pragma.hpp>
#include <metashell/in_memory_displayer.hpp>
#include <metashell/null_displayer.hpp>
#include <metashell/shell.hpp>
#include <metashell/null_executable.hpp>

#include "test_config.hpp"

#include <just/test.hpp>

using namespace metashell;

JUST_TEST_CASE(test_parse_pragma)
{
  JUST_ASSERT(bool(parse_pragma(data::command("#pragma metashell foo"))));
}

JUST_TEST_CASE(test_parse_no_pragma)
{
  JUST_ASSERT(!parse_pragma(data::command("")));
}

JUST_TEST_CASE(test_parse_pragma_with_inital_whitespace)
{
  JUST_ASSERT(bool(parse_pragma(data::command(" \t #pragma metashell foo"))));
}

JUST_TEST_CASE(test_whitespace_is_not_pragma)
{
  JUST_ASSERT(!parse_pragma(data::command(" ")));
}

JUST_TEST_CASE(test_gcc_pragma_is_not_metashell_pragma)
{
  JUST_ASSERT(!parse_pragma(data::command("#pragma gcc foo")));
}

JUST_TEST_CASE(test_name_of_pragma)
{
  const data::command c_foo("#pragma metashell foo");
  const data::command c_bar("#pragma metashell bar");

  const data::command::iterator
    op_foo = *parse_pragma(c_foo),
    op_bar = *parse_pragma(c_bar);

  JUST_ASSERT_EQUAL("foo", op_foo->value());
  JUST_ASSERT_EQUAL("bar", op_bar->value());
}

JUST_TEST_CASE(test_name_of_pragma_is_not_a_literal)
{
  JUST_ASSERT_THROWS(
    [] { parse_pragma(data::command("#pragma metashell 13")); }
  );
}

JUST_TEST_CASE(test_name_of_pragma_is_missing)
{
  JUST_ASSERT_THROWS([] { parse_pragma(data::command("#pragma metashell")); });
}

JUST_TEST_CASE(test_help_pragma_displays_message)
{
  in_memory_displayer d;
  metashell::null_executable clang_binary;
  shell sh(test_config(), clang_binary);
  sh.line_available("#pragma metashell help", d);
  JUST_ASSERT(!d.comments().empty());
}

JUST_TEST_CASE(test_error_for_non_existing_pragma)
{
  in_memory_displayer d;
  metashell::null_executable clang_binary;
  shell sh(test_config(), clang_binary);
  sh.line_available("#pragma metashell foo_bar", d);
  JUST_ASSERT(!d.errors().empty());
}

JUST_TEST_CASE(test_check_verbosity)
{
  in_memory_displayer d;
  metashell::null_executable clang_binary;
  shell sh(test_config(), clang_binary);
  sh.line_available("#pragma metashell verbose", d);
  JUST_ASSERT_EQUAL_CONTAINER(
    {data::text("verbose mode is off")},
    d.comments()
  );
}

JUST_TEST_CASE(test_check_enabling_verbosity)
{
  in_memory_displayer d;
  metashell::null_executable clang_binary;
  shell sh(test_config(), clang_binary);
  sh.line_available("#pragma metashell verbose on", d);
  JUST_ASSERT_EQUAL_CONTAINER({data::text("verbose mode is on")}, d.comments());
}

JUST_TEST_CASE(test_pragma_metashell_does_not_kill_the_shell)
{
  null_displayer d;
  metashell::null_executable clang_binary;
  shell sh(test_config(), clang_binary);

  // should not throw
  sh.line_available("#pragma metashell", d);
}

JUST_TEST_CASE(test_quit)
{
  in_memory_displayer d;
  metashell::null_executable clang_binary;
  shell sh(test_config(), clang_binary);
  sh.line_available("#pragma metashell quit", d);
  JUST_ASSERT(sh.stopped());
}

JUST_TEST_CASE(test_accept_pound_msh_as_pragma_metashell)
{
  JUST_ASSERT(bool(parse_pragma(data::command("#msh foo"))));
  JUST_ASSERT(bool(parse_pragma(data::command("# msh foo"))));
  JUST_ASSERT(bool(parse_pragma(data::command(" # msh foo"))));
}

JUST_TEST_CASE(test_accept_pound_metashell_as_pragma_metashell)
{
  JUST_ASSERT(bool(parse_pragma(data::command("#metashell foo"))));
  JUST_ASSERT(bool(parse_pragma(data::command("# metashell foo"))));
  JUST_ASSERT(bool(parse_pragma(data::command(" # metashell foo"))));
}

JUST_TEST_CASE(test_accept_pragma_msh_as_pragma_metashell)
{
  JUST_ASSERT(bool(parse_pragma(data::command("#pragma msh foo"))));
  JUST_ASSERT(bool(parse_pragma(data::command("# pragma msh foo"))));
  JUST_ASSERT(bool(parse_pragma(data::command(" # pragma msh foo"))));
}

