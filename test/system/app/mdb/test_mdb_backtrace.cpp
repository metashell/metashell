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

#include <metashell/system_test/backtrace.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/raw_text.hpp>

#include "fib.hpp"
#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell::system_test;

using pattern::_;

JUST_TEST_CASE(test_mdb_backtrace_without_evaluation)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  JUST_ASSERT_EQUAL(
      error("Metaprogram not evaluated yet"), mi.command("backtrace").front());
}

JUST_TEST_CASE(test_mdb_backtrace_unstepped_fibonacci)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(backtrace({frame(type("int_<fib<10>::value>"))}),
                    mi.command("backtrace").front());
}

JUST_TEST_CASE(test_mdb_backtrace_when_metaprogram_finished)
{
  metashell_instance mi;
  mi.command("#msh mdb");
  mi.command("evaluate int");

  JUST_ASSERT_EQUAL(
      raw_text("Metaprogram finished"), mi.command("continue").front());
  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int")), to_json_string(prompt("(mdb)"))},
      mi.command("backtrace"));
}

JUST_TEST_CASE(test_mdb_backtrace_when_metaprogram_finished_in_full_mode)
{
  metashell_instance mi;
  mi.command("#msh mdb");
  mi.command("evaluate -full int");

  JUST_ASSERT_EQUAL(
      raw_text("Metaprogram finished"), mi.command("continue").front());
  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int")), to_json_string(prompt("(mdb)"))},
      mi.command("backtrace"));
}

JUST_TEST_CASE(test_mdb_backtrace_1_stepped_fibonacci)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<10>::value>");
  mi.command("step");

  JUST_ASSERT_EQUAL(
      backtrace(
          {frame(fib<10>(), _, _, instantiation_kind::template_instantiation),
           frame(type("int_<fib<10>::value>"))}),
      mi.command("backtrace").front());
}

JUST_TEST_CASE(test_mdb_backtrace_2_stepped_fibonacci)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<10>::value>");
  mi.command("step 2");

  JUST_ASSERT_EQUAL(
      backtrace(
          {frame(fib<8>(), _, _, instantiation_kind::template_instantiation),
           frame(fib<10>(), _, _, instantiation_kind::template_instantiation),
           frame(type("int_<fib<10>::value>"))}),
      mi.command("backtrace").front());
}

JUST_TEST_CASE(test_mdb_backtrace_3_stepped_fibonacci)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<10>::value>");
  mi.command("step 3");

  JUST_ASSERT_EQUAL(
      backtrace(
          {frame(fib<6>(), _, _, instantiation_kind::template_instantiation),
           frame(fib<8>(), _, _, instantiation_kind::template_instantiation),
           frame(fib<10>(), _, _, instantiation_kind::template_instantiation),
           frame(type("int_<fib<10>::value>"))}),
      mi.command("backtrace").front());
}

JUST_TEST_CASE(test_mdb_backtrace_garbage_argument)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(error("This command doesn't accept arguments"),
                    mi.command("backtrace asd").front());
}

JUST_TEST_CASE(test_mdb_backtrace_bt_alias)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<10>::value>");
  mi.command("step");

  JUST_ASSERT_EQUAL(
      backtrace(
          {frame(fib<10>(), _, _, instantiation_kind::template_instantiation),
           frame(type("int_<fib<10>::value>"))}),
      mi.command("backtrace").front());

  JUST_ASSERT_EQUAL(
      backtrace(
          {frame(fib<10>(), _, _, instantiation_kind::template_instantiation),
           frame(type("int_<fib<10>::value>"))}),
      mi.command("bt").front());
}

JUST_TEST_CASE(test_mdb_backtrace_on_error)
{
  metashell_instance mi;
  mi.command(missing_value_fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<5>::value>");
  mi.command("continue");

  JUST_ASSERT_EQUAL(
      backtrace(
          {frame(fib<0>(), _, _, instantiation_kind::memoization),
           frame(fib<2>(), _, _, instantiation_kind::template_instantiation),
           frame(fib<3>(), _, _, instantiation_kind::template_instantiation),
           frame(fib<5>(), _, _, instantiation_kind::template_instantiation),
           frame(type("int_<fib<5>::value>"))}),
      mi.command("backtrace").front());
}
