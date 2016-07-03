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
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/prompt.hpp>

#include <just/test.hpp>

using namespace metashell::system_test;

namespace
{
  std::string macro_in_marker() { return "__METASHELL_PP_MARKER"; }
  std::string marker() { return "* __METASHELL_PP_MARKER *"; }
}

JUST_TEST_CASE(test_pp_empty)
{
  metashell_instance mi;

  const std::vector<json_string> resp = mi.command("#msh pp");

  JUST_ASSERT_EQUAL_CONTAINER({to_json_string(prompt(">"))}, resp);
}

JUST_TEST_CASE(test_pp_non_macro)
{
  JUST_ASSERT_EQUAL(
      cpp_code("int"), metashell_instance().command("#msh pp int").front());
}

JUST_TEST_CASE(test_pp_macro)
{
  metashell_instance mi;
  mi.command("#define FOO bar");

  JUST_ASSERT_EQUAL(cpp_code("bar"), mi.command("#msh pp FOO").front());
}

JUST_TEST_CASE(test_pp_exp_with_multiple_tokens)
{
  metashell_instance mi;
  mi.command("#define FOO bar");

  JUST_ASSERT_EQUAL(cpp_code("bar int"), mi.command("#msh pp FOO int").front());
}

JUST_TEST_CASE(test_pp_marker)
{
  JUST_ASSERT_EQUAL(
      error("Marker (" + marker() +
            ") found more than two times in preprocessed output."),
      metashell_instance().command("#msh pp " + marker()).front());
}

JUST_TEST_CASE(test_pp_with_marker_defined)
{
  metashell_instance mi;
  mi.command("#define " + macro_in_marker() + " foo");

  JUST_ASSERT_EQUAL(error("Marker (" + marker() +
                          ") not found in preprocessed output."
                          " Does it contain a macro that has been defined?"),
                    mi.command("#msh pp " + marker()).front());
}

JUST_TEST_CASE(test_pp_preprocessor_directive)
{
  JUST_ASSERT_EQUAL(cpp_code("#error foo"),
                    metashell_instance().command("#msh pp #error foo").front());
}
