// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell_system_test/prompt.hpp>
#include <metashell_system_test/type.hpp>

#include <metashell_system_test/json_generator.hpp>
#include <metashell_system_test/run_metashell.hpp>
#include <metashell_system_test/path_builder.hpp>

#include <just/test.hpp>

using namespace metashell_system_test;

JUST_TEST_CASE(test_basic_formatting)
{
  const auto r =
    run_metashell(
      {
        command(
          "namespace metashell"
          "{"
            "template <>"
            "struct format<int>"
            "{"
              "typedef double type;"
            "};"
          "}"
        ),
        command("int")
      }
    );

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i); ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i); ++i;
  JUST_ASSERT_EQUAL(type("double"), *i); ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_tag_dispatched_formatting)
{
  const auto r =
    run_metashell(
      {
        command(
          "struct foo_tag {};"
    
          "struct foo"
          "{"
            "typedef foo_tag tag;"
          "};"
    
          "namespace metashell"
          "{"
            "template <>"
            "struct format_impl<foo_tag>"
            "{"
              "typedef format_impl type;"
    
              "template <class T>"
              "struct apply"
              "{"
                "typedef char type;"
              "};"
            "};"
          "}"
        ),
        command("foo")
      }
    );

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i); ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i); ++i;
  JUST_ASSERT_EQUAL(type("char"), *i); ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_formatting_disabled)
{
  const auto r =
    run_metashell(
      {
        command("template <class... Ts> struct template_with_a_long_name {};"),
        command("template_with_a_long_name<int, double, char>")
      }
    );

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i); ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i); ++i;

  JUST_ASSERT_EQUAL(type("template_with_a_long_name<int, double, char>"), *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_nested_mpl_vector_formatting)
{
  const std::string vector_hpp =
    path_builder() / "metashell" / "formatter" / "vector.hpp";

  const auto r =
    run_metashell(
      {
        command("#include <" + vector_hpp + ">"),
        command("boost::mpl::vector<boost::mpl::vector<int>>")
      }
    );

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i); ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i); ++i;

  JUST_ASSERT_EQUAL(type("boost_::mpl::vector<boost_::mpl::vector<int> >"), *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

