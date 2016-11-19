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

#include <metashell/console_displayer.hpp>

#include "mock_console.hpp"
#include "util.hpp"

#include <gtest/gtest.h>

#include <vector>

using namespace metashell;

namespace
{
  typedef std::vector<data::call_graph_node> call_grph;

  call_grph fib5_call_graph()
  {
    using metashell::data::instantiation_kind;
    using metashell::data::type;
    using metashell::data::frame;
    using metashell::data::file_location;

    file_location f("a.cpp", 1, 2);
    file_location f2("b.hpp", 1, 2);

    return call_grph{
        {frame(type("int_<fib<5>::value>"), f2), 0, 3},
        {frame(fib<5>(), f2, f, instantiation_kind::template_instantiation), 1,
         2},
        {frame(fib<3>(), f2, f, instantiation_kind::template_instantiation), 2,
         2},
        {frame(fib<1>(), f2, f, instantiation_kind::memoization), 3, 0},
        {frame(fib<2>(), f2, f, instantiation_kind::template_instantiation), 3,
         2},
        {frame(fib<0>(), f2, f, instantiation_kind::memoization), 4, 0},
        {frame(fib<1>(), f2, f, instantiation_kind::memoization), 4, 0},
        {frame(fib<4>(), f2, f, instantiation_kind::template_instantiation), 2,
         2},
        {frame(fib<2>(), f2, f, instantiation_kind::memoization), 3, 0},
        {frame(fib<3>(), f2, f, instantiation_kind::memoization), 3, 0},
        {frame(fib<5>(), f2, f, instantiation_kind::memoization), 1, 0},
        {frame(type("int_<5>"), f2, f,
               instantiation_kind::template_instantiation),
         1, 0}};
  }
}

TEST(console_displayer, nothing_is_displayed_by_default)
{
  mock_console c;
  console_displayer cd(c, false, false);

  ASSERT_EQ("", c.content().get_string());
}

TEST(console_displayer, raw_text_is_printed)
{
  mock_console c;
  console_displayer cd(c, false, false);
  cd.show_raw_text("Hello world!");

  ASSERT_EQ("Hello world!\n", c.content().get_string());
}

TEST(console_displayer, raw_text_with_new_line_is_printed)
{
  mock_console c;
  console_displayer cd(c, false, false);
  cd.show_raw_text("Hello\nworld!");

  ASSERT_EQ("Hello\nworld!\n", c.content().get_string());
}

TEST(console_displayer, error_with_no_colors_is_printed)
{
  mock_console c;
  console_displayer cd(c, false, false);
  cd.show_error("Something went wrong");

  ASSERT_EQ(
      data::colored_string("Something went wrong\n", boost::none), c.content());
}

TEST(console_displayer, error_with_colors_is_printed_in_red)
{
  mock_console c;
  console_displayer cd(c, false, true);
  cd.show_error("Something went wrong");

  ASSERT_EQ(
      data::colored_string("Something went wrong", data::color::bright_red) +
          "\n",
      c.content());
}

TEST(console_displayer, mdb_forwardtrace_from_root_on_narrow_terminal)
{
  mock_console c(25);
  console_displayer d(c, false, false);

  d.show_call_graph(fib5_call_graph());

  ASSERT_EQ(
      "int_<fib<5>::value>\n"
      "+ fib<5> at b.hpp:1:2 (Te\n"
      "| mplateInstantiation fro\n"
      "| m a.cpp:1:2)\n"
      "| + fib<3> at b.hpp:1:2 (\n"
      "| | TemplateInstantiation\n"
      "| |  from a.cpp:1:2)\n"
      "| | + fib<1> at b.hpp:1:2\n"
      "| | |  (Memoization from \n"
      "| | | a.cpp:1:2)\n"
      "| | ` fib<2> at b.hpp:1:2\n"
      "| |    (TemplateInstantia\n"
      "| |   tion from a.cpp:1:2\n"
      "| |   )\n"
      "| |   + fib<0> at b.hpp:1\n"
      "| |   | :2 (Memoization f\n"
      "| |   | rom a.cpp:1:2)\n"
      "| |   ` fib<1> at b.hpp:1\n"
      "| |     :2 (Memoization f\n"
      "| |     rom a.cpp:1:2)\n"
      "| ` fib<4> at b.hpp:1:2 (\n"
      "|   TemplateInstantiation\n"
      "|    from a.cpp:1:2)\n"
      "|   + fib<2> at b.hpp:1:2\n"
      "|   |  (Memoization from \n"
      "|   | a.cpp:1:2)\n"
      "|   ` fib<3> at b.hpp:1:2\n"
      "|      (Memoization from \n"
      "|     a.cpp:1:2)\n"
      "+ fib<5> at b.hpp:1:2 (Me\n"
      "| moization from a.cpp:1:\n"
      "| 2)\n"
      "` int_<5> at b.hpp:1:2 (T\n"
      "  emplateInstantiation fr\n"
      "  om a.cpp:1:2)\n",
      c.content().get_string());
}

TEST(console_displayer, mdb_forwardtrace_on_extremely_narrow_terminal_w0)
{
  mock_console c(0);
  console_displayer d(c, false, false);

  d.show_call_graph(fib5_call_graph());

  // The algorithm just gives up, and prints without extra line breaks
  ASSERT_EQ(
      "int_<fib<5>::value>\n"
      "+ fib<5> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)\n"
      "| + fib<3> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)\n"
      "| | + fib<1> at b.hpp:1:2 (Memoization from a.cpp:1:2)\n"
      "| | ` fib<2> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)\n"
      "| |   + fib<0> at b.hpp:1:2 (Memoization from a.cpp:1:2)\n"
      "| |   ` fib<1> at b.hpp:1:2 (Memoization from a.cpp:1:2)\n"
      "| ` fib<4> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)\n"
      "|   + fib<2> at b.hpp:1:2 (Memoization from a.cpp:1:2)\n"
      "|   ` fib<3> at b.hpp:1:2 (Memoization from a.cpp:1:2)\n"
      "+ fib<5> at b.hpp:1:2 (Memoization from a.cpp:1:2)\n"
      "` int_<5> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)\n",
      c.content().get_string());
}

TEST(console_displayer, mdb_forwardtrace_on_extremely_narrow_terminal_w1)
{
  mock_console c(1);
  console_displayer d(c, false, false);

  d.show_call_graph(fib5_call_graph());

  // The algorithm just gives up, and prints without extra line breaks
  ASSERT_EQ(
      "int_<fib<5>::value>\n"
      "+ fib<5> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)\n"
      "| + fib<3> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)\n"
      "| | + fib<1> at b.hpp:1:2 (Memoization from a.cpp:1:2)\n"
      "| | ` fib<2> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)\n"
      "| |   + fib<0> at b.hpp:1:2 (Memoization from a.cpp:1:2)\n"
      "| |   ` fib<1> at b.hpp:1:2 (Memoization from a.cpp:1:2)\n"
      "| ` fib<4> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)\n"
      "|   + fib<2> at b.hpp:1:2 (Memoization from a.cpp:1:2)\n"
      "|   ` fib<3> at b.hpp:1:2 (Memoization from a.cpp:1:2)\n"
      "+ fib<5> at b.hpp:1:2 (Memoization from a.cpp:1:2)\n"
      "` int_<5> at b.hpp:1:2 (TemplateInstantiation from a.cpp:1:2)\n",
      c.content().get_string());
}

TEST(console_displayer, show_file_section_3_lines_1)
{
  mock_console c(0);
  console_displayer d(c, false, false);

  std::string stdin_content =
      "first\n"
      "second\n"
      "third\n";

  data::file_location location("<stdin>", 2, 0);
  d.show_file_section(location, stdin_content);

  ASSERT_EQ(
      "   1  first\n"
      "-> 2  second\n"
      "   3  third\n",
      c.content().get_string());
}

TEST(console_displayer, show_file_section_3_lines_2)
{
  mock_console c(0);
  console_displayer d(c, false, false);

  std::string stdin_content =
      "first\n"
      "second\n"
      "third\n";

  data::file_location location("<stdin>", 3, 0);
  d.show_file_section(location, stdin_content);

  ASSERT_EQ(
      "   1  first\n"
      "   2  second\n"
      "-> 3  third\n",
      c.content().get_string());
}

TEST(console_displayer, show_file_section_6_lines_1)
{
  mock_console c(0);
  console_displayer d(c, false, false);

  std::string stdin_content =
      "first\n"
      "second\n"
      "third\n"
      "fourth\n"
      "fifth\n"
      "sixth\n";

  data::file_location location("<stdin>", 3, 0);
  d.show_file_section(location, stdin_content);

  ASSERT_EQ(
      "   1  first\n"
      "   2  second\n"
      "-> 3  third\n"
      "   4  fourth\n"
      "   5  fifth\n"
      "   6  sixth\n",
      c.content().get_string());
}

TEST(console_displayer, show_file_section_6_lines_2)
{
  mock_console c(0);
  console_displayer d(c, false, false);

  std::string stdin_content =
      "first\n"
      "second\n"
      "third\n"
      "fourth\n"
      "fifth\n"
      "sixth\n"
      "seventh\n"
      "eight\n";

  data::file_location location("<stdin>", 5, 0);
  d.show_file_section(location, stdin_content);

  ASSERT_EQ(
      "   2  second\n"
      "   3  third\n"
      "   4  fourth\n"
      "-> 5  fifth\n"
      "   6  sixth\n"
      "   7  seventh\n"
      "   8  eight\n",
      c.content().get_string());
}

TEST(console_displayer, show_file_section_10_lines)
{
  mock_console c(0);
  console_displayer d(c, false, false);

  std::string stdin_content =
      "first\n"
      "second\n"
      "third\n"
      "fourth\n"
      "fifth\n"
      "sixth\n"
      "seventh\n"
      "eighth\n"
      "ninth\n"
      "tenth\n";

  data::file_location location("<stdin>", 7, 0);
  d.show_file_section(location, stdin_content);

  ASSERT_EQ(
      "    4  fourth\n"
      "    5  fifth\n"
      "    6  sixth\n"
      "->  7  seventh\n"
      "    8  eighth\n"
      "    9  ninth\n"
      "   10  tenth\n",
      c.content().get_string());
}
