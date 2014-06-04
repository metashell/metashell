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

typedef boost::optional<metashell_pragma> optional_pragma;

JUST_TEST_CASE(test_parse_pragma)
{
  const optional_pragma op = metashell_pragma::parse("#pragma metashell foo");

  JUST_ASSERT(op);
}

JUST_TEST_CASE(test_parse_no_pragma)
{
  const optional_pragma op = metashell_pragma::parse("");

  JUST_ASSERT(!op);
}

JUST_TEST_CASE(test_parse_pragma_with_inital_whitespace)
{
  const optional_pragma
    op = metashell_pragma::parse(" \t #pragma metashell foo");

  JUST_ASSERT(op);
}

JUST_TEST_CASE(test_whitespace_is_not_pragma)
{
  const optional_pragma op = metashell_pragma::parse(" ");

  JUST_ASSERT(!op);
}

JUST_TEST_CASE(test_gcc_pragma_is_not_metashell_pragma)
{
  const optional_pragma op = metashell_pragma::parse("#pragma gcc foo");

  JUST_ASSERT(!op);
}

JUST_TEST_CASE(test_name_of_pragma)
{
  const optional_pragma op_foo
    = metashell_pragma::parse("#pragma metashell foo");

  const optional_pragma op_bar
    = metashell_pragma::parse("#pragma metashell bar");

  JUST_ASSERT_EQUAL("foo", op_foo->name());
  JUST_ASSERT_EQUAL("bar", op_bar->name());
}

JUST_TEST_CASE(test_name_of_pragma_is_not_a_literal)
{
  JUST_ASSERT_THROWS_SOMETHING(metashell_pragma::parse("#pragma metashell 13"));
}

JUST_TEST_CASE(test_name_of_pragma_is_missing)
{
  JUST_ASSERT_THROWS_SOMETHING(metashell_pragma::parse("#pragma metashell"));
}

JUST_TEST_CASE(test_first_pragma_argument)
{
  const optional_pragma op
    = metashell_pragma::parse("#pragma metashell foo bar");

  JUST_ASSERT_EQUAL("bar", op->begin()->get_value());
}

JUST_TEST_CASE(test_no_pragma_arguments)
{
  const optional_pragma op = metashell_pragma::parse("#pragma metashell foo");

  JUST_ASSERT(op->begin() == op->end());
}

JUST_TEST_CASE(test_non_empty_list_of_pragma_arguments)
{
  const optional_pragma op
    = metashell_pragma::parse("#pragma metashell foo bar");

  JUST_ASSERT(op->begin() != op->end());
}

JUST_TEST_CASE(test_pargma_argument_values)
{
  const optional_pragma op
    = metashell_pragma::parse("#pragma metashell foo a1 a2 a3");

  token_iterator i = op->begin();
  JUST_ASSERT_EQUAL("a1", i->get_value());
  ++i;
  JUST_ASSERT_EQUAL(" ", i->get_value());
  ++i;
  JUST_ASSERT_EQUAL("a2", i->get_value());
  ++i;
  JUST_ASSERT_EQUAL(" ", i->get_value());
  ++i;
  JUST_ASSERT_EQUAL("a3", i->get_value());
  ++i;
  JUST_ASSERT(i == op->end());
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

