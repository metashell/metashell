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

#include <metashell/json_line_reader.hpp>
#include <metashell/null_displayer.hpp>
#include <metashell/null_json_writer.hpp>
#include <metashell/in_memory_displayer.hpp>

#include "mock_json_writer.hpp"
#include "string_reader.hpp"

#include <just/test.hpp>

using namespace metashell;

JUST_TEST_CASE(test_end_of_input_with_json_line_reader)
{
  null_json_writer jw;
  null_displayer d;
  const line_reader r = build_json_line_reader(string_reader{}, d, jw);

  JUST_ASSERT(boost::none == r(">"));
}

JUST_TEST_CASE(test_reading_empty_json)
{
  null_json_writer jw;
  null_displayer d;
  const line_reader r = build_json_line_reader(string_reader{""}, d, jw);

  const boost::optional<std::string> l = r(">");

  JUST_ASSERT(boost::none != l);
  JUST_ASSERT_EQUAL("", *l);
}

JUST_TEST_CASE(test_getting_line_with_json_line_reader)
{
  null_json_writer jw;
  null_displayer d;
  const line_reader r =
    build_json_line_reader(
      string_reader{"{\"type\":\"cmd\",\"cmd\":\"int\"}"},
      d,
      jw
    );

  const boost::optional<std::string> l = r(">");

  JUST_ASSERT(boost::none != l);
  JUST_ASSERT_EQUAL("int", *l);
}

JUST_TEST_CASE(test_rejected_json_is_skipped)
{
  null_json_writer jw;
  null_displayer d;
  const line_reader r =
    build_json_line_reader(
      string_reader{"\"invalid_json\"", "{\"type\":\"cmd\",\"cmd\":\"int\"}"},
      d,
      jw
    );

  const boost::optional<std::string> l = r(">");

  JUST_ASSERT(boost::none != l);
  JUST_ASSERT_EQUAL("int", *l);
}

JUST_TEST_CASE(test_command_without_type)
{
  null_json_writer jw;
  in_memory_displayer d;
  const line_reader r =
    build_json_line_reader(
      string_reader{"{}", "{\"type\":\"cmd\",\"cmd\":\"int\"}"},
      d,
      jw
    );

  const boost::optional<std::string> l = r(">");

  // generates an error
  JUST_ASSERT_EQUAL_CONTAINER({"Command without a type: {}"}, d.errors());

  // skipped
  JUST_ASSERT(boost::none != l);
  JUST_ASSERT_EQUAL("int", *l);
}

JUST_TEST_CASE(test_command_of_unknown_type)
{
  null_json_writer jw;
  in_memory_displayer d;
  const line_reader r =
    build_json_line_reader(
      string_reader{
        "{\"type\":\"some unknown type\"}",
        "{\"type\":\"cmd\",\"cmd\":\"int\"}"
      },
      d,
      jw
    );

  const boost::optional<std::string> l = r(">");

  // generates an error
  JUST_ASSERT_EQUAL_CONTAINER(
    {"Unknown command type: some unknown type"},
    d.errors()
  );

  // skipped
  JUST_ASSERT(boost::none != l);
  JUST_ASSERT_EQUAL("int", *l);
}

JUST_TEST_CASE(test_cmd_command_without_cmd_field)
{
  null_json_writer jw;
  in_memory_displayer d;
  const line_reader r =
    build_json_line_reader(
      string_reader{"{\"type\":\"cmd\"}", "{\"type\":\"cmd\",\"cmd\":\"int\"}"},
      d,
      jw
    );

  const boost::optional<std::string> l = r(">");

  // generates an error
  JUST_ASSERT_EQUAL_CONTAINER(
    {"The cmd field of the cmd command is missing"},
    d.errors()
  );

  // skipped
  JUST_ASSERT(boost::none != l);
  JUST_ASSERT_EQUAL("int", *l);
}

JUST_TEST_CASE(test_json_line_reader_displays_prompt)
{
  mock_json_writer jw;
  null_displayer d;

  const line_reader r = build_json_line_reader(string_reader{}, d, jw);

  r(">");

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      "start_object",
        "key type", "string prompt",
        "key prompt", "string >",
      "end_object",
      "end_document"
    },
    jw.calls()
  );
}

