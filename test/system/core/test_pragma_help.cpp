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

#include <metashell/system_test/comment.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/prompt.hpp>

#include <metashell/system_test/metashell_instance.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

using pattern::_;

namespace
{
  bool contains(const std::string& substr_, const std::string& s_)
  {
    return s_.find(substr_) != std::string::npos;
  }
}

TEST(pragma_help, no_arguments)
{
  metashell_instance mi;

  const auto result = mi.command("#pragma metashell help");

  ASSERT_EQ(2u, result.size());
  ASSERT_EQ(comment(_), result.front());
  ASSERT_EQ(prompt(">"), result.back());
}

TEST(pragma_help, non_existing_pragma_argument)
{
  metashell_instance mi;

  const auto result = mi.command("#pragma metashell help foo");

  ASSERT_EQ(2u, result.size());
  ASSERT_EQ(error("Pragma foo not found."), result.front());
  ASSERT_EQ(prompt(">"), result.back());
}

TEST(pragma_help, non_existing_pragma_argument_2)
{
  metashell_instance mi;

  const auto result = mi.command("#pragma metashell help foo bar");

  ASSERT_EQ(2u, result.size());
  ASSERT_EQ(error("Pragma foo bar not found."), result.front());
  ASSERT_EQ(prompt(">"), result.back());
}

TEST(pragma_help, for_a_pragma)
{
  metashell_instance mi;

  const auto result = mi.command("#pragma metashell help help");

  ASSERT_EQ(2u, result.size());
  ASSERT_EQ(comment(_), result.front());
  ASSERT_EQ(prompt(">"), result.back());
}

TEST(pragma_help, for_a_pragma_which_is_also_the_prefix_of_other_pragmas)
{
  metashell_instance mi;

  const auto result = mi.command("#msh help environment");

  ASSERT_EQ(2u, result.size());

  ASSERT_EQ(comment(_), result.front());
  ASSERT_TRUE(contains("#msh environment", result.front().get()));
  ASSERT_TRUE(contains("#msh environment pop", result.front().get()));

  ASSERT_EQ(prompt(">"), result.back());
}
