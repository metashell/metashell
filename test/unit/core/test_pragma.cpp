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

#include <metashell/core/build_default_pragma_map.hpp>
#include <metashell/core/command.hpp>
#include <metashell/core/engine_constant.hpp>
#include <metashell/core/in_memory_displayer.hpp>
#include <metashell/core/metashell_pragma.hpp>
#include <metashell/core/null_displayer.hpp>
#include <metashell/core/shell.hpp>

#include "test_config.hpp"

#include <gtest/gtest.h>

using namespace metashell;

namespace
{
  data::command command(const std::string& s_)
  {
    return core::to_command(data::cpp_code(s_));
  }
}

TEST(pragma, parse)
{
  ASSERT_TRUE(bool(core::parse_pragma(command("#pragma metashell foo"))));
}

TEST(pragma, parse_no_pragma) { ASSERT_FALSE(core::parse_pragma(command(""))); }

TEST(pragma, parse_with_inital_whitespace)
{
  ASSERT_TRUE(bool(core::parse_pragma(command(" \t #pragma metashell foo"))));
}

TEST(pragma, whitespace_is_not_pragma)
{
  ASSERT_FALSE(core::parse_pragma(command(" ")));
}

TEST(pragma, gcc_pragma_is_not_metashell_pragma)
{
  ASSERT_FALSE(core::parse_pragma(command("#pragma gcc foo")));
}

TEST(pragma, name_of_pragma)
{
  const data::command c_foo = command("#pragma metashell foo");
  const data::command c_bar = command("#pragma metashell bar");

  const data::command::iterator op_foo = *core::parse_pragma(c_foo),
                                op_bar = *core::parse_pragma(c_bar);

  ASSERT_EQ("foo", op_foo->value());
  ASSERT_EQ("bar", op_bar->value());
}

TEST(pragma, name_of_pragma_is_not_a_literal)
{
  ASSERT_ANY_THROW(core::parse_pragma(command("#pragma metashell 13")));
}

TEST(pragma, name_of_pragma_is_missing)
{
  ASSERT_ANY_THROW(core::parse_pragma(command("#pragma metashell")));
}

TEST(pragma, help_pragma_displays_message)
{
  core::in_memory_displayer d;
  core::shell sh(test_config(), "", "", core::create_failing_engine(),
                 core::build_default_pragma_map(nullptr, "", nullptr));
  sh.line_available("#pragma metashell help", d);
  ASSERT_FALSE(d.comments().empty());
}

TEST(pragma, error_for_non_existing_pragma)
{
  core::in_memory_displayer d;
  core::shell sh(test_config(), "", "", core::create_failing_engine());
  sh.line_available("#pragma metashell foo_bar", d);
  ASSERT_FALSE(d.errors().empty());
}

TEST(pragma, check_verbosity)
{
  core::in_memory_displayer d;
  core::shell sh(test_config(), "", "", core::create_failing_engine(),
                 core::build_default_pragma_map(nullptr, "", nullptr));
  sh.line_available("#pragma metashell verbose", d);
  ASSERT_EQ(
      std::vector<data::text>{data::text("verbose mode is off")}, d.comments());
}

TEST(pragma, check_enabling_verbosity)
{
  core::in_memory_displayer d;
  core::shell sh(test_config(), "", "", core::create_failing_engine(),
                 core::build_default_pragma_map(nullptr, "", nullptr));
  sh.line_available("#pragma metashell verbose on", d);
  ASSERT_EQ(
      std::vector<data::text>{data::text("verbose mode is on")}, d.comments());
}

TEST(pragma, pragma_metashell_does_not_kill_the_shell)
{
  core::null_displayer d;
  core::shell sh(test_config(), "", "", core::create_failing_engine());

  // should not throw
  sh.line_available("#pragma metashell", d);
}

TEST(pragma, quit)
{
  core::in_memory_displayer d;
  core::shell sh(test_config(), "", "", core::create_failing_engine(),
                 core::build_default_pragma_map(nullptr, "", nullptr));
  sh.line_available("#pragma metashell quit", d);
  ASSERT_TRUE(sh.stopped());
}

TEST(pragma, accept_pound_msh_as_pragma_metashell)
{
  ASSERT_TRUE(bool(core::parse_pragma(command("#msh foo"))));
  ASSERT_TRUE(bool(core::parse_pragma(command("# msh foo"))));
  ASSERT_TRUE(bool(core::parse_pragma(command(" # msh foo"))));
}

TEST(pragma, accept_pound_metashell_as_pragma_metashell)
{
  ASSERT_TRUE(bool(core::parse_pragma(command("#metashell foo"))));
  ASSERT_TRUE(bool(core::parse_pragma(command("# metashell foo"))));
  ASSERT_TRUE(bool(core::parse_pragma(command(" # metashell foo"))));
}

TEST(pragma, accept_pragma_msh_as_pragma_metashell)
{
  ASSERT_TRUE(bool(core::parse_pragma(command("#pragma msh foo"))));
  ASSERT_TRUE(bool(core::parse_pragma(command("# pragma msh foo"))));
  ASSERT_TRUE(bool(core::parse_pragma(command(" # pragma msh foo"))));
}
