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

#include <metashell/core/console_displayer.hpp>

#include <metashell/mock/console.hpp>

#include "fib.hpp"

#include <gtest/gtest.h>

#include <vector>

using namespace metashell;
using ::testing::NiceMock;
using ::testing::Return;

namespace
{
  typedef std::vector<data::call_graph_node> call_grph;

  call_grph fib5_call_graph()
  {
    using data::event_kind;
    using data::type;
    using data::frame;
    using data::file_location;

    file_location f("a.cpp", 1, 2);
    file_location f2("b.hpp", 1, 2);

    return call_grph{
        {frame(false, std::nullopt, type("int_<fib<5>::value>"), f2), 0, 3},
        {frame(false, std::nullopt, fib<5>(), f2, f,
               event_kind::template_instantiation),
         1, 2},
        {frame(false, std::nullopt, fib<3>(), f2, f,
               event_kind::template_instantiation),
         2, 2},
        {frame(true, std::nullopt, fib<1>(), f2, f, event_kind::memoization), 3,
         0},
        {frame(false, std::nullopt, fib<2>(), f2, f,
               event_kind::template_instantiation),
         3, 2},
        {frame(true, std::nullopt, fib<0>(), f2, f, event_kind::memoization), 4,
         0},
        {frame(true, std::nullopt, fib<1>(), f2, f, event_kind::memoization), 4,
         0},
        {frame(false, std::nullopt, fib<4>(), f2, f,
               event_kind::template_instantiation),
         2, 2},
        {frame(true, std::nullopt, fib<2>(), f2, f, event_kind::memoization), 3,
         0},
        {frame(true, std::nullopt, fib<3>(), f2, f, event_kind::memoization), 3,
         0},
        {frame(true, std::nullopt, fib<5>(), f2, f, event_kind::memoization), 1,
         0},
        {frame(true, std::nullopt, type("int_<5>"), f2, f,
               event_kind::template_instantiation),
         1, 0}};
  }
}

TEST(console_displayer, nothing_is_displayed_by_default)
{
  ::testing::StrictMock<mock::console> c;
  core::console_displayer cd(c, false, false);
}

TEST(console_displayer, raw_text_is_printed)
{
  mock::console c;
  core::console_displayer cd(c, false, false);

  EXPECT_CALL(c, show(data::colored_string("Hello world!")));
  EXPECT_CALL(c, new_line());

  cd.show_raw_text("Hello world!");
}

TEST(console_displayer, raw_text_with_new_line_is_printed)
{
  mock::console c;
  core::console_displayer cd(c, false, false);

  EXPECT_CALL(c, show(data::colored_string("Hello\nworld!")));
  EXPECT_CALL(c, new_line());

  cd.show_raw_text("Hello\nworld!");
}

TEST(console_displayer, error_with_no_colors_is_printed)
{
  mock::console c;
  core::console_displayer cd(c, false, false);

  EXPECT_CALL(c, show(data::colored_string("Something went wrong")));
  EXPECT_CALL(c, new_line());

  cd.show_error("Something went wrong");
}

TEST(console_displayer, error_with_colors_is_printed_in_red)
{
  mock::console c;
  core::console_displayer cd(c, false, true);

  EXPECT_CALL(c, show(data::colored_string(
                     "Something went wrong", data::color::bright_red)));
  EXPECT_CALL(c, new_line());

  cd.show_error("Something went wrong");
}

TEST(console_displayer, mdb_forwardtrace_from_root_on_narrow_terminal)
{
  NiceMock<mock::console> c;

  ON_CALL(c, width()).WillByDefault(Return(25));
  ON_CALL(c, height()).WillByDefault(Return(100));

  core::console_displayer d(c, false, false);

  {
    ::testing::InSequence s;

    EXPECT_CALL(c, show(data::colored_string("int_<fib<5>::value>")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("fib<5> at b.hpp:1:2 (Te")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("mplateInstantiation fro")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("m a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("fib<3> at b.hpp:1:2 (")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("TemplateInstantiation")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string(" from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string("fib<1> at b.hpp:1:2")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string(" (Memoization from ")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string("a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("` ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string("fib<2> at b.hpp:1:2")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("  ")));
    EXPECT_CALL(c, show(data::colored_string(" (TemplateInstantia")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("  ")));
    EXPECT_CALL(c, show(data::colored_string("tion from a.cpp:1:2")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("  ")));
    EXPECT_CALL(c, show(data::colored_string(")")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::cyan)));
    EXPECT_CALL(c, show(data::colored_string("fib<0> at b.hpp:1")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::cyan)));
    EXPECT_CALL(c, show(data::colored_string(":2 (Memoization f")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::cyan)));
    EXPECT_CALL(c, show(data::colored_string("rom a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string("` ", data::color::cyan)));
    EXPECT_CALL(c, show(data::colored_string("fib<1> at b.hpp:1")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string("  ")));
    EXPECT_CALL(c, show(data::colored_string(":2 (Memoization f")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string("  ")));
    EXPECT_CALL(c, show(data::colored_string("rom a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("` ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("fib<4> at b.hpp:1:2 (")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("  ")));
    EXPECT_CALL(c, show(data::colored_string("TemplateInstantiation")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("  ")));
    EXPECT_CALL(c, show(data::colored_string(" from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string("fib<2> at b.hpp:1:2")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string(" (Memoization from ")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string("a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("` ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string("fib<3> at b.hpp:1:2")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("  ")));
    EXPECT_CALL(c, show(data::colored_string(" (Memoization from ")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("  ")));
    EXPECT_CALL(c, show(data::colored_string("a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("fib<5> at b.hpp:1:2 (Me")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("moization from a.cpp:1:")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("` ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("int_<5> at b.hpp:1:2 (T")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("  ")));
    EXPECT_CALL(c, show(data::colored_string("emplateInstantiation fr")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("  ")));
    EXPECT_CALL(c, show(data::colored_string("om a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
  }

  d.show_call_graph(fib5_call_graph());
}

TEST(console_displayer, mdb_forwardtrace_on_extremely_narrow_terminal_w0)
{
  NiceMock<mock::console> c;

  ON_CALL(c, width()).WillByDefault(Return(0));
  ON_CALL(c, height()).WillByDefault(Return(1000));

  core::console_displayer d(c, false, false);

  // The algorithm just gives up, and prints without extra line breaks
  {
    ::testing::InSequence s;

    EXPECT_CALL(c, show(data::colored_string("int_<fib<5>::value>")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::green)));
    EXPECT_CALL(
        c, show(data::colored_string(
               "fib<5> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::yellow)));
    EXPECT_CALL(
        c, show(data::colored_string(
               "fib<3> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string(
                       "fib<1> at b.hpp:1:2 (Memoization from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("` ", data::color::blue)));
    EXPECT_CALL(
        c, show(data::colored_string(
               "fib<2> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::cyan)));
    EXPECT_CALL(c, show(data::colored_string(
                       "fib<0> at b.hpp:1:2 (Memoization from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string("` ", data::color::cyan)));
    EXPECT_CALL(c, show(data::colored_string(
                       "fib<1> at b.hpp:1:2 (Memoization from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("` ", data::color::yellow)));
    EXPECT_CALL(
        c, show(data::colored_string(
               "fib<4> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string(
                       "fib<2> at b.hpp:1:2 (Memoization from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("` ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string(
                       "fib<3> at b.hpp:1:2 (Memoization from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string(
                       "fib<5> at b.hpp:1:2 (Memoization from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("` ", data::color::green)));
    EXPECT_CALL(
        c, show(data::colored_string(
               "int_<5> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
  }

  d.show_call_graph(fib5_call_graph());
}

TEST(console_displayer, mdb_forwardtrace_on_extremely_narrow_terminal_w1)
{
  NiceMock<mock::console> c;

  ON_CALL(c, width()).WillByDefault(Return(1));
  ON_CALL(c, height()).WillByDefault(Return(1000));

  core::console_displayer d(c, false, false);

  // The algorithm just gives up, and prints without extra line breaks
  {
    ::testing::InSequence s;

    EXPECT_CALL(c, show(data::colored_string("int_<fib<5>::value>")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::green)));
    EXPECT_CALL(
        c, show(data::colored_string(
               "fib<5> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::yellow)));
    EXPECT_CALL(
        c, show(data::colored_string(
               "fib<3> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string(
                       "fib<1> at b.hpp:1:2 (Memoization from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("` ", data::color::blue)));
    EXPECT_CALL(
        c, show(data::colored_string(
               "fib<2> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::cyan)));
    EXPECT_CALL(c, show(data::colored_string(
                       "fib<0> at b.hpp:1:2 (Memoization from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string("` ", data::color::cyan)));
    EXPECT_CALL(c, show(data::colored_string(
                       "fib<1> at b.hpp:1:2 (Memoization from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("` ", data::color::yellow)));
    EXPECT_CALL(
        c, show(data::colored_string(
               "fib<4> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string(
                       "fib<2> at b.hpp:1:2 (Memoization from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("| ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string("  ", data::color::yellow)));
    EXPECT_CALL(c, show(data::colored_string("` ", data::color::blue)));
    EXPECT_CALL(c, show(data::colored_string(
                       "fib<3> at b.hpp:1:2 (Memoization from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("+ ", data::color::green)));
    EXPECT_CALL(c, show(data::colored_string(
                       "fib<5> at b.hpp:1:2 (Memoization from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
    EXPECT_CALL(c, show(data::colored_string("` ", data::color::green)));
    EXPECT_CALL(
        c, show(data::colored_string(
               "int_<5> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)")));
    EXPECT_CALL(c, new_line());
  }

  d.show_call_graph(fib5_call_graph());
}

TEST(console_displayer, show_file_section_3_lines_1)
{
  NiceMock<mock::console> c;

  ON_CALL(c, width()).WillByDefault(Return(0));

  core::console_displayer d(c, false, false);

  {
    ::testing::InSequence s;

    EXPECT_CALL(c, show(data::colored_string("   1  ")));
    EXPECT_CALL(c, show(data::colored_string("first\n")));
    EXPECT_CALL(c, show(data::colored_string("-> 2  ")));
    EXPECT_CALL(c, show(data::colored_string("second\n")));
    EXPECT_CALL(c, show(data::colored_string("   3  ")));
    EXPECT_CALL(c, show(data::colored_string("third\n")));
  }

  d.show_file_section(
      data::file_location("<stdin>", 2, 0), "first\nsecond\nthird\n");
}

TEST(console_displayer, show_file_section_3_lines_2)
{
  NiceMock<mock::console> c;

  ON_CALL(c, width()).WillByDefault(Return(0));

  core::console_displayer d(c, false, false);

  {
    ::testing::InSequence s;

    EXPECT_CALL(c, show(data::colored_string("   1  ")));
    EXPECT_CALL(c, show(data::colored_string("first\n")));
    EXPECT_CALL(c, show(data::colored_string("   2  ")));
    EXPECT_CALL(c, show(data::colored_string("second\n")));
    EXPECT_CALL(c, show(data::colored_string("-> 3  ")));
    EXPECT_CALL(c, show(data::colored_string("third\n")));
  }

  d.show_file_section(
      data::file_location("<stdin>", 3, 0), "first\nsecond\nthird\n");
}

TEST(console_displayer, show_file_section_6_lines_1)
{
  NiceMock<mock::console> c;

  ON_CALL(c, width()).WillByDefault(Return(0));

  core::console_displayer d(c, false, false);

  {
    ::testing::InSequence s;

    EXPECT_CALL(c, show(data::colored_string("   1  ")));
    EXPECT_CALL(c, show(data::colored_string("first\n")));
    EXPECT_CALL(c, show(data::colored_string("   2  ")));
    EXPECT_CALL(c, show(data::colored_string("second\n")));
    EXPECT_CALL(c, show(data::colored_string("-> 3  ")));
    EXPECT_CALL(c, show(data::colored_string("third\n")));
    EXPECT_CALL(c, show(data::colored_string("   4  ")));
    EXPECT_CALL(c, show(data::colored_string("fourth\n")));
    EXPECT_CALL(c, show(data::colored_string("   5  ")));
    EXPECT_CALL(c, show(data::colored_string("fifth\n")));
    EXPECT_CALL(c, show(data::colored_string("   6  ")));
    EXPECT_CALL(c, show(data::colored_string("sixth\n")));
  }

  d.show_file_section(data::file_location("<stdin>", 3, 0),
                      "first\nsecond\nthird\nfourth\nfifth\nsixth\n");
}

TEST(console_displayer, show_file_section_6_lines_2)
{
  NiceMock<mock::console> c;

  ON_CALL(c, width()).WillByDefault(Return(0));

  core::console_displayer d(c, false, false);

  {
    ::testing::InSequence s;

    EXPECT_CALL(c, show(data::colored_string("   2  ")));
    EXPECT_CALL(c, show(data::colored_string("second\n")));
    EXPECT_CALL(c, show(data::colored_string("   3  ")));
    EXPECT_CALL(c, show(data::colored_string("third\n")));
    EXPECT_CALL(c, show(data::colored_string("   4  ")));
    EXPECT_CALL(c, show(data::colored_string("fourth\n")));
    EXPECT_CALL(c, show(data::colored_string("-> 5  ")));
    EXPECT_CALL(c, show(data::colored_string("fifth\n")));
    EXPECT_CALL(c, show(data::colored_string("   6  ")));
    EXPECT_CALL(c, show(data::colored_string("sixth\n")));
    EXPECT_CALL(c, show(data::colored_string("   7  ")));
    EXPECT_CALL(c, show(data::colored_string("seventh\n")));
    EXPECT_CALL(c, show(data::colored_string("   8  ")));
    EXPECT_CALL(c, show(data::colored_string("eight\n")));
  }

  d.show_file_section(
      data::file_location("<stdin>", 5, 0),
      "first\nsecond\nthird\nfourth\nfifth\nsixth\nseventh\neight\n");
}

TEST(console_displayer, show_file_section_10_lines)
{
  NiceMock<mock::console> c;

  ON_CALL(c, width()).WillByDefault(Return(0));

  core::console_displayer d(c, false, false);

  {
    ::testing::InSequence s;

    EXPECT_CALL(c, show(data::colored_string("    4  ")));
    EXPECT_CALL(c, show(data::colored_string("fourth\n")));
    EXPECT_CALL(c, show(data::colored_string("    5  ")));
    EXPECT_CALL(c, show(data::colored_string("fifth\n")));
    EXPECT_CALL(c, show(data::colored_string("    6  ")));
    EXPECT_CALL(c, show(data::colored_string("sixth\n")));
    EXPECT_CALL(c, show(data::colored_string("->  7  ")));
    EXPECT_CALL(c, show(data::colored_string("seventh\n")));
    EXPECT_CALL(c, show(data::colored_string("    8  ")));
    EXPECT_CALL(c, show(data::colored_string("eighth\n")));
    EXPECT_CALL(c, show(data::colored_string("    9  ")));
    EXPECT_CALL(c, show(data::colored_string("ninth\n")));
    EXPECT_CALL(c, show(data::colored_string("   10  ")));
    EXPECT_CALL(c, show(data::colored_string("tenth\n")));
  }

  d.show_file_section(data::file_location("<stdin>", 7, 0),
                      "first\nsecond\nthird\nfourth\nfifth\nsixth\nseventh\neig"
                      "hth\nninth\ntenth\n");
}
