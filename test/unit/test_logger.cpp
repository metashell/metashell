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

#include <metashell/in_memory_displayer.hpp>
#include <metashell/logger.hpp>
#include <metashell/null_displayer.hpp>

#include "mock_file_writer.hpp"

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <string>

using namespace metashell;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;

TEST(logger, logging_is_disabled_by_default)
{
  null_displayer d;
  mock_file_writer w;
  logger l(d, w);

  ASSERT_FALSE(l.logging());
}

TEST(logger, logging_is_enabled_when_logging_to_console)
{
  null_displayer d;
  NiceMock<mock_file_writer> w;
  ON_CALL(w, is_open()).WillByDefault(Return(true));
  logger l(d, w);
  l.log_to_console();

  ASSERT_TRUE(l.logging());
}

TEST(logger, logging_is_disabled_after_logging_is_stopped)
{
  null_displayer d;
  NiceMock<mock_file_writer> w;
  ON_CALL(w, is_open()).WillByDefault(Return(true));
  logger l(d, w);
  l.log_to_console();
  l.stop_logging();

  ASSERT_FALSE(l.logging());
}

TEST(logger, log_is_displayed_on_console_when_logging_to_console)
{
  in_memory_displayer d;
  NiceMock<mock_file_writer> w;
  ON_CALL(w, is_open()).WillByDefault(Return(true));
  logger l(d, w);
  l.log_to_console();

  l.log("foo bar");

  ASSERT_EQ(std::vector<data::text>{data::text("Log: foo bar")}, d.comments());
}

TEST(logger, log_into_file_tries_to_open_the_file)
{
  null_displayer d;
  NiceMock<mock_file_writer> w;

  ON_CALL(w, is_open()).WillByDefault(Return(true));
  EXPECT_CALL(w, open("/tmp/foo.txt")).WillOnce(Return(true));

  logger l(d, w);
  l.log_into_file("/tmp/foo.txt");

  ASSERT_TRUE(l.logging());
  ASSERT_EQ(data::logging_mode::file, l.mode());
}

TEST(logger, failure_when_opening_log_file)
{
  null_displayer d;
  NiceMock<mock_file_writer> w;

  ON_CALL(w, is_open()).WillByDefault(Return(true));
  EXPECT_CALL(w, open(_)).WillOnce(Return(false));

  logger l(d, w);
  l.log_into_file("/tmp/foo.txt");

  ASSERT_FALSE(l.logging());
  ASSERT_EQ(data::logging_mode::none, l.mode());
}

TEST(logger, logging_into_a_different_file)
{
  null_displayer d;
  NiceMock<mock_file_writer> w;

  EXPECT_CALL(w, open(_)).WillOnce(Return(true)).WillOnce(Return(true));
  ON_CALL(w, is_open()).WillByDefault(Return(true));

  logger l(d, w);
  l.log_into_file("/tmp/foo.txt");

  EXPECT_CALL(w, close());
  l.log_into_file("/tmp/bar.txt");
}

TEST(
    logger,
    logging_is_disabled_when_trying_to_log_into_a_different_file_but_fails_to_open_it)
{
  null_displayer d;
  NiceMock<mock_file_writer> w;

  ON_CALL(w, is_open()).WillByDefault(Return(true));
  EXPECT_CALL(w, open(_)).WillOnce(Return(true));

  logger l(d, w);
  l.log_into_file("/tmp/foo.txt");

  EXPECT_CALL(w, open(_)).WillOnce(Return(false));

  l.log_into_file("/tmp/bar.txt");

  ASSERT_FALSE(l.logging());
  ASSERT_EQ(data::logging_mode::none, l.mode());
}

TEST(logger, log_file_is_closed_when_starting_to_log_to_console)
{
  null_displayer d;
  NiceMock<mock_file_writer> w;

  EXPECT_CALL(w, open(_)).WillOnce(Return(true));
  ON_CALL(w, is_open()).WillByDefault(Return(true));

  logger l(d, w);
  l.log_into_file("/tmp/foo.txt");

  EXPECT_CALL(w, close());
  l.log_to_console();
}

TEST(logger, log_file_is_closed_when_logging_is_stopped)
{
  null_displayer d;
  NiceMock<mock_file_writer> w;

  EXPECT_CALL(w, open(_)).WillOnce(Return(true));
  ON_CALL(w, is_open()).WillByDefault(Return(true));

  logger l(d, w);
  l.log_into_file("/tmp/foo.txt");

  EXPECT_CALL(w, close());
  l.stop_logging();
}

TEST(logger, log_is_written_to_file)
{
  null_displayer d;
  NiceMock<mock_file_writer> w;

  EXPECT_CALL(w, open(_)).WillOnce(Return(true));
  ON_CALL(w, is_open()).WillByDefault(Return(true));
  EXPECT_CALL(w, write("foo\n")).WillOnce(Return(true));

  logger l(d, w);
  l.log_into_file("/tmp/foo.txt");
  l.log("foo");
}
