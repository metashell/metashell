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

#include <metashell/in_memory_displayer.hpp>
#include <metashell/shell.hpp>
#include <metashell/null_executable.hpp>
#include <metashell/engine_constant.hpp>

#include "test_config.hpp"

#include <just/test.hpp>

#include <algorithm>

using namespace metashell;

namespace
{
  bool contains(const std::string& substr_, const std::string& s_)
  {
    return s_.find(substr_) != std::string::npos;
  }

  bool contains(
    const std::string& substr_,
    const std::vector<data::paragraph>& v_
  )
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

JUST_TEST_CASE(test_pragma_help_with_no_arguments)
{
  in_memory_displayer d;
  metashell::null_executable clang_binary;
  metashell::shell
    sh(
      metashell::test_config(),
      clang_binary,
      "",
      "",
      metashell::create_failing_engine()
    );
  sh.line_available("#pragma metashell help", d);

  JUST_ASSERT(!d.comments().empty());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(test_pragma_help_with_non_existing_pragma_argument)
{
  in_memory_displayer d;
  metashell::null_executable clang_binary;
  metashell::shell
    sh(
      metashell::test_config(),
      clang_binary,
      "",
      "",
      metashell::create_failing_engine()
    );
  sh.line_available("#pragma metashell help foo", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.comments());
  JUST_ASSERT_EQUAL_CONTAINER({"Pragma foo not found."}, d.errors());
}

JUST_TEST_CASE(test_pragma_help_with_non_existing_pragma_argument_2)
{
  in_memory_displayer d;
  metashell::null_executable clang_binary;
  metashell::shell
    sh(
      metashell::test_config(),
      clang_binary,
      "",
      "",
      metashell::create_failing_engine()
    );
  sh.line_available("#pragma metashell help foo bar", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.comments());
  JUST_ASSERT_EQUAL_CONTAINER({"Pragma foo bar not found."}, d.errors());
}

JUST_TEST_CASE(test_pragma_help_for_a_pragma)
{
  in_memory_displayer d;
  metashell::null_executable clang_binary;
  metashell::shell
    sh(
      metashell::test_config(),
      clang_binary,
      "",
      "",
      metashell::create_failing_engine()
    );
  sh.line_available("#pragma metashell help help", d);

  JUST_ASSERT(!d.comments().empty());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
}

JUST_TEST_CASE(
  test_pragma_help_for_a_pragma_which_is_also_the_prefix_of_other_pragmas
)
{
  in_memory_displayer d;
  metashell::null_executable clang_binary;
  metashell::shell
    sh(
      metashell::test_config(),
      clang_binary,
      "",
      "",
      metashell::create_failing_engine()
    );
  sh.line_available("#msh help environment", d);

  JUST_ASSERT_EQUAL(1u, d.comments().size());
  const auto& ps = d.comments().front().paragraphs;
  JUST_ASSERT(contains("#msh environment", ps));
  JUST_ASSERT(contains("#msh environment pop", ps));
}

