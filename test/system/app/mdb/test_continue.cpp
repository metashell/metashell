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

#include <metashell/system_test/error.hpp>
#include <metashell/system_test/frame.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/raw_text.hpp>
#include <metashell/system_test/type.hpp>

#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell::system_test;

using pattern::_;

JUST_TEST_CASE(test_mdb_continue_without_evaluation)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  JUST_ASSERT_EQUAL(
      error("Metaprogram not evaluated yet"), mi.command("continue").front());
}

JUST_TEST_CASE(test_mdb_continue_garbage_argument)
{
  metashell_instance mi;
  mi.command("#msh mdb int");

  JUST_ASSERT_EQUAL(
      error("Argument parsing failed"), mi.command("continue asd").front());
}

JUST_TEST_CASE(test_mdb_continue_fibonacci_no_breakpoint)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int_<55>")), to_json_string(prompt("(mdb)"))},
      mi.command("continue"));
}

JUST_TEST_CASE(test_mdb_continue_fibonacci_reevaluation_removes_breakpoints)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("rbreak fib<0>");
  mi.command("evaluate int_<fib<10>::value>");

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int_<55>")), to_json_string(prompt("(mdb)"))},
      mi.command("continue"));
}

JUST_TEST_CASE(test_mdb_continue_fibonacci_1_breakpoint)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("rbreak fib<0>");

  const std::vector<json_string> r_cont = mi.command("continue");
  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint 1: regex(\"fib<0>\") reached"), r_cont[0]);
  JUST_ASSERT_EQUAL(
      frame(type("fib<0>"), _, _, instantiation_kind::memoization), r_cont[1]);
}

JUST_TEST_CASE(test_mdb_continue_2_fibonacci_1_breakpoint)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("rbreak fib<5>");

  const std::vector<json_string> r_cont = mi.command("continue 2");
  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint 1: regex(\"fib<5>\") reached"), r_cont[0]);
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::memoization), r_cont[1]);
}

JUST_TEST_CASE(test_mdb_continue_twice_fibonacci_1_breakpoint)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("rbreak fib<5>");

  const std::vector<json_string> r_cont1 = mi.command("continue");
  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint 1: regex(\"fib<5>\") reached"), r_cont1[0]);
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::template_instantiation),
      r_cont1[1]);

  const std::vector<json_string> r_cont2 = mi.command("continue");
  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint 1: regex(\"fib<5>\") reached"), r_cont2[0]);
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::memoization), r_cont2[1]);
}

JUST_TEST_CASE(test_mdb_continue_fibonacci_2_breakpoints)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("rbreak fib<5>");
  mi.command("rbreak fib<6>");

  const std::vector<json_string> r_cont1 = mi.command("continue");
  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint 2: regex(\"fib<6>\") reached"), r_cont1[0]);
  JUST_ASSERT_EQUAL(
      frame(type("fib<6>"), _, _, instantiation_kind::template_instantiation),
      r_cont1[1]);

  const std::vector<json_string> r_cont2 = mi.command("continue");
  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint 1: regex(\"fib<5>\") reached"), r_cont2[0]);
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::template_instantiation),
      r_cont2[1]);
}

JUST_TEST_CASE(test_mdb_continue_2_fibonacci_2_breakpoints)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("rbreak fib<5>");
  mi.command("rbreak fib<6>");

  const std::vector<json_string> r_cont = mi.command("continue 2");
  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint 1: regex(\"fib<5>\") reached"), r_cont[0]);
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::template_instantiation),
      r_cont[1]);
}

JUST_TEST_CASE(test_mdb_continue_10_fibonacci_2_breakpoints)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("rbreak fib<5>");
  mi.command("rbreak fib<6>");

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int_<55>")), to_json_string(prompt("(mdb)"))},
      mi.command("continue 10"));
}

JUST_TEST_CASE(test_mdb_continue_0_fibonacci_1_breakpoint)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("rbreak fib<6>");

  // continue 0 doesn't print anything at start
  JUST_ASSERT_EQUAL(prompt("(mdb)"), mi.command("continue 0").front());
}

JUST_TEST_CASE(test_mdb_continue_minus_1_at_start)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"),
                    mi.command("continue -1").front());
}

JUST_TEST_CASE(test_mdb_continue_minus_2_at_start)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"),
                    mi.command("continue -2").front());
}

JUST_TEST_CASE(test_mdb_continue_minus_1_with_preceding_breakpoint)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("rbreak fib<6>");
  mi.command("rbreak fib<5>");

  const std::vector<json_string> r_cont1 = mi.command("continue 2");
  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint 2: regex(\"fib<5>\") reached"), r_cont1[0]);
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::template_instantiation),
      r_cont1[1]);

  const std::vector<json_string> r_cont2 = mi.command("continue -1");
  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint 1: regex(\"fib<6>\") reached"), r_cont2[0]);
  JUST_ASSERT_EQUAL(
      frame(type("fib<6>"), _, _, instantiation_kind::template_instantiation),
      r_cont2[1]);
}

JUST_TEST_CASE(test_mdb_continue_minus_1_without_preceding_breakpoint)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("rbreak fib<5>");

  const std::vector<json_string> r_cont = mi.command("continue 1");
  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint 1: regex(\"fib<5>\") reached"), r_cont[0]);
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::template_instantiation),
      r_cont[1]);

  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"),
                    mi.command("continue -1").front());
}

JUST_TEST_CASE(test_mdb_continue_to_end_and_back_to_start)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("rbreak fib<5>");

  const std::vector<json_string> r_cont = mi.command("continue 4");
  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), r_cont[0]);
  JUST_ASSERT_EQUAL(type("int_<55>"), r_cont[1]);

  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"),
                    mi.command("continue -4").front());
}

JUST_TEST_CASE(test_mdb_continue_to_end_and_back_to_start_in_full_mode)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb -full int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint \"fib<5>\" will stop the execution on 8 locations"),
      mi.command("rbreak fib<5>").front());

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int_<55>")), to_json_string(prompt("(mdb)"))},
      mi.command("continue 9"));

  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"),
                    mi.command("continue -9").front());
}
JUST_TEST_CASE(
    test_mdb_continue_to_one_before_end_and_back_to_start_in_full_mode)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb -full int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(
      raw_text("Breakpoint \"fib<5>\" will stop the execution on 8 locations"),
      mi.command("rbreak fib<5>").front());

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Breakpoint 1: regex(\"fib<5>\") reached")),
       to_json_string(frame(type("fib<5>"))), to_json_string(prompt("(mdb)"))},
      mi.command("continue 8"));

  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"),
                    mi.command("continue -8").front());
}

JUST_TEST_CASE(test_mdb_continue_to_end_will_print_error_message_if_errored)
{
  metashell_instance mi;
  mi.command(missing_value_fibonacci_mp);
  mi.command("#msh mdb int_<fib<5>::value>");

  const std::vector<json_string> r_cont = mi.command("continue");
  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), r_cont[0]);
  JUST_ASSERT_EQUAL(error(_), r_cont[1]);
}

JUST_TEST_CASE(test_mdb_continue_to_end_and_back_if_errored)
{
  metashell_instance mi;
  mi.command(missing_value_fibonacci_mp);
  mi.command("#msh mdb int_<fib<5>::value>");
  mi.command("continue");

  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"),
                    mi.command("continue -1").front());
}
