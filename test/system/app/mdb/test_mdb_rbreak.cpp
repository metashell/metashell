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

#include <metashell_system_test/error.hpp>
#include <metashell_system_test/json_generator.hpp>
#include <metashell_system_test/raw_text.hpp>
#include <metashell_system_test/run_metashell.hpp>

#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell_system_test;

JUST_TEST_CASE(test_mdb_rbreak_without_evaluated_metaprogram)
{
  const auto r = run_metashell({command("#msh mdb"), command("rbreak int")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(error("Metaprogram not evaluated yet"), *i);
}

JUST_TEST_CASE(test_mdb_rbreak_with_no_arguments)
{
  const auto r = run_metashell(
      {command("#msh mdb"), command("evaluate int"), command("rbreak")});

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(error("Argument expected"), *i);
}

JUST_TEST_CASE(test_mdb_rbreak_with_no_arguments_with_trailing_whitespace)
{
  const auto r = run_metashell(
      {command("#msh mdb"), command("evaluate int"), command("rbreak ")});

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(error("Argument expected"), *i);
}

JUST_TEST_CASE(test_mdb_rbreak_with_invalid_regex)
{
  const auto r = run_metashell(
      {command("#msh mdb"), command("evaluate int"), command("rbreak [")});

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(error("\"[\" is not a valid regex"), *i);
}

JUST_TEST_CASE(test_mdb_rbreak_with_valid_regex_no_match)
{
  const auto r = run_metashell(
      {command("#msh mdb"), command("evaluate int"), command("rbreak xyz")});

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint \"xyz\" will never stop the execution"), *i);
}

JUST_TEST_CASE(test_mdb_rbreak_with_valid_regex_with_one_match)
{
  const auto r = run_metashell(
      {command("#msh mdb"), command("evaluate int"), command("rbreak int")});

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint \"int\" will stop the execution on 1 location"), *i);
}

JUST_TEST_CASE(test_mdb_rbreak_with_valid_regex_with_three_matches)
{
  const auto r = run_metashell({command(fibonacci_mp), command("#msh mdb"),
                                command("evaluate int_<fib<5>::value>"),
                                command("rbreak fib<3>")});

  auto i = r.begin() + 5;

  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint \"fib<3>\" will stop the execution on 3 locations"),
      *i);
}

JUST_TEST_CASE(test_mdb_rbreak_does_not_count_stops_in_unreachable_subgraphs)
{
  const auto r = run_metashell(
      {command(fibonacci_mp + "int __x = fib<10>::value;"), command("#msh mdb"),
       command("evaluate int_<fib<2>::value>"), command("rbreak fib")});

  auto i = r.begin() + 5;

  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint \"fib\" will stop the execution on 3 locations"),
      *i);
}

JUST_TEST_CASE(test_mdb_rbreak_with_valid_regex_in_full_mode)
{
  const auto r = run_metashell({command(fibonacci_mp), command("#msh mdb"),
                                command("evaluate -full int_<fib<5>::value>"),
                                command("rbreak fib<3>")});

  auto i = r.begin() + 5;

  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint \"fib<3>\" will stop the execution on 2 locations"),
      *i);
}

JUST_TEST_CASE(test_mdb_rbreak_with_valid_regex_in_full_mode_match_only_root)
{
  const auto r = run_metashell({command(fibonacci_mp), command("#msh mdb"),
                                command("evaluate -full int_<fib<5>::value>"),
                                command("rbreak int_<fib<5>::value>")});

  auto i = r.begin() + 5;

  JUST_ASSERT_EQUAL(
      raw_text(
          "Breakpoint \"int_<fib<5>::value>\" will never stop the execution"),
      *i);
}

JUST_TEST_CASE(test_mdb_rbreak_with_valid_regex_in_full_mode_match_also_root)
{
  const auto r =
      run_metashell({command(fibonacci_mp), command("#msh mdb"),
                     command("evaluate -full int_<fib<5>::value>"),
                     command("rbreak (int_<fib<5>::value>)|(fib<3>)")});

  auto i = r.begin() + 5;

  JUST_ASSERT_EQUAL(raw_text("Breakpoint \"(int_<fib<5>::value>)|(fib<3>)\" "
                             "will stop the execution on 2 locations"),
                    *i);
}
