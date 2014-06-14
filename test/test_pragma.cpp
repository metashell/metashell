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
#include "test_shell.hpp"

#include <just/test.hpp>

using namespace metashell;

JUST_TEST_CASE(test_parse_pragma)
{
  JUST_ASSERT(parse_pragma("#pragma metashell foo"));
}

JUST_TEST_CASE(test_parse_no_pragma)
{
  JUST_ASSERT(!parse_pragma(""));
}

JUST_TEST_CASE(test_parse_pragma_with_inital_whitespace)
{
  JUST_ASSERT(parse_pragma(" \t #pragma metashell foo"));
}

JUST_TEST_CASE(test_whitespace_is_not_pragma)
{
  JUST_ASSERT(!parse_pragma(" "));
}

JUST_TEST_CASE(test_gcc_pragma_is_not_metashell_pragma)
{
  JUST_ASSERT(!parse_pragma("#pragma gcc foo"));
}

JUST_TEST_CASE(test_name_of_pragma)
{
  const token_iterator
    op_foo = *parse_pragma("#pragma metashell foo"),
    op_bar = *parse_pragma("#pragma metashell bar");

  JUST_ASSERT_EQUAL("foo", op_foo->get_value());
  JUST_ASSERT_EQUAL("bar", op_bar->get_value());
}

JUST_TEST_CASE(test_name_of_pragma_is_not_a_literal)
{
  JUST_ASSERT_THROWS_SOMETHING(parse_pragma("#pragma metashell 13"));
}

JUST_TEST_CASE(test_name_of_pragma_is_missing)
{
  JUST_ASSERT_THROWS_SOMETHING(parse_pragma("#pragma metashell"));
}

JUST_TEST_CASE(test_help_pragma_displays_message)
{
  test_shell sh;
  sh.line_available("#pragma metashell help");
  JUST_ASSERT(!sh.output().empty());
}

JUST_TEST_CASE(test_error_for_non_existing_pragma)
{
  test_shell sh;
  sh.line_available("#pragma metashell foo_bar");
  JUST_ASSERT(!sh.error().empty());
}

JUST_TEST_CASE(test_check_verbosity)
{
  test_shell sh;
  sh.line_available("#pragma metashell verbose");
  JUST_ASSERT_EQUAL("// verbose mode is off\n", sh.output());
}

JUST_TEST_CASE(test_check_enabling_verbosity)
{
  test_shell sh;
  sh.line_available("#pragma metashell verbose on");
  JUST_ASSERT_EQUAL("// verbose mode is on\n", sh.output());
}

JUST_TEST_CASE(test_pragma_metashell_does_not_kill_the_shell)
{
  test_shell sh;

  // should not throw
  sh.line_available("#pragma metashell");
}

JUST_TEST_CASE(test_quit)
{
  test_shell sh;
  sh.line_available("#pragma metashell quit");
  JUST_ASSERT(sh.stopped());
}

