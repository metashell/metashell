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

#include <metashell_system_test/error.hpp>
#include <metashell_system_test/json_generator.hpp>
#include <metashell_system_test/raw_text.hpp>
#include <metashell_system_test/run_metashell.hpp>

#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell_system_test;

JUST_TEST_CASE(test_mdb_break_list_with_no_breakpoints)
{
  const auto r = run_metashell({command("#msh mdb"), command("break list")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(raw_text("No breakpoints currently set"), *i);
}

JUST_TEST_CASE(test_mdb_break_list_with_one_breakpoint)
{
  const auto r = run_metashell(
      {command("#msh mdb int"), command("rbreak i"), command("break list")});

  auto i = r.begin() + 5;

  JUST_ASSERT_EQUAL(raw_text("Breakpoint 1: regex(\"i\")"), *i);
}

JUST_TEST_CASE(test_mdb_break_list_with_two_breakpoints)
{
  const auto r = run_metashell({command("#msh mdb int"), command("rbreak i"),
                                command("rbreak n"), command("break list")});

  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(raw_text("Breakpoint 1: regex(\"i\")"), *i);
  ++i;
  JUST_ASSERT_EQUAL(raw_text("Breakpoint 2: regex(\"n\")"), *i);
}

JUST_TEST_CASE(test_mdb_break_garbage_argument)
{
  const auto r = run_metashell({command("#msh mdb"), command("break asd")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(error("Call break like this: \"break list\""), *i);
}
