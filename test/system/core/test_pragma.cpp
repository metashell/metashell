// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/comment.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/prompt.hpp>

#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/metashell_terminated.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

TEST(pragma, error_for_non_existing_pragma)
{
  metashell_instance mi;

  const auto result = mi.command("#pragma metashell foo_bar");

  ASSERT_EQ(2u, result.size());
  ASSERT_EQ(error("Error: Pragma foo_bar not found"), result.front());
  ASSERT_EQ(prompt(">"), result.back());
}

TEST(pragma, check_verbosity)
{
  metashell_instance mi;

  const auto result = mi.command("#pragma metashell verbose");

  ASSERT_EQ(2u, result.size());
  ASSERT_EQ(comment({paragraph("verbose mode is off")}), result.front());
  ASSERT_EQ(prompt(">"), result.back());
}

TEST(pragma, check_enabling_verbosity)
{
  metashell_instance mi;

  const auto result = mi.command("#pragma metashell verbose on");

  ASSERT_EQ(2u, result.size());
  ASSERT_EQ(comment({paragraph("verbose mode is on")}), result.front());
  ASSERT_EQ(prompt(">"), result.back());
}

TEST(pragma, pragma_metashell_does_not_kill_the_shell)
{
  metashell_instance mi;

  mi.command("#pragma metashell");
  const auto result = mi.command("#pragma metashell help");

  ASSERT_EQ(prompt(">"), result.back());
}

TEST(pragma, quit)
{
  metashell_instance mi;

  ASSERT_THROW(mi.command("#pragma metashell quit"), metashell_terminated);
}
