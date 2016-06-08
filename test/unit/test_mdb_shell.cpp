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

#include <metashell/in_memory_displayer.hpp>
#include <metashell/in_memory_history.hpp>
#include <metashell/null_displayer.hpp>

#include <just/test.hpp>

#include "mdb_test_shell.hpp"

using namespace metashell;

JUST_TEST_CASE(test_mdb_shell_is_stopped_false_by_default)
{
  mdb_test_shell sh;

  JUST_ASSERT(!sh.stopped());
}

JUST_TEST_CASE(test_mdb_shell_empty_lines)
{
  in_memory_displayer d;
  in_memory_history h;
  mdb_test_shell sh;

  JUST_ASSERT_EMPTY_CONTAINER(h.commands());

  sh.line_available("", d, h);
  JUST_ASSERT_EMPTY_CONTAINER(h.commands());
  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT(d.call_graphs().empty());

  sh.line_available(" ", d, h);
  JUST_ASSERT_EQUAL_CONTAINER({" "}, h.commands());
  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT(d.call_graphs().empty());

  sh.line_available("\t", d, h);
  JUST_ASSERT_EQUAL_CONTAINER({" ", "\t"}, h.commands());
  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
  JUST_ASSERT_EMPTY_CONTAINER(d.types());
  JUST_ASSERT(d.call_graphs().empty());
}

JUST_TEST_CASE(test_mdb_shell_identical_lines_in_history)
{
  null_displayer d;
  in_memory_history h;
  mdb_test_shell sh;

  JUST_ASSERT_EMPTY_CONTAINER(h.commands());

  sh.line_available("asd", d, h);
  JUST_ASSERT_EQUAL_CONTAINER({"asd"}, h.commands());

  sh.line_available("asd", d, h);
  JUST_ASSERT_EQUAL_CONTAINER({"asd"}, h.commands());

  sh.line_available("xyz", d, h);
  JUST_ASSERT_EQUAL_CONTAINER({"asd", "xyz"}, h.commands());

  sh.line_available("asd", d, h);
  JUST_ASSERT_EQUAL_CONTAINER({"asd", "xyz", "asd"}, h.commands());
}

JUST_TEST_CASE(test_mdb_shell_identical_all_space_lines_in_history)
{
  null_displayer d;
  in_memory_history h;
  mdb_test_shell sh;

  JUST_ASSERT_EMPTY_CONTAINER(h.commands());

  sh.line_available(" ", d, h);
  JUST_ASSERT_EQUAL_CONTAINER({" "}, h.commands());

  sh.line_available(" ", d, h);
  JUST_ASSERT_EQUAL_CONTAINER({" "}, h.commands());

  sh.line_available("  ", d, h);
  JUST_ASSERT_EQUAL_CONTAINER({" ", "  "}, h.commands());

  sh.line_available(" ", d, h);
  JUST_ASSERT_EQUAL_CONTAINER({" ", "  ", " "}, h.commands());
}

JUST_TEST_CASE(test_mdb_shell_skips_empty_lines)
{
  null_displayer d;
  in_memory_history h;
  mdb_test_shell sh;

  JUST_ASSERT_EMPTY_CONTAINER(h.commands());

  sh.line_available("ads", d, h);
  JUST_ASSERT_EQUAL_CONTAINER({"ads"}, h.commands());

  sh.line_available("", d, h);
  JUST_ASSERT_EQUAL_CONTAINER({"ads"}, h.commands());

  sh.line_available("ads", d, h);
  JUST_ASSERT_EQUAL_CONTAINER({"ads"}, h.commands());
}

JUST_TEST_CASE(test_mdb_shell_prompt)
{
  mdb_test_shell sh;

  JUST_ASSERT_EQUAL(sh.prompt(), "(mdb)");
}

JUST_TEST_CASE(test_mdb_shell_display_splash)
{
  mdb_test_shell sh;

  in_memory_displayer d;
  sh.display_splash(d);

  JUST_ASSERT_EQUAL_CONTAINER({"For help, type \"help\"."}, d.raw_texts());
}
