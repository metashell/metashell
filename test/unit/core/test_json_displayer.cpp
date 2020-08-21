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

#include <metashell/mock/json_writer.hpp>

#include <metashell/core/json_displayer.hpp>

#include <gtest/gtest.h>

#include <vector>

using namespace metashell;

TEST(json_displayer, raw_text)
{
  mock::json_writer w;
  core::json_displayer d(w);

  {
    ::testing::InSequence s;

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("type"));
    EXPECT_CALL(w, string("raw_text"));
    EXPECT_CALL(w, key("value"));
    EXPECT_CALL(w, string("foo"));
    EXPECT_CALL(w, end_object());
    EXPECT_CALL(w, end_document());
  }

  d.show_raw_text("foo");
}

TEST(json_displayer, error)
{
  mock::json_writer w;
  core::json_displayer d(w);

  {
    ::testing::InSequence s;

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("type"));
    EXPECT_CALL(w, string("error"));
    EXPECT_CALL(w, key("msg"));
    EXPECT_CALL(w, string("foo"));
    EXPECT_CALL(w, end_object());
    EXPECT_CALL(w, end_document());
  }

  d.show_error("foo");
}

TEST(json_displayer, type)
{
  mock::json_writer w;
  core::json_displayer d(w);

  {
    ::testing::InSequence s;

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("type"));
    EXPECT_CALL(w, string("type"));
    EXPECT_CALL(w, key("name"));
    EXPECT_CALL(w, string("const fib_c<3>"));
    EXPECT_CALL(w, end_object());
    EXPECT_CALL(w, end_document());
  }

  d.show_type(data::type("const fib_c<3>"));
}

TEST(json_displayer, empty_comment)
{
  mock::json_writer w;
  core::json_displayer d(w);

  {
    ::testing::InSequence s;

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("type"));
    EXPECT_CALL(w, string("comment"));
    EXPECT_CALL(w, key("paragraphs"));
    EXPECT_CALL(w, start_array());
    EXPECT_CALL(w, end_array());
    EXPECT_CALL(w, end_object());
    EXPECT_CALL(w, end_document());
  }

  d.show_comment({});
}

TEST(json_displayer, comment_with_one_paragraph)
{
  mock::json_writer w;
  core::json_displayer d(w);

  {
    ::testing::InSequence s;

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("type"));
    EXPECT_CALL(w, string("comment"));
    EXPECT_CALL(w, key("paragraphs"));
    EXPECT_CALL(w, start_array());
    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("first_line_indentation"));
    EXPECT_CALL(w, string("y"));
    EXPECT_CALL(w, key("rest_of_lines_indentation"));
    EXPECT_CALL(w, string("x"));
    EXPECT_CALL(w, key("content"));
    EXPECT_CALL(w, string("foo bar"));
    EXPECT_CALL(w, end_object());
    EXPECT_CALL(w, end_array());
    EXPECT_CALL(w, end_object());
    EXPECT_CALL(w, end_document());
  }

  d.show_comment(data::text{data::paragraph("foo bar", "x", "y")});
}

TEST(json_displayer, cpp_code)
{
  mock::json_writer w;
  core::json_displayer d(w);

  {
    ::testing::InSequence s;

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("type"));
    EXPECT_CALL(w, string("cpp_code"));
    EXPECT_CALL(w, key("code"));
    EXPECT_CALL(w, string("int main() {}"));
    EXPECT_CALL(w, end_object());
    EXPECT_CALL(w, end_document());
  }

  d.show_cpp_code(data::cpp_code("int main() {}"));
}

TEST(json_displayer, frame_normal)
{
  mock::json_writer w;
  core::json_displayer d(w);

  {
    ::testing::InSequence s;

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("type"));
    EXPECT_CALL(w, string("frame"));
    EXPECT_CALL(w, key("name"));
    EXPECT_CALL(w, string("fib_c<13>::type"));
    EXPECT_CALL(w, key("source_location"));
    EXPECT_CALL(w, string("a.hpp:10:20"));
    EXPECT_CALL(w, end_object());
    EXPECT_CALL(w, end_document());
  }

  d.show_frame(data::frame(true, std::nullopt, data::type("fib_c<13>::type"),
                           data::file_location("a.hpp", 10, 20)));
}

namespace
{
  void test_frame_full(data::event_kind kind_, const std::string& kind_in_json_)
  {
    using metashell::data::file_location;

    mock::json_writer w;
    core::json_displayer d(w);

    const data::file_location source_location("sl.hpp", 10, 20);
    const data::file_location point_of_event("pof.cpp", 20, 30);

    {
      ::testing::InSequence s;

      EXPECT_CALL(w, start_object());
      EXPECT_CALL(w, key("type"));
      EXPECT_CALL(w, string("frame"));
      EXPECT_CALL(w, key("name"));
      EXPECT_CALL(w, string("fib_c<13>::type"));
      EXPECT_CALL(w, key("source_location"));
      EXPECT_CALL(w, string("sl.hpp:10:20"));
      EXPECT_CALL(w, key("kind"));
      EXPECT_CALL(w, string(kind_in_json_));
      EXPECT_CALL(w, key("point_of_event"));
      EXPECT_CALL(w, string("pof.cpp:20:30"));
      EXPECT_CALL(w, end_object());
      EXPECT_CALL(w, end_document());
    }

    d.show_frame(data::frame(true, std::nullopt, data::type("fib_c<13>::type"),
                             source_location, point_of_event, kind_));
  }
}

TEST(json_displayer, frame_full)
{
  test_frame_full(
      data::event_kind::template_instantiation, "TemplateInstantiation");
  test_frame_full(data::event_kind::default_template_argument_instantiation,
                  "DefaultTemplateArgumentInstantiation");
  test_frame_full(data::event_kind::default_function_argument_instantiation,
                  "DefaultFunctionArgumentInstantiation");
  test_frame_full(data::event_kind::explicit_template_argument_substitution,
                  "ExplicitTemplateArgumentSubstitution");
  test_frame_full(data::event_kind::deduced_template_argument_substitution,
                  "DeducedTemplateArgumentSubstitution");
  test_frame_full(data::event_kind::prior_template_argument_substitution,
                  "PriorTemplateArgumentSubstitution");
  test_frame_full(data::event_kind::default_template_argument_checking,
                  "DefaultTemplateArgumentChecking");
  test_frame_full(data::event_kind::exception_spec_instantiation,
                  "ExceptionSpecInstantiation");
  test_frame_full(data::event_kind::memoization, "Memoization");
  test_frame_full(data::event_kind::non_template_type, "NonTemplateType");
}

TEST(json_displayer, backtrace)
{
  mock::json_writer w;
  core::json_displayer d(w);

  {
    ::testing::InSequence s;

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("type"));
    EXPECT_CALL(w, string("backtrace"));
    EXPECT_CALL(w, key("frames"));
    EXPECT_CALL(w, start_array());

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("name"));
    EXPECT_CALL(w, string("fib_c<13>::type"));
    EXPECT_CALL(w, key("source_location"));
    EXPECT_CALL(w, string("sl.hpp:134:10"));
    EXPECT_CALL(w, end_object());

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("name"));
    EXPECT_CALL(w, string("fib<int_<13>>::type"));
    EXPECT_CALL(w, key("source_location"));
    EXPECT_CALL(w, string("sl2.hpp:154:10"));
    EXPECT_CALL(w, end_object());

    EXPECT_CALL(w, end_array());
    EXPECT_CALL(w, end_object());
    EXPECT_CALL(w, end_document());
  }

  d.show_backtrace(data::backtrace{
      data::frame(true, std::nullopt, data::type("fib_c<13>::type"),
                  data::file_location("sl.hpp", 134, 10)),
      data::frame(true, std::nullopt, data::type("fib<int_<13>>::type"),
                  data::file_location("sl2.hpp", 154, 10))});
}

TEST(json_displayer, call_graph)
{
  mock::json_writer w;
  core::json_displayer d(w);

  const data::type int_("int");

  const std::vector<data::call_graph_node> cg{
      {data::frame(true, std::nullopt, int_, data::file_location{}), 0, 1},
      {data::frame(true, std::nullopt, int_, data::file_location{}), 1, 0}};

  {
    ::testing::InSequence s;

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("type"));
    EXPECT_CALL(w, string("call_graph"));
    EXPECT_CALL(w, key("nodes"));
    EXPECT_CALL(w, start_array());

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("name"));
    EXPECT_CALL(w, string("int"));
    EXPECT_CALL(w, key("source_location"));
    EXPECT_CALL(w, string(":-1:-1"));
    EXPECT_CALL(w, key("depth"));
    EXPECT_CALL(w, int_(0));
    EXPECT_CALL(w, key("children"));
    EXPECT_CALL(w, int_(1));
    EXPECT_CALL(w, end_object());

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("name"));
    EXPECT_CALL(w, string("int"));
    EXPECT_CALL(w, key("source_location"));
    EXPECT_CALL(w, string(":-1:-1"));
    EXPECT_CALL(w, key("depth"));
    EXPECT_CALL(w, int_(1));
    EXPECT_CALL(w, key("children"));
    EXPECT_CALL(w, int_(0));
    EXPECT_CALL(w, end_object());

    EXPECT_CALL(w, end_array());
    EXPECT_CALL(w, end_object());
    EXPECT_CALL(w, end_document());
  }

  d.show_call_graph(cg);
}
