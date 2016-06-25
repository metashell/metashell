// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/cpp_code.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/json_generator.hpp>
#include <metashell/system_test/run_metashell.hpp>

#include <just/test.hpp>

using namespace metashell::system_test;

namespace
{
  std::string macro_in_marker() { return "__METASHELL_PP_MARKER"; }
  std::string marker() { return "* __METASHELL_PP_MARKER *"; }
}

JUST_TEST_CASE(test_pp_empty)
{
  const auto r = run_metashell({command("#msh pp")});

  auto i = r.begin();

  JUST_ASSERT(i != r.end());
  ++i;
  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_pp_non_macro)
{
  const auto r = run_metashell({command("#msh pp int")});

  auto i = r.begin() + 1;

  JUST_ASSERT_EQUAL(cpp_code("int"), *i);
}

JUST_TEST_CASE(test_pp_macro)
{
  const auto r =
      run_metashell({command("#define FOO bar"), command("#msh pp FOO")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(cpp_code("bar"), *i);
}

JUST_TEST_CASE(test_pp_exp_with_multiple_tokens)
{
  const auto r =
      run_metashell({command("#define FOO bar"), command("#msh pp FOO int")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(cpp_code("bar int"), *i);
}

JUST_TEST_CASE(test_pp_marker)
{
  const auto r = run_metashell({command("#msh pp " + marker())});

  auto i = r.begin() + 1;

  JUST_ASSERT_EQUAL(
      error("Marker (" + marker() +
            ") found more than two times in preprocessed output."),
      *i);
}

JUST_TEST_CASE(test_pp_with_marker_defined)
{
  const auto r =
      run_metashell({command("#define " + macro_in_marker() + " foo"),
                     command("#msh pp " + marker())});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(error("Marker (" + marker() +
                          ") not found in preprocessed output."
                          " Does it contain a macro that has been defined?"),
                    *i);
}

JUST_TEST_CASE(test_pp_preprocessor_directive)
{
  const auto r = run_metashell({command("#msh pp #error foo")});

  auto i = r.begin() + 1;

  JUST_ASSERT_EQUAL(cpp_code("#error foo"), *i);
}
