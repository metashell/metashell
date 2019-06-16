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

#include <metashell/main_shell/metashell_pragma.hpp>

#include <metashell/core/command.hpp>

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
  ASSERT_TRUE(bool(main_shell::parse_pragma(command("#pragma metashell foo"))));
}

TEST(pragma, parse_no_pragma)
{
  ASSERT_FALSE(main_shell::parse_pragma(command("")));
}

TEST(pragma, parse_with_inital_whitespace)
{
  ASSERT_TRUE(
      bool(main_shell::parse_pragma(command(" \t #pragma metashell foo"))));
}

TEST(pragma, whitespace_is_not_pragma)
{
  ASSERT_FALSE(main_shell::parse_pragma(command(" ")));
}

TEST(pragma, gcc_pragma_is_not_metashell_pragma)
{
  ASSERT_FALSE(main_shell::parse_pragma(command("#pragma gcc foo")));
}

TEST(pragma, name_of_pragma)
{
  const data::command c_foo = command("#pragma metashell foo");
  const data::command c_bar = command("#pragma metashell bar");

  const data::command::iterator op_foo = *main_shell::parse_pragma(c_foo),
                                op_bar = *main_shell::parse_pragma(c_bar);

  ASSERT_EQ("foo", value(*op_foo));
  ASSERT_EQ("bar", value(*op_bar));
}

TEST(pragma, name_of_pragma_is_not_a_literal)
{
  ASSERT_ANY_THROW(main_shell::parse_pragma(command("#pragma metashell 13")));
}

TEST(pragma, name_of_pragma_is_missing)
{
  ASSERT_ANY_THROW(main_shell::parse_pragma(command("#pragma metashell")));
}

TEST(pragma, accept_pound_msh_as_pragma_metashell)
{
  ASSERT_TRUE(bool(main_shell::parse_pragma(command("#msh foo"))));
  ASSERT_TRUE(bool(main_shell::parse_pragma(command("# msh foo"))));
  ASSERT_TRUE(bool(main_shell::parse_pragma(command(" # msh foo"))));
}

TEST(pragma, accept_pound_metashell_as_pragma_metashell)
{
  ASSERT_TRUE(bool(main_shell::parse_pragma(command("#metashell foo"))));
  ASSERT_TRUE(bool(main_shell::parse_pragma(command("# metashell foo"))));
  ASSERT_TRUE(bool(main_shell::parse_pragma(command(" # metashell foo"))));
}

TEST(pragma, accept_pragma_msh_as_pragma_metashell)
{
  ASSERT_TRUE(bool(main_shell::parse_pragma(command("#pragma msh foo"))));
  ASSERT_TRUE(bool(main_shell::parse_pragma(command("# pragma msh foo"))));
  ASSERT_TRUE(bool(main_shell::parse_pragma(command(" # pragma msh foo"))));
}
