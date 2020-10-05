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

#include <gtest/gtest.h>

using namespace metashell::system_test;

namespace
{
  std::string macro_in_marker() { return "__METASHELL_PP_MARKER"; }
  std::string marker() { return "* __METASHELL_PP_MARKER *"; }
} // namespace

TEST(pp, empty)
{
  ASSERT_EQ(std::vector<json_string>{to_json_string(prompt(">"))},
            metashell_instance().command("#msh pp"));
}

TEST(pp, non_macro)
{
  ASSERT_EQ(
      cpp_code("int"), metashell_instance().command("#msh pp int").front());
}

TEST(pp, macro)
{
  metashell_instance mi;
  mi.command("#define FOO bar");

  ASSERT_EQ(cpp_code("bar"), mi.command("#msh pp FOO").front());
}

TEST(pp, exp_with_multiple_tokens)
{
  metashell_instance mi;
  mi.command("#define FOO bar");

  ASSERT_EQ(cpp_code("bar int"), mi.command("#msh pp FOO int").front());
}

TEST(pp, marker)
{
  ASSERT_EQ(error("Marker (" + marker() +
                  ") found more than two times in preprocessed output."),
            metashell_instance().command("#msh pp " + marker()).front());
}

TEST(pp, marker_defined)
{
  metashell_instance mi;
  mi.command("#define " + macro_in_marker() + " foo");

  ASSERT_EQ(error("Marker (" + marker() +
                  ") not found in preprocessed output."
                  " Does it contain a macro that has been defined?"),
            mi.command("#msh pp " + marker()).front());
}

TEST(pp, preprocessor_directive)
{
  ASSERT_EQ(cpp_code("#error foo"),
            metashell_instance().command("#msh pp #error foo").front());
}
