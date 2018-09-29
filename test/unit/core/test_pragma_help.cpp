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

#include <metashell/core/engine_constant.hpp>
#include <metashell/core/in_memory_displayer.hpp>
#include <metashell/core/shell.hpp>

#include "empty_container.hpp"
#include "test_config.hpp"

#include <gtest/gtest.h>

#include <algorithm>

using namespace metashell;

namespace
{
  bool contains(const std::string& substr_, const std::string& s_)
  {
    return s_.find(substr_) != std::string::npos;
  }

  bool contains(const std::string& substr_,
                const std::vector<data::paragraph>& v_)
  {
    for (const auto& p : v_)
    {
      if (contains(substr_, p.content))
      {
        return true;
      }
    }
    return false;
  }
}

TEST(pragma_help, no_arguments)
{
  core::in_memory_displayer d;
  core::shell sh(
      metashell::test_config(), "", "", "", core::create_failing_engine());
  sh.line_available("#pragma metashell help", d);

  ASSERT_FALSE(d.comments().empty());
  ASSERT_EQ(empty_container, d.errors());
}

TEST(pragma_help, non_existing_pragma_argument)
{
  core::in_memory_displayer d;
  core::shell sh(
      metashell::test_config(), "", "", "", core::create_failing_engine());
  sh.line_available("#pragma metashell help foo", d);

  ASSERT_EQ(empty_container, d.comments());
  ASSERT_EQ(std::vector<std::string>{"Pragma foo not found."}, d.errors());
}

TEST(pragma_help, non_existing_pragma_argument_2)
{
  core::in_memory_displayer d;
  core::shell sh(
      metashell::test_config(), "", "", "", core::create_failing_engine());
  sh.line_available("#pragma metashell help foo bar", d);

  ASSERT_EQ(empty_container, d.comments());
  ASSERT_EQ(std::vector<std::string>{"Pragma foo bar not found."}, d.errors());
}

TEST(pragma_help, for_a_pragma)
{
  core::in_memory_displayer d;
  core::shell sh(
      metashell::test_config(), "", "", "", core::create_failing_engine());
  sh.line_available("#pragma metashell help help", d);

  ASSERT_FALSE(d.comments().empty());
  ASSERT_EQ(empty_container, d.errors());
}

TEST(pragma_help, for_a_pragma_which_is_also_the_prefix_of_other_pragmas)
{
  core::in_memory_displayer d;
  core::shell sh(
      metashell::test_config(), "", "", "", core::create_failing_engine());
  sh.line_available("#msh help environment", d);

  ASSERT_EQ(1u, d.comments().size());
  const auto& ps = d.comments().front().paragraphs;
  ASSERT_TRUE(contains("#msh environment", ps));
  ASSERT_TRUE(contains("#msh environment pop", ps));
}
