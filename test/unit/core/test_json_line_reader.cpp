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

#include <metashell/core/command_processor_queue.hpp>
#include <metashell/core/in_memory_displayer.hpp>
#include <metashell/core/json_line_reader.hpp>
#include <metashell/core/null_displayer.hpp>
#include <metashell/core/null_json_writer.hpp>

#include <metashell/mock/command_processor.hpp>
#include <metashell/mock/json_writer.hpp>

#include "string_reader.hpp"

#include <gtest/gtest.h>

using namespace metashell;

using ::testing::Return;

TEST(json_line_reader, end_of_input)
{
  core::null_json_writer jw;
  core::null_displayer d;
  core::command_processor_queue cpq;
  const data::line_reader r =
      build_json_line_reader(string_reader{}, d, jw, cpq);

  ASSERT_TRUE(std::nullopt == r(">"));
}

TEST(json_line_reader, empty_json)
{
  core::null_json_writer jw;
  core::null_displayer d;
  core::command_processor_queue cpq;
  const data::line_reader r =
      build_json_line_reader(string_reader{data::user_input()}, d, jw, cpq);

  const std::optional<data::user_input> l = r(">");

  ASSERT_TRUE(std::nullopt != l);
  ASSERT_EQ(data::user_input(), *l);
}

TEST(json_line_reader, getting_line)
{
  core::null_json_writer jw;
  core::null_displayer d;
  core::command_processor_queue cpq;
  const data::line_reader r = build_json_line_reader(
      string_reader{data::user_input("{\"type\":\"cmd\",\"cmd\":\"int\"}")}, d,
      jw, cpq);

  const std::optional<data::user_input> l = r(">");

  ASSERT_TRUE(std::nullopt != l);
  ASSERT_EQ(data::user_input("int"), *l);
}

TEST(json_line_reader, rejected_json_is_skipped)
{
  core::null_json_writer jw;
  core::null_displayer d;
  core::command_processor_queue cpq;
  const data::line_reader r = build_json_line_reader(
      string_reader{data::user_input("\"invalid_json\""),
                    data::user_input("{\"type\":\"cmd\",\"cmd\":\"int\"}")},
      d, jw, cpq);

  const std::optional<data::user_input> l = r(">");

  ASSERT_TRUE(std::nullopt != l);
  ASSERT_EQ(data::user_input("int"), *l);
}

TEST(json_line_reader, command_without_type)
{
  core::null_json_writer jw;
  core::in_memory_displayer d;
  core::command_processor_queue cpq;
  const data::line_reader r = build_json_line_reader(
      string_reader{data::user_input("{}"),
                    data::user_input("{\"type\":\"cmd\",\"cmd\":\"int\"}")},
      d, jw, cpq);

  const std::optional<data::user_input> l = r(">");

  // generates an error
  ASSERT_EQ(std::vector<std::string>{"Command without a type: {}"}, d.errors());

  // skipped
  ASSERT_TRUE(bool(l));
  ASSERT_EQ(data::user_input("int"), *l);
}

TEST(json_line_reader, command_of_unknown_type)
{
  core::null_json_writer jw;
  core::in_memory_displayer d;
  core::command_processor_queue cpq;
  const data::line_reader r = build_json_line_reader(
      string_reader{data::user_input("{\"type\":\"some unknown type\"}"),
                    data::user_input("{\"type\":\"cmd\",\"cmd\":\"int\"}")},
      d, jw, cpq);

  const std::optional<data::user_input> l = r(">");

  // generates an error
  ASSERT_EQ(std::vector<std::string>{"Unknown command type: some unknown type"},
            d.errors());

  // skipped
  ASSERT_TRUE(std::nullopt != l);
  ASSERT_EQ(data::user_input("int"), *l);
}

TEST(json_line_reader, cmd_command_without_cmd_field)
{
  core::null_json_writer jw;
  core::in_memory_displayer d;
  core::command_processor_queue cpq;
  const data::line_reader r = build_json_line_reader(
      string_reader{data::user_input("{\"type\":\"cmd\"}"),
                    data::user_input("{\"type\":\"cmd\",\"cmd\":\"int\"}")},
      d, jw, cpq);

  const std::optional<data::user_input> l = r(">");

  // generates an error
  ASSERT_EQ(
      std::vector<std::string>{"The cmd field of the cmd command is missing"},
      d.errors());

  // skipped
  ASSERT_TRUE(std::nullopt != l);
  ASSERT_EQ(data::user_input("int"), *l);
}

TEST(json_line_reader, displays_prompt)
{
  mock::json_writer w;
  core::null_displayer d;
  core::command_processor_queue cpq;

  const data::line_reader r =
      build_json_line_reader(string_reader{}, d, w, cpq);

  {
    ::testing::InSequence s;

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("type"));
    EXPECT_CALL(w, string("prompt"));
    EXPECT_CALL(w, key("prompt"));
    EXPECT_CALL(w, string(">"));
    EXPECT_CALL(w, end_object());
    EXPECT_CALL(w, end_document());
  }

  r(">");
}

TEST(json_line_reader, code_completion_without_code)
{
  core::null_json_writer jw;
  core::in_memory_displayer d;
  core::command_processor_queue cpq;
  const data::line_reader r = build_json_line_reader(
      string_reader{data::user_input("{\"type\":\"code_completion\"}"),
                    data::user_input("{\"type\":\"cmd\",\"cmd\":\"int\"}")},
      d, jw, cpq);

  const std::optional<data::user_input> l = r(">");

  // generates an error
  ASSERT_EQ(
      std::vector<std::string>{
          "The code field of the code_completion command is missing"},
      d.errors());

  // skipped
  ASSERT_TRUE(std::nullopt != l);
  ASSERT_EQ(data::user_input("int"), *l);
}

TEST(json_line_reader, code_completion_gets_code_completion)
{
  core::null_json_writer jw;
  core::null_displayer d;

  mock::command_processor* cp =
      new ::testing::StrictMock<mock::command_processor>;
  EXPECT_CALL(*cp, code_complete(data::user_input("foo"), true));

  core::command_processor_queue cpq;
  cpq.push(std::unique_ptr<iface::command_processor>(cp));

  const data::line_reader r = build_json_line_reader(
      string_reader{
          data::user_input("{\"type\":\"code_completion\",\"code\":\"foo\"}")},
      d, jw, cpq);

  r(">");
}

TEST(json_line_reader, code_completion_result)
{
  mock::json_writer w;
  core::null_displayer d;

  mock::command_processor* cp = new mock::command_processor;
  EXPECT_CALL(*cp, code_complete(data::user_input("foo"), true))
      .WillOnce(Return(data::code_completion{
          data::user_input("hello"), data::user_input("world")}));

  core::command_processor_queue cpq;
  cpq.push(std::unique_ptr<iface::command_processor>(cp));

  const data::line_reader r = build_json_line_reader(
      string_reader{
          data::user_input("{\"type\":\"code_completion\",\"code\":\"foo\"}")},
      d, w, cpq);

  {
    ::testing::InSequence s;

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("type"));
    EXPECT_CALL(w, string("prompt"));
    EXPECT_CALL(w, key("prompt"));
    EXPECT_CALL(w, string(">"));
    EXPECT_CALL(w, end_object());
    EXPECT_CALL(w, end_document());

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("type"));
    EXPECT_CALL(w, string("code_completion_result"));
    EXPECT_CALL(w, key("completions"));
    EXPECT_CALL(w, start_array());
    EXPECT_CALL(w, string("hello"));
    EXPECT_CALL(w, string("world"));
    EXPECT_CALL(w, end_array());
    EXPECT_CALL(w, end_object());
    EXPECT_CALL(w, end_document());

    EXPECT_CALL(w, start_object());
    EXPECT_CALL(w, key("type"));
    EXPECT_CALL(w, string("prompt"));
    EXPECT_CALL(w, key("prompt"));
    EXPECT_CALL(w, string(">"));
    EXPECT_CALL(w, end_object());
    EXPECT_CALL(w, end_document());
  }

  r(">");
}
