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
#include <metashell_system_test/backtrace.hpp>
#include <metashell_system_test/raw_text.hpp>
#include <metashell_system_test/json_generator.hpp>
#include <metashell_system_test/run_metashell.hpp>

#include "test_metaprograms.hpp"
#include "fib.hpp"

#include <just/test.hpp>

using namespace metashell_system_test;

JUST_TEST_CASE(test_mdb_backtrace_without_evaluation)
{
  const auto r = run_metashell({command("#msh mdb"), command("backtrace")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(error("Metaprogram not evaluated yet"), *i);
}

JUST_TEST_CASE(test_mdb_backtrace_unstepped_fibonacci)
{
  const auto r = run_metashell({command(fibonacci_mp), command("#msh mdb"),
                                command("evaluate int_<fib<10>::value>"),
                                command("backtrace")});

  auto i = r.begin() + 5;

  JUST_ASSERT_EQUAL(backtrace({frame(type("int_<fib<10>::value>"))}), *i);
}

JUST_TEST_CASE(test_mdb_backtrace_when_metaprogram_finished)
{
  const auto r = run_metashell({command("#msh mdb"), command("evaluate int"),
                                command("continue"), command("backtrace")});

  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i);
  ++i;
  JUST_ASSERT_EQUAL(type("int"), *i);
}

JUST_TEST_CASE(test_mdb_backtrace_when_metaprogram_finished_in_full_mode)
{
  const auto r =
      run_metashell({command("#msh mdb"), command("evaluate -full int"),
                     command("continue"), command("backtrace")});

  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i);
  ++i;
  JUST_ASSERT_EQUAL(type("int"), *i);
}

JUST_TEST_CASE(test_mdb_backtrace_1_stepped_fibonacci)
{
  const auto r = run_metashell({command(fibonacci_mp), command("#msh mdb"),
                                command("evaluate int_<fib<10>::value>"),
                                command("step"), command("backtrace")});

  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(
      backtrace(
          {frame(fib<10>(), _, _, instantiation_kind::template_instantiation),
           frame(type("int_<fib<10>::value>"))}),
      *i);
}

JUST_TEST_CASE(test_mdb_backtrace_2_stepped_fibonacci)
{
  const auto r = run_metashell({command(fibonacci_mp), command("#msh mdb"),
                                command("evaluate int_<fib<10>::value>"),
                                command("step 2"), command("backtrace")});

  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(
      backtrace(
          {frame(fib<8>(), _, _, instantiation_kind::template_instantiation),
           frame(fib<10>(), _, _, instantiation_kind::template_instantiation),
           frame(type("int_<fib<10>::value>"))}),
      *i);
}

JUST_TEST_CASE(test_mdb_backtrace_3_stepped_fibonacci)
{
  const auto r = run_metashell({command(fibonacci_mp), command("#msh mdb"),
                                command("evaluate int_<fib<10>::value>"),
                                command("step 3"), command("backtrace")});

  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(
      backtrace(
          {frame(fib<6>(), _, _, instantiation_kind::template_instantiation),
           frame(fib<8>(), _, _, instantiation_kind::template_instantiation),
           frame(fib<10>(), _, _, instantiation_kind::template_instantiation),
           frame(type("int_<fib<10>::value>"))}),
      *i);
}

JUST_TEST_CASE(test_mdb_backtrace_garbage_argument)
{
  const auto r = run_metashell({command(fibonacci_mp), command("#msh mdb"),
                                command("evaluate int_<fib<10>::value>"),
                                command("backtrace asd")});

  auto i = r.begin() + 5;

  JUST_ASSERT_EQUAL(error("This command doesn't accept arguments"), *i);
}

JUST_TEST_CASE(test_mdb_backtrace_bt_alias)
{
  const auto r =
      run_metashell({command(fibonacci_mp), command("#msh mdb"),
                     command("evaluate int_<fib<10>::value>"), command("step"),
                     command("backtrace"), command("bt")});

  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(
      backtrace(
          {frame(fib<10>(), _, _, instantiation_kind::template_instantiation),
           frame(type("int_<fib<10>::value>"))}),
      *i);

  ++i;
  ++i;

  JUST_ASSERT_EQUAL(
      backtrace(
          {frame(fib<10>(), _, _, instantiation_kind::template_instantiation),
           frame(type("int_<fib<10>::value>"))}),
      *i);
}

JUST_TEST_CASE(test_mdb_backtrace_on_error)
{
  const auto r =
      run_metashell({command(missing_value_fibonacci_mp), command("#msh mdb"),
                     command("evaluate int_<fib<5>::value>"),
                     command("continue"), command("backtrace")});

  auto i = r.begin() + 8;

  JUST_ASSERT_EQUAL(
      backtrace(
          {frame(fib<0>(), _, _, instantiation_kind::memoization),
           frame(fib<2>(), _, _, instantiation_kind::template_instantiation),
           frame(fib<3>(), _, _, instantiation_kind::template_instantiation),
           frame(fib<5>(), _, _, instantiation_kind::template_instantiation),
           frame(type("int_<fib<5>::value>"))}),
      *i);
}
