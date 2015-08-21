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

#include <just/test.hpp>

using namespace metashell;

JUST_TEST_CASE(test_pager_one_line) {
  mock_console c(80, 100);
  pager p(c);

  p.show("first");
  JUST_ASSERT(p.new_line());

  JUST_ASSERT_EQUAL(0, c.ask_for_continuation_count());
  JUST_ASSERT_EQUAL("first\n", c.content());
}

JUST_TEST_CASE(test_pager_non_full_page) {
  mock_console c(80, 5);
  pager p(c);
  c.set_continiation_answer(iface::console::user_answer::next_page);

  p.show("first");
  JUST_ASSERT(p.new_line());
  p.show("second");
  JUST_ASSERT(p.new_line());
  p.show("third");
  JUST_ASSERT(p.new_line());

  JUST_ASSERT_EQUAL(0, c.ask_for_continuation_count());
  JUST_ASSERT_EQUAL("first\nsecond\nthird\n", c.content());
}

JUST_TEST_CASE(test_pager_almost_full_page) {
  mock_console c(80, 4);
  pager p(c);
  c.set_continiation_answer(iface::console::user_answer::next_page);

  p.show("first");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(0, c.ask_for_continuation_count());

  p.show("second");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(0, c.ask_for_continuation_count());

  p.show("third");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(1, c.ask_for_continuation_count());

  JUST_ASSERT_EQUAL("first\nsecond\nthird\n", c.content());
}

JUST_TEST_CASE(test_pager_full_page_by_one_line) {
  mock_console c(80, 4);
  pager p(c);
  c.set_continiation_answer(iface::console::user_answer::next_page);

  p.show("first");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(0, c.ask_for_continuation_count());

  p.show("second");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(0, c.ask_for_continuation_count());

  p.show("third");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(1, c.ask_for_continuation_count());

  p.show("forth");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(1, c.ask_for_continuation_count());

  JUST_ASSERT_EQUAL("first\nsecond\nthird\nforth\n", c.content());
}

JUST_TEST_CASE(test_pager_multi_page_next_page_answer) {
  mock_console c(80, 3);
  pager p(c);
  c.set_continiation_answer(iface::console::user_answer::next_page);

  p.show("first");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(0, c.ask_for_continuation_count());

  p.show("second");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(1, c.ask_for_continuation_count());

  p.show("third");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(1, c.ask_for_continuation_count());

  p.show("forth");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(2, c.ask_for_continuation_count());

  p.show("fifth");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(2, c.ask_for_continuation_count());

  JUST_ASSERT_EQUAL("first\nsecond\nthird\nforth\nfifth\n", c.content());
}

JUST_TEST_CASE(test_pager_multi_page_multiline_shows) {
  mock_console c(80, 3);
  pager p(c);
  c.set_continiation_answer(iface::console::user_answer::next_page);

  p.show("first\nsecond");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(1, c.ask_for_continuation_count());

  p.show("third");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(1, c.ask_for_continuation_count());

  p.show("forth\nfifth");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(2, c.ask_for_continuation_count());

  JUST_ASSERT_EQUAL("first\nsecond\nthird\nforth\nfifth\n", c.content());
}

JUST_TEST_CASE(test_pager_multi_page_narrow_terminal) {
  mock_console c(5, 3);
  pager p(c);
  c.set_continiation_answer(iface::console::user_answer::next_page);

  p.show("first" /*\n*/ "second");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(1, c.ask_for_continuation_count());

  p.show("third");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(1, c.ask_for_continuation_count());

  p.show("forth" /*\n*/ "fifth");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(2, c.ask_for_continuation_count());

  JUST_ASSERT_EQUAL("firstsecond\nthird\nforthfifth\n", c.content());
}

JUST_TEST_CASE(test_pager_multi_page_show_all_answer) {
  mock_console c(80, 3);
  pager p(c);
  c.set_continiation_answer(iface::console::user_answer::show_all);

  p.show("first");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(0, c.ask_for_continuation_count());

  p.show("second");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(1, c.ask_for_continuation_count());

  p.show("third");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(1, c.ask_for_continuation_count());

  p.show("forth");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(1, c.ask_for_continuation_count());

  p.show("fifth");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(1, c.ask_for_continuation_count());

  JUST_ASSERT_EQUAL("first\nsecond\nthird\nforth\nfifth\n", c.content());
}

JUST_TEST_CASE(test_pager_quit_answer) {
  mock_console c(80, 3);
  pager p(c);
  c.set_continiation_answer(iface::console::user_answer::quit);

  p.show("first");
  JUST_ASSERT(p.new_line());
  JUST_ASSERT_EQUAL(0, c.ask_for_continuation_count());

  p.show("second");
  JUST_ASSERT(!p.new_line());
  JUST_ASSERT_EQUAL(1, c.ask_for_continuation_count());

  JUST_ASSERT_EQUAL("first\nsecond\n", c.content());
}
