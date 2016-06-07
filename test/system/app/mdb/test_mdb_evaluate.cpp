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

#include <metashell_system_test/call_graph.hpp>
#include <metashell_system_test/error.hpp>
#include <metashell_system_test/json_generator.hpp>
#include <metashell_system_test/prompt.hpp>
#include <metashell_system_test/raw_text.hpp>
#include <metashell_system_test/run_metashell.hpp>
#include <metashell_system_test/type.hpp>

#include "fib.hpp"
#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell_system_test;

using pattern::_;

JUST_TEST_CASE(test_mdb_evaluate_int)
{
  const auto r = run_metashell(
      {command("#msh mdb"), command("evaluate int"), command("continue")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  i += 2;
  ++i;
  JUST_ASSERT_EQUAL(type("int"), *i);
}

JUST_TEST_CASE(test_mdb_evaluate_fib_10)
{
  const auto r = run_metashell({command(fibonacci_mp), command("#msh mdb"),
                                command("evaluate int_<fib<10>::value>"),
                                command("continue")});

  auto i = r.begin() + 3;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  i += 2;
  ++i;
  JUST_ASSERT_EQUAL(type("int_<55>"), *i);
}

JUST_TEST_CASE(test_mdb_evaluate_empty_environment)
{
  const auto r = run_metashell(
      {command("#msh mdb"), command("evaluate -"), command("continue")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  i += 2;
  ++i;
  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
}

JUST_TEST_CASE(test_mdb_evaluate_no_arguments_no_evaluation)
{
  const auto r = run_metashell({command("#msh mdb"), command("evaluate")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(error("Nothing has been evaluated yet."), *i);
}

JUST_TEST_CASE(
    test_mdb_evaluate_no_arguments_with_trailing_spaces_no_evaluation)
{
  const auto r = run_metashell({command("#msh mdb"), command("evaluate  ")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(error("Nothing has been evaluated yet."), *i);
}

JUST_TEST_CASE(test_mdb_evaluate_failure_will_reset_metaprogram_state)
{
  const auto r =
      run_metashell({command("#msh mdb"), command("evaluate int"),
                     command("evaluate in"), command("evaluate int")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  i += 2;
  JUST_ASSERT_EQUAL(error(_), *i);
  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
}

JUST_TEST_CASE(test_mdb_evaluate_instantiation_failure_will_start_metaprogram)
{
  const auto r = run_metashell({
      command(missing_value_fibonacci_mp), command("#msh mdb"),
      command("evaluate int_<fib<5>::value>"),
  });

  auto i = r.begin() + 3;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
}

JUST_TEST_CASE(test_mdb_evaluate_missing_argument_will_run_last_metaprogram)
{
  const auto r = run_metashell({command("#msh mdb"), command("evaluate int"),
                                command("evaluate"), command("continue")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  i += 3;
  JUST_ASSERT_EQUAL(type("int"), *i);
}

JUST_TEST_CASE(test_mdb_evaluate_missing_argument_will_reset_metaprogram_state)
{
  const auto r =
      run_metashell({command(fibonacci_mp), command("#msh mdb"),
                     command("evaluate int_<fib<10>::value>"), command("ft"),
                     command("step 5"), command("evaluate"), command("ft")});

  auto i = r.begin() + 5;

  const auto first_ft = *i;
  i += 6;
  JUST_ASSERT_EQUAL(first_ft, *i);
}

JUST_TEST_CASE(test_mdb_reevaluate_environment)
{
  const auto r = run_metashell({command("#msh mdb"), command("evaluate -"),
                                command("continue"), command("evaluate"),
                                command("continue")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  i += 2;
  ++i;
  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  i += 2;
  ++i;
  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
}

JUST_TEST_CASE(test_mdb_evaluate_filters_similar_edges)
{
  const auto r = run_metashell(
      {command(fibonacci_with_enum_mp), command("#msh mdb"),
       command("evaluate int_<fib<2>::value>"), command("forwardtrace")});

  auto i = r.begin() + 3;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  i += 2;

// On Windows clang tries to be compatible with MSVC, and this affects the Sema
// code to take slightly different paths. Probably because of this, Memoization
// events are not generated for the nested enum type.
#ifdef _WIN32
  JUST_ASSERT_EQUAL(
      call_graph(
          {{frame(type("int_<fib<2>::value>")), 0, 3},
           {frame(fib<2>(), _, _, instantiation_kind::template_instantiation),
            1, 2},
           {frame(fib<0>(), _, _, instantiation_kind::memoization), 2, 0},
           {frame(fib<1>(), _, _, instantiation_kind::memoization), 2, 0},
           {frame(fib<2>(), _, _, instantiation_kind::memoization), 1, 0},
           {frame(type("int_<1>"), _, _,
                  instantiation_kind::template_instantiation),
            1, 0}}),
      *i);
#else
  JUST_ASSERT_EQUAL(
      call_graph(
          {{frame(type("int_<fib<2>::value>")), 0, 4},
           {frame(fib<2>(), _, _, instantiation_kind::template_instantiation),
            1, 4},
           {frame(fib<0>(), _, _, instantiation_kind::memoization), 2, 0},
           {frame(fib<1>(), _, _, instantiation_kind::memoization), 2, 0},
           {frame(type("fib<1>::ENUM"), _, _, instantiation_kind::memoization),
            2, 0},
           {frame(type("fib<0>::ENUM"), _, _, instantiation_kind::memoization),
            2, 0},
           {frame(fib<2>(), _, _, instantiation_kind::memoization), 1, 0},
           {frame(type("fib<2>::ENUM"), _, _, instantiation_kind::memoization),
            1, 0},
           {frame(type("int_<1>"), _, _,
                  instantiation_kind::template_instantiation),
            1, 0}}),
      *i);
#endif
}

JUST_TEST_CASE(test_mdb_evaluate_clears_breakpoints)
{
  const auto r = run_metashell({
      command("#msh mdb"), command("evaluate int"), command("rbreak int"),
      command("break list"), command("evaluate unsigned int"),
      command("break list"),
  });

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint \"int\" will stop the execution on 1 location"), *i);
  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Breakpoint 1: regex(\"int\")"), *i);
  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  i += 2;
  JUST_ASSERT_EQUAL(raw_text("No breakpoints currently set"), *i);
}

JUST_TEST_CASE(test_mdb_evaluate_reevaluate_clears_breakpoints)
{
  const auto r = run_metashell({command("#msh mdb"), command("evaluate int"),
                                command("rbreak int"), command("break list"),
                                command("evaluate"), command("break list")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint \"int\" will stop the execution on 1 location"), *i);
  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Breakpoint 1: regex(\"int\")"), *i);
  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  i += 2;
  JUST_ASSERT_EQUAL(raw_text("No breakpoints currently set"), *i);
}

JUST_TEST_CASE(test_mdb_evaluate_failure_clears_breakpoints)
{
  const auto r = run_metashell(
      {command("#msh mdb"), command("evaluate int"), command("rbreak int"),
       command("break list"), command("evaluate asd"), command("break list")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint \"int\" will stop the execution on 1 location"), *i);
  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Breakpoint 1: regex(\"int\")"), *i);
  i += 2;
  JUST_ASSERT_EQUAL(error(_), *i);
  i += 2;
  JUST_ASSERT_EQUAL(raw_text("No breakpoints currently set"), *i);
}
