// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <just/test.hpp>

#include "mdb_test_shell.hpp"

using namespace metashell;

JUST_TEST_CASE(test_mdb_shell_empty_lines) {
  mdb_test_shell sh;

  JUST_ASSERT(sh.get_history().empty());

  sh.line_available("");
  JUST_ASSERT(sh.get_history().empty());
  JUST_ASSERT_EQUAL(sh.get_output(), "");

  sh.line_available(" ");
  JUST_ASSERT(sh.get_history().empty());
  JUST_ASSERT_EQUAL(sh.get_output(), "");

  sh.line_available("\t");
  JUST_ASSERT(sh.get_history().empty());
  JUST_ASSERT_EQUAL(sh.get_output(), "");
}

JUST_TEST_CASE(test_mdb_shell_identical_lines_in_history) {
  mdb_test_shell sh;

  JUST_ASSERT(sh.get_history().empty());

  sh.line_available("asd");
  JUST_ASSERT(sh.get_history().size() == 1);
  JUST_ASSERT_EQUAL(sh.get_history()[0], "asd");

  sh.line_available("asd");
  JUST_ASSERT(sh.get_history().size() == 1);
  JUST_ASSERT_EQUAL(sh.get_history()[0], "asd");

  sh.line_available("xyz");
  JUST_ASSERT(sh.get_history().size() == 2);
  JUST_ASSERT_EQUAL(sh.get_history()[0], "asd");
  JUST_ASSERT_EQUAL(sh.get_history()[1], "xyz");

  sh.line_available("asd");
  JUST_ASSERT(sh.get_history().size() == 3);
  JUST_ASSERT_EQUAL(sh.get_history()[0], "asd");
  JUST_ASSERT_EQUAL(sh.get_history()[1], "xyz");
  JUST_ASSERT_EQUAL(sh.get_history()[2], "asd");
}

JUST_TEST_CASE(test_mdb_shell_prompt) {
  mdb_test_shell sh;

  JUST_ASSERT_EQUAL(sh.prompt(), "(mdb) ");
}

