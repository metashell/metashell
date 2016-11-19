// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Andras Kucsma (andras.kucsma@gmail.com)
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
#include <metashell/pager.hpp>

#include "mock_console.hpp"

#include <gtest/gtest.h>

using namespace metashell;

TEST(pager, one_line)
{
  mock_console c(80, 100);
  pager p(c);

  p.show("first");
  ASSERT_TRUE(p.new_line());

  ASSERT_EQ(0, c.ask_for_continuation_count());
  ASSERT_EQ("first\n", c.content());
}

TEST(pager, non_full_page)
{
  mock_console c(80, 5);
  pager p(c);
  c.set_continiation_answer(iface::console::user_answer::next_page);

  p.show("first");
  ASSERT_TRUE(p.new_line());
  p.show("second");
  ASSERT_TRUE(p.new_line());
  p.show("third");
  ASSERT_TRUE(p.new_line());

  ASSERT_EQ(0, c.ask_for_continuation_count());
  ASSERT_EQ("first\nsecond\nthird\n", c.content());
}

TEST(pager, almost_full_page)
{
  mock_console c(80, 4);
  pager p(c);
  c.set_continiation_answer(iface::console::user_answer::next_page);

  p.show("first");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(0, c.ask_for_continuation_count());

  p.show("second");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(0, c.ask_for_continuation_count());

  p.show("third");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(1, c.ask_for_continuation_count());

  ASSERT_EQ("first\nsecond\nthird\n", c.content());
}

TEST(pager, full_page_by_one_line)
{
  mock_console c(80, 4);
  pager p(c);
  c.set_continiation_answer(iface::console::user_answer::next_page);

  p.show("first");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(0, c.ask_for_continuation_count());

  p.show("second");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(0, c.ask_for_continuation_count());

  p.show("third");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(1, c.ask_for_continuation_count());

  p.show("forth");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(1, c.ask_for_continuation_count());

  ASSERT_EQ("first\nsecond\nthird\nforth\n", c.content());
}

TEST(pager, multi_page_next_page_answer)
{
  mock_console c(80, 3);
  pager p(c);
  c.set_continiation_answer(iface::console::user_answer::next_page);

  p.show("first");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(0, c.ask_for_continuation_count());

  p.show("second");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(1, c.ask_for_continuation_count());

  p.show("third");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(1, c.ask_for_continuation_count());

  p.show("forth");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(2, c.ask_for_continuation_count());

  p.show("fifth");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(2, c.ask_for_continuation_count());

  ASSERT_EQ("first\nsecond\nthird\nforth\nfifth\n", c.content());
}

TEST(pager, multi_page_multiline_shows)
{
  mock_console c(80, 3);
  pager p(c);
  c.set_continiation_answer(iface::console::user_answer::next_page);

  p.show("first\nsecond");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(1, c.ask_for_continuation_count());

  p.show("third");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(1, c.ask_for_continuation_count());

  p.show("forth\nfifth");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(2, c.ask_for_continuation_count());

  ASSERT_EQ("first\nsecond\nthird\nforth\nfifth\n", c.content());
}

TEST(pager, multi_page_narrow_terminal)
{
  mock_console c(5, 3);
  pager p(c);
  c.set_continiation_answer(iface::console::user_answer::next_page);

  p.show("first" /*\n*/ "second");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(1, c.ask_for_continuation_count());

  p.show("third");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(1, c.ask_for_continuation_count());

  p.show("forth" /*\n*/ "fifth");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(2, c.ask_for_continuation_count());

  ASSERT_EQ("firstsecond\nthird\nforthfifth\n", c.content());
}

TEST(pager, multi_page_show_all_answer)
{
  mock_console c(80, 3);
  pager p(c);
  c.set_continiation_answer(iface::console::user_answer::show_all);

  p.show("first");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(0, c.ask_for_continuation_count());

  p.show("second");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(1, c.ask_for_continuation_count());

  p.show("third");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(1, c.ask_for_continuation_count());

  p.show("forth");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(1, c.ask_for_continuation_count());

  p.show("fifth");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(1, c.ask_for_continuation_count());

  ASSERT_EQ("first\nsecond\nthird\nforth\nfifth\n", c.content());
}

TEST(pager, quit_answer)
{
  mock_console c(80, 3);
  pager p(c);
  c.set_continiation_answer(iface::console::user_answer::quit);

  p.show("first");
  ASSERT_TRUE(p.new_line());
  ASSERT_EQ(0, c.ask_for_continuation_count());

  p.show("second");
  ASSERT_FALSE(p.new_line());
  ASSERT_EQ(1, c.ask_for_continuation_count());

  ASSERT_EQ("first\nsecond\n", c.content());
}
