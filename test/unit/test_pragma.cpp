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

#include <metashell/engine_constant.hpp>
#include <metashell/in_memory_displayer.hpp>
#include <metashell/metashell_pragma.hpp>
#include <metashell/null_displayer.hpp>
#include <metashell/shell.hpp>

#include "test_config.hpp"

#include <gtest/gtest.h>

using namespace metashell;

TEST(pragma, parse)
{
  ASSERT_TRUE(bool(parse_pragma(data::command("#pragma metashell foo"))));
}

TEST(pragma, parse_no_pragma) { ASSERT_FALSE(parse_pragma(data::command(""))); }

TEST(pragma, parse_with_inital_whitespace)
{
  ASSERT_TRUE(bool(parse_pragma(data::command(" \t #pragma metashell foo"))));
}

TEST(pragma, whitespace_is_not_pragma)
{
  ASSERT_FALSE(parse_pragma(data::command(" ")));
}

TEST(pragma, gcc_pragma_is_not_metashell_pragma)
{
  ASSERT_FALSE(parse_pragma(data::command("#pragma gcc foo")));
}

TEST(pragma, name_of_pragma)
{
  const data::command c_foo("#pragma metashell foo");
  const data::command c_bar("#pragma metashell bar");

  const data::command::iterator op_foo = *parse_pragma(c_foo),
                                op_bar = *parse_pragma(c_bar);

  ASSERT_EQ("foo", op_foo->value());
  ASSERT_EQ("bar", op_bar->value());
}

TEST(pragma, name_of_pragma_is_not_a_literal)
{
  ASSERT_ANY_THROW(parse_pragma(data::command("#pragma metashell 13")));
}

TEST(pragma, name_of_pragma_is_missing)
{
  ASSERT_ANY_THROW(parse_pragma(data::command("#pragma metashell")));
}

TEST(pragma, help_pragma_displays_message)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());
  sh.line_available("#pragma metashell help", d);
  ASSERT_FALSE(d.comments().empty());
}

TEST(pragma, error_for_non_existing_pragma)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());
  sh.line_available("#pragma metashell foo_bar", d);
  ASSERT_FALSE(d.errors().empty());
}

TEST(pragma, check_verbosity)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());
  sh.line_available("#pragma metashell verbose", d);
  ASSERT_EQ(
      std::vector<data::text>{data::text("verbose mode is off")}, d.comments());
}

TEST(pragma, check_enabling_verbosity)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());
  sh.line_available("#pragma metashell verbose on", d);
  ASSERT_EQ(
      std::vector<data::text>{data::text("verbose mode is on")}, d.comments());
}

TEST(pragma, pragma_metashell_does_not_kill_the_shell)
{
  null_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());

  // should not throw
  sh.line_available("#pragma metashell", d);
}

TEST(pragma, quit)
{
  in_memory_displayer d;
  shell sh(test_config(), "", "", "", create_failing_engine());
  sh.line_available("#pragma metashell quit", d);
  ASSERT_TRUE(sh.stopped());
}

TEST(pragma, accept_pound_msh_as_pragma_metashell)
{
  ASSERT_TRUE(bool(parse_pragma(data::command("#msh foo"))));
  ASSERT_TRUE(bool(parse_pragma(data::command("# msh foo"))));
  ASSERT_TRUE(bool(parse_pragma(data::command(" # msh foo"))));
}

TEST(pragma, accept_pound_metashell_as_pragma_metashell)
{
  ASSERT_TRUE(bool(parse_pragma(data::command("#metashell foo"))));
  ASSERT_TRUE(bool(parse_pragma(data::command("# metashell foo"))));
  ASSERT_TRUE(bool(parse_pragma(data::command(" # metashell foo"))));
}

TEST(pragma, accept_pragma_msh_as_pragma_metashell)
{
  ASSERT_TRUE(bool(parse_pragma(data::command("#pragma msh foo"))));
  ASSERT_TRUE(bool(parse_pragma(data::command("# pragma msh foo"))));
  ASSERT_TRUE(bool(parse_pragma(data::command(" # pragma msh foo"))));
}
