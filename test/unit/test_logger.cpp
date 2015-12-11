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

#include <metashell/logger.hpp>
#include <metashell/null_displayer.hpp>
#include <metashell/in_memory_displayer.hpp>

#include "mock_file_writer.hpp"

#include <just/test.hpp>

#include <string>
#include <sstream>
#include <iostream>

using namespace metashell;

JUST_TEST_CASE(test_loggers_logging_is_disabled_by_default)
{
  null_displayer d;
  mock_file_writer w;
  logger l(d, w);

  JUST_ASSERT(!l.logging());
}

JUST_TEST_CASE(test_logging_is_enabled_when_logging_to_console)
{
  null_displayer d;
  mock_file_writer w;
  logger l(d, w);
  l.log_to_console();

  JUST_ASSERT(l.logging());
}

JUST_TEST_CASE(test_logging_is_disabled_after_logging_is_stopped)
{
  null_displayer d;
  mock_file_writer w;
  logger l(d, w);
  l.log_to_console();
  l.stop_logging();

  JUST_ASSERT(!l.logging());
}

JUST_TEST_CASE(test_log_is_displayed_on_console_when_logging_to_console)
{
  in_memory_displayer d;
  mock_file_writer w;
  logger l(d, w);
  l.log_to_console();

  l.log("foo bar");

  JUST_ASSERT_EQUAL_CONTAINER({data::text("Log: foo bar")}, d.comments());
}

JUST_TEST_CASE(test_log_into_file_tries_to_open_the_file)
{
  null_displayer d;
  mock_file_writer w;

  bool open_called = false;
  std::string fn;
  w.open_callback =
    [&open_called, &fn](const std::string& fn_)
    {
      open_called = true;
      fn = fn_;
      return true;
    };

  logger l(d, w);
  l.log_into_file("/tmp/foo.txt");

  JUST_ASSERT(open_called);
  JUST_ASSERT_EQUAL("/tmp/foo.txt", fn);
  JUST_ASSERT(l.logging());
  JUST_ASSERT_EQUAL(data::logging_mode::file, l.mode());
}

JUST_TEST_CASE(test_failure_when_opening_log_file)
{
  null_displayer d;
  mock_file_writer w;

  w.open_callback = [](const std::string&) { return false; };

  logger l(d, w);
  l.log_into_file("/tmp/foo.txt");

  JUST_ASSERT(!l.logging());
  JUST_ASSERT_EQUAL(data::logging_mode::none, l.mode());
}

JUST_TEST_CASE(test_logging_into_a_different_file)
{
  null_displayer d;
  mock_file_writer w;

  w.open_callback = [](const std::string&) { return true; };
  w.is_open_callback = [] { return true; };

  logger l(d, w);
  l.log_into_file("/tmp/foo.txt");

  bool close_called = false;
  w.close_callback = [&close_called] { close_called = true; };

  l.log_into_file("/tmp/bar.txt");

  JUST_ASSERT(close_called);
}

JUST_TEST_CASE(
  test_logging_is_disabled_when_trying_to_log_into_a_different_file_but_fails_to_open_it
)
{
  null_displayer d;
  mock_file_writer w;

  w.open_callback = [](const std::string&) { return true; };

  logger l(d, w);
  l.log_into_file("/tmp/foo.txt");

  w.open_callback = [](const std::string&) { return false; };

  l.log_into_file("/tmp/bar.txt");

  JUST_ASSERT(!l.logging());
  JUST_ASSERT_EQUAL(data::logging_mode::none, l.mode());
}

JUST_TEST_CASE(test_log_file_is_closed_when_starting_to_log_to_console)
{
  null_displayer d;
  mock_file_writer w;

  w.open_callback = [](const std::string&) { return true; };
  w.is_open_callback = [] { return true; };

  logger l(d, w);
  l.log_into_file("/tmp/foo.txt");

  bool close_called = false;
  w.close_callback = [&close_called] { close_called = true; };

  l.log_to_console();

  JUST_ASSERT(close_called);
}

JUST_TEST_CASE(test_log_file_is_closed_when_logging_is_stopped)
{
  null_displayer d;
  mock_file_writer w;

  w.open_callback = [](const std::string&) { return true; };
  w.is_open_callback = [] { return true; };

  logger l(d, w);
  l.log_into_file("/tmp/foo.txt");

  bool close_called = false;
  w.close_callback = [&close_called] { close_called = true; };

  l.stop_logging();

  JUST_ASSERT(close_called);
}

JUST_TEST_CASE(test_log_is_written_to_file)
{
  null_displayer d;
  mock_file_writer w;

  std::ostringstream file_content;

  w.open_callback = [](const std::string&) { return true; };
  w.is_open_callback = [] { return true; };
  w.write_callback =
    [&file_content](const std::string& msg_)
    {
      file_content << msg_;
      return true;
    };

  logger l(d, w);
  l.log_into_file("/tmp/foo.txt");
  l.log("foo");

  JUST_ASSERT_EQUAL("foo\n", file_content.str());
}

