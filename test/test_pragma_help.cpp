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

#include "test_shell.hpp"

#include <just/test.hpp>

#include <algorithm>

using namespace metashell;

namespace
{
  bool contains(const std::string& substr_, const std::string& s_)
  {
    return s_.find(substr_) != std::string::npos;
  }
}

JUST_TEST_CASE(test_pragma_help_with_no_arguments)
{
  test_shell sh;
  sh.line_available("#pragma metashell help");

  JUST_ASSERT(!sh.output().empty());
  JUST_ASSERT(sh.error().empty());
}

JUST_TEST_CASE(test_pragma_help_with_non_existing_pragma_argument)
{
  test_shell sh;
  sh.line_available("#pragma metashell help foo");

  JUST_ASSERT(sh.output().empty());
  JUST_ASSERT_EQUAL("Pragma foo not found.", sh.error());
}

JUST_TEST_CASE(test_pragma_help_with_non_existing_pragma_argument_2)
{
  test_shell sh;
  sh.line_available("#pragma metashell help foo bar");

  JUST_ASSERT(sh.output().empty());
  JUST_ASSERT_EQUAL("Pragma foo bar not found.", sh.error());
}

JUST_TEST_CASE(test_pragma_help_for_a_pragma)
{
  test_shell sh;
  sh.line_available("#pragma metashell help help");

  JUST_ASSERT(!sh.output().empty());
  JUST_ASSERT(sh.error().empty());
}

JUST_TEST_CASE(
  test_pragma_help_for_a_pragma_which_is_also_the_prefix_of_other_pragmas
)
{
  test_shell sh;
  sh.line_available("#msh help environment");

  JUST_ASSERT(contains("#msh environment\n", sh.output()));
  JUST_ASSERT(contains("#msh environment pop\n", sh.output()));
}

