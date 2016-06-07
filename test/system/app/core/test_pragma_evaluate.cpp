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

#include <metashell_system_test/comment.hpp>
#include <metashell_system_test/error.hpp>
#include <metashell_system_test/prompt.hpp>
#include <metashell_system_test/type.hpp>

#include <metashell_system_test/json_generator.hpp>
#include <metashell_system_test/run_metashell.hpp>

#include <just/test.hpp>

using namespace metashell_system_test;

using pattern::_;

JUST_TEST_CASE(test_pragma_evaluate_runs_a_metaprogram)
{
  const auto r = run_metashell(
      {command("typedef int x;"), command("#pragma metashell evaluate x")});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(type("int"), *i);
  ++i;

  JUST_ASSERT_EQUAL(
      comment({paragraph("You don't need the evaluate add pragma to evaluate "
                         "this metaprogram."
                         " The following command does this as well:"),
               paragraph("x")}),
      *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_pragma_evaluate_displays_error_for_invalid_code)
{
  const auto r =
      run_metashell({command("#pragma metashell evaluate nonexisting_type")});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(error(_), *i);
  ++i;

  JUST_ASSERT_EQUAL(
      comment({paragraph("You don't need the evaluate add pragma to evaluate "
                         "this metaprogram."
                         " The following command does this as well:"),
               paragraph("nonexisting_type")}),
      *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_pragma_evaluate_warns)
{
  const auto r = run_metashell({command("#pragma metashell evaluate int")});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(type("int"), *i);
  ++i;

  JUST_ASSERT_EQUAL(
      comment({paragraph("You don't need the evaluate add pragma to evaluate "
                         "this metaprogram."
                         " The following command does this as well:"),
               paragraph("int")}),
      *i);
  ++i;

  JUST_ASSERT(i == r.end());
}
