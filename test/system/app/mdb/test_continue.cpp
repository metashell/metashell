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

#include <metashell_system_test/type.hpp>
#include <metashell_system_test/error.hpp>
#include <metashell_system_test/frame.hpp>
#include <metashell_system_test/prompt.hpp>
#include <metashell_system_test/raw_text.hpp>
#include <metashell_system_test/run_metashell.hpp>
#include <metashell_system_test/json_generator.hpp>

#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell_system_test;

JUST_TEST_CASE(test_mdb_continue_without_evaluation)
{
  const auto r = run_metashell({command("#msh mdb"), command("continue")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(error("Metaprogram not evaluated yet"), *i);
}

JUST_TEST_CASE(test_mdb_continue_garbage_argument)
{
  const auto r =
      run_metashell({command("#msh mdb int"), command("continue asd")});

  auto i = r.begin() + 3;

  JUST_ASSERT_EQUAL(error("Argument parsing failed"), *i);
}

JUST_TEST_CASE(test_mdb_continue_fibonacci_no_breakpoint)
{
  const auto r = run_metashell({command(fibonacci_mp),
                                command("#msh mdb int_<fib<10>::value>"),
                                command("continue")});

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int_<55>"), *i);
}

JUST_TEST_CASE(test_mdb_continue_fibonacci_reevaluation_removes_breakpoints)
{
  const auto r = run_metashell(
      {command(fibonacci_mp), command("#msh mdb int_<fib<10>::value>"),
       command("rbreak fib<0>"), command("evaluate int_<fib<10>::value>"),
       command("continue")});

  auto i = r.begin() + 8;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int_<55>"), *i);
}

JUST_TEST_CASE(test_mdb_continue_fibonacci_1_breakpoint)
{
  const auto r = run_metashell({command(fibonacci_mp),
                                command("#msh mdb int_<fib<10>::value>"),
                                command("rbreak fib<0>"), command("continue")});

  auto i = r.begin() + 6;

  JUST_ASSERT_EQUAL(raw_text("Breakpoint 1: regex(\"fib<0>\") reached"), *i++);
  JUST_ASSERT_EQUAL(
      frame(type("fib<0>"), _, _, instantiation_kind::memoization), *i);
}

JUST_TEST_CASE(test_mdb_continue_2_fibonacci_1_breakpoint)
{
  const auto r = run_metashell(
      {command(fibonacci_mp), command("#msh mdb int_<fib<10>::value>"),
       command("rbreak fib<5>"), command("continue 2")});

  auto i = r.begin() + 6;

  JUST_ASSERT_EQUAL(raw_text("Breakpoint 1: regex(\"fib<5>\") reached"), *i++);
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::memoization), *i);
}

JUST_TEST_CASE(test_mdb_continue_twice_fibonacci_1_breakpoint)
{
  const auto r = run_metashell(
      {command(fibonacci_mp), command("#msh mdb int_<fib<10>::value>"),
       command("rbreak fib<5>"), command("continue"), command("continue")});

  auto i = r.begin() + 6;

  JUST_ASSERT_EQUAL(raw_text("Breakpoint 1: regex(\"fib<5>\") reached"), *i++);
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::template_instantiation),
      *i);

  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Breakpoint 1: regex(\"fib<5>\") reached"), *i++);
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::memoization), *i);
}

JUST_TEST_CASE(test_mdb_continue_fibonacci_2_breakpoints)
{
  const auto r = run_metashell(
      {command(fibonacci_mp), command("#msh mdb int_<fib<10>::value>"),
       command("rbreak fib<5>"), command("rbreak fib<6>"), command("continue"),
       command("continue")});

  auto i = r.begin() + 8;

  JUST_ASSERT_EQUAL(raw_text("Breakpoint 2: regex(\"fib<6>\") reached"), *i++);
  JUST_ASSERT_EQUAL(
      frame(type("fib<6>"), _, _, instantiation_kind::template_instantiation),
      *i);

  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Breakpoint 1: regex(\"fib<5>\") reached"), *i++);
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::template_instantiation),
      *i);
}

JUST_TEST_CASE(test_mdb_continue_2_fibonacci_2_breakpoints)
{
  const auto r = run_metashell(
      {command(fibonacci_mp), command("#msh mdb int_<fib<10>::value>"),
       command("rbreak fib<5>"), command("rbreak fib<6>"),
       command("continue 2")});

  auto i = r.begin() + 8;

  JUST_ASSERT_EQUAL(raw_text("Breakpoint 1: regex(\"fib<5>\") reached"), *i++);
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::template_instantiation),
      *i);
}

JUST_TEST_CASE(test_mdb_continue_10_fibonacci_2_breakpoints)
{
  const auto r = run_metashell(
      {command(fibonacci_mp), command("#msh mdb int_<fib<10>::value>"),
       command("rbreak fib<5>"), command("rbreak fib<6>"),
       command("continue 10")});

  auto i = r.begin() + 8;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int_<55>"), *i);
}

JUST_TEST_CASE(test_mdb_continue_0_fibonacci_1_breakpoint)
{
  const auto r = run_metashell(
      {command(fibonacci_mp), command("#msh mdb int_<fib<10>::value>"),
       command("rbreak fib<6>"), command("continue 0")});
  auto i = r.begin() + 6;

  // continue 0 doesn't print anything at start
  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
}

JUST_TEST_CASE(test_mdb_continue_minus_1_at_start)
{
  const auto r = run_metashell({command(fibonacci_mp),
                                command("#msh mdb int_<fib<10>::value>"),
                                command("continue -1")});
  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"), *i);
}

JUST_TEST_CASE(test_mdb_continue_minus_2_at_start)
{
  const auto r = run_metashell({command(fibonacci_mp),
                                command("#msh mdb int_<fib<10>::value>"),
                                command("continue -2")});
  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"), *i);
}

JUST_TEST_CASE(test_mdb_continue_minus_1_with_preceding_breakpoint)
{
  const auto r = run_metashell(
      {command(fibonacci_mp), command("#msh mdb int_<fib<10>::value>"),
       command("rbreak fib<6>"), command("rbreak fib<5>"),
       command("continue 2"), command("continue -1")});
  auto i = r.begin() + 8;

  JUST_ASSERT_EQUAL(raw_text("Breakpoint 2: regex(\"fib<5>\") reached"), *i++);
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::template_instantiation),
      *i);

  i += 2;

  JUST_ASSERT_EQUAL(raw_text("Breakpoint 1: regex(\"fib<6>\") reached"), *i++);
  JUST_ASSERT_EQUAL(
      frame(type("fib<6>"), _, _, instantiation_kind::template_instantiation),
      *i);
}

JUST_TEST_CASE(test_mdb_continue_minus_1_without_preceding_breakpoint)
{
  const auto r = run_metashell({command(fibonacci_mp),
                                command("#msh mdb int_<fib<10>::value>"),
                                command("rbreak fib<5>"), command("continue 1"),
                                command("continue -1")});
  auto i = r.begin() + 6;

  JUST_ASSERT_EQUAL(raw_text("Breakpoint 1: regex(\"fib<5>\") reached"), *i++);
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::template_instantiation),
      *i);

  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"), *i);
}

JUST_TEST_CASE(test_mdb_continue_to_end_and_back_to_start)
{
  const auto r = run_metashell({command(fibonacci_mp),
                                command("#msh mdb int_<fib<10>::value>"),
                                command("rbreak fib<5>"), command("continue 3"),
                                command("continue -3")});
  auto i = r.begin() + 6;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int_<55>"), *i);

  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"), *i);
}

JUST_TEST_CASE(test_mdb_continue_to_end_and_back_to_start_in_full_mode)
{
  const auto r = run_metashell({command(fibonacci_mp),
                                command("#msh mdb -full int_<fib<10>::value>"),
                                command("rbreak fib<5>"), command("continue 9"),
                                command("continue -9")});
  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint \"fib<5>\" will stop the execution on 8 locations"),
      *i);

  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int_<55>"), *i);

  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"), *i);
}
JUST_TEST_CASE(
    test_mdb_continue_to_one_before_end_and_back_to_start_in_full_mode)
{
  const auto r = run_metashell({command(fibonacci_mp),
                                command("#msh mdb -full int_<fib<10>::value>"),
                                command("rbreak fib<5>"), command("continue 8"),
                                command("continue -8")});
  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint \"fib<5>\" will stop the execution on 8 locations"),
      *i);

  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Breakpoint 1: regex(\"fib<5>\") reached"), *i++);
  JUST_ASSERT_EQUAL(frame(type("fib<5>")), *i);

  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"), *i);
}

JUST_TEST_CASE(test_mdb_continue_to_end_will_print_error_message_if_errored)
{
  const auto r = run_metashell({command(missing_value_fibonacci_mp),
                                command("#msh mdb int_<fib<5>::value>"),
                                command("continue")});
  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(error(_), *i);
}

JUST_TEST_CASE(test_mdb_continue_to_end_and_back_if_errored)
{
  const auto r = run_metashell({command(missing_value_fibonacci_mp),
                                command("#msh mdb int_<fib<5>::value>"),
                                command("continue"), command("continue -1")});
  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"), *i);
}
