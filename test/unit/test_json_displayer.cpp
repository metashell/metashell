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

#include <metashell/json_displayer.hpp>
#include "mock_json_writer.hpp"

#include <just/test.hpp>

#include <vector>

using namespace metashell;

JUST_TEST_CASE(test_json_display_of_raw_text)
{
  mock_json_writer w;
  json_displayer d(w);

  d.show_raw_text("foo");

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      "start_object",
        "key type", "string raw_text",
        "key value", "string foo",
      "end_object",
      "end_document"
    },
    w.calls()
  );
}

JUST_TEST_CASE(test_json_display_of_error)
{
  mock_json_writer w;
  json_displayer d(w);

  d.show_error("foo");

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      "start_object",
        "key type", "string error",
        "key msg", "string foo",
      "end_object",
      "end_document"
    },
    w.calls()
  );
}

JUST_TEST_CASE(test_json_display_of_type)
{
  mock_json_writer w;
  json_displayer d(w);

  d.show_type(data::type("const fib_c<3>"));

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      "start_object",
        "key type", "string type",
        "key name", "string const fib_c<3>",
      "end_object",
      "end_document"
    },
    w.calls()
  );
}

JUST_TEST_CASE(test_json_display_of_empty_comment)
{
  mock_json_writer w;
  json_displayer d(w);

  d.show_comment({});

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      "start_object",
        "key type", "string comment",
        "key paragraphs",
          "start_array",
          "end_array",
      "end_object",
      "end_document"
    },
    w.calls()
  );
}

JUST_TEST_CASE(test_json_display_of_comment_with_one_paragraph)
{
  mock_json_writer w;
  json_displayer d(w);

  d.show_comment(data::text{data::paragraph("foo bar", "x", "y")});

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      "start_object",
        "key type", "string comment",
        "key paragraphs",
          "start_array",
            "start_object",
              "key first_line_indentation", "string y",
              "key rest_of_lines_indentation", "string x",
              "key content", "string foo bar",
            "end_object",
          "end_array",
      "end_object",
      "end_document"
    },
    w.calls()
  );
}

JUST_TEST_CASE(test_json_display_of_cpp_code)
{
  mock_json_writer w;
  json_displayer d(w);

  d.show_cpp_code("int main() {}");

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      "start_object",
        "key type", "string cpp_code",
        "key code", "string int main() {}",
      "end_object",
      "end_document"
    },
    w.calls()
  );
}

JUST_TEST_CASE(test_json_display_of_frame_normal)
{
  mock_json_writer w;
  json_displayer d(w);

  d.show_frame(
    data::frame(data::type("fib_c<13>::type"),
      data::file_location("a.hpp", 10, 20)));

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      "start_object",
        "key type", "string frame",
        "key name", "string fib_c<13>::type",
        "key source_location", "string a.hpp:10:20",
      "end_object",
      "end_document"
    },
    w.calls()
  );
}

namespace
{
  void test_frame_full(
    data::instantiation_kind kind_,
    const std::string& kind_in_json_
  )
  {
    using metashell::data::file_location;

    mock_json_writer w;
    json_displayer d(w);

    data::file_location source_location("sl.hpp", 10, 20);
    data::file_location point_of_instantiation("pof.cpp", 20, 30);

    d.show_frame(data::frame(
        data::type("fib_c<13>::type"),
        source_location,
        point_of_instantiation,
        kind_));

    JUST_ASSERT_EQUAL_CONTAINER(
      {
        "start_object",
          "key type", "string frame",
          "key name", "string fib_c<13>::type",
          "key source_location", "string sl.hpp:10:20",
          "key kind", "string " + kind_in_json_,
          "key point_of_instantiation", "string pof.cpp:20:30",
        "end_object",
        "end_document"
      },
      w.calls()
    );
  }
}

JUST_TEST_CASE(test_json_display_of_frame_full)
{
  test_frame_full(
    data::instantiation_kind::template_instantiation,
    "TemplateInstantiation"
  );
  test_frame_full(
    data::instantiation_kind::default_template_argument_instantiation,
    "DefaultTemplateArgumentInstantiation"
  );
  test_frame_full(
    data::instantiation_kind::default_function_argument_instantiation,
    "DefaultFunctionArgumentInstantiation"
  );
  test_frame_full(
    data::instantiation_kind::explicit_template_argument_substitution,
    "ExplicitTemplateArgumentSubstitution"
  );
  test_frame_full(
    data::instantiation_kind::deduced_template_argument_substitution,
    "DeducedTemplateArgumentSubstitution"
  );
  test_frame_full(
    data::instantiation_kind::prior_template_argument_substitution,
    "PriorTemplateArgumentSubstitution"
  );
  test_frame_full(
    data::instantiation_kind::default_template_argument_checking,
    "DefaultTemplateArgumentChecking"
  );
  test_frame_full(
    data::instantiation_kind::exception_spec_instantiation,
    "ExceptionSpecInstantiation"
  );
  test_frame_full(
    data::instantiation_kind::memoization,
    "Memoization"
  );
  test_frame_full(
    data::instantiation_kind::non_template_type,
    "NonTemplateType"
  );
}

JUST_TEST_CASE(test_json_display_of_backtrace)
{
  mock_json_writer w;
  json_displayer d(w);

  d.show_backtrace(
    data::backtrace{
      data::frame(
        data::type("fib_c<13>::type"),
        data::file_location("sl.hpp", 134, 10)
      ),
      data::frame(
        data::type("fib<int_<13>>::type"),
        data::file_location("sl2.hpp", 154, 10)
      )
    }
  );

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      "start_object",
        "key type", "string backtrace",
        "key frames",
          "start_array",
            "start_object",
              "key name", "string fib_c<13>::type",
              "key source_location", "string sl.hpp:134:10",
            "end_object",
            "start_object",
              "key name", "string fib<int_<13>>::type",
              "key source_location", "string sl2.hpp:154:10",
            "end_object",
          "end_array",
      "end_object",
      "end_document"
    },
    w.calls()
  );
}

JUST_TEST_CASE(test_json_display_of_call_graph)
{
  mock_json_writer w;
  json_displayer d(w);

  const data::type int_("int");

  const std::vector<data::call_graph_node>
    cg{
      {data::frame(int_, data::file_location{}), 0, 1},
      {data::frame(int_, data::file_location{}), 1, 0}
    };

  d.show_call_graph(cg);

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      "start_object",
        "key type", "string call_graph",
        "key nodes",
          "start_array",
            "start_object",
              "key name", "string int",
              "key source_location", "string :-1:-1",
              "key depth", "int 0",
              "key children", "int 1",
            "end_object",
            "start_object",
              "key name", "string int",
              "key source_location", "string :-1:-1",
              "key depth", "int 1",
              "key children", "int 0",
            "end_object",
          "end_array",
      "end_object",
      "end_document"
    },
    w.calls()
  );
}

