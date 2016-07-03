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

JUST_TEST_CASE(test_mdb_frame_without_evaluation)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  JUST_ASSERT_EQUAL(
      error("Metaprogram not evaluated yet"), mi.command("frame").front());
}

JUST_TEST_CASE(test_mdb_frame_before_starting)
{
  metashell_instance mi;
  mi.command("#msh mdb int");

  JUST_ASSERT_EQUAL(frame(type("int")), mi.command("frame 0").front());
}

JUST_TEST_CASE(test_mdb_frame_fib_step_1)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("step");

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      mi.command("frame 0").front());

  JUST_ASSERT_EQUAL(
      frame(type("int_<fib<10>::value>")), mi.command("frame 1").front());
}

JUST_TEST_CASE(test_mdb_frame_fib_step_2)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("step 2");

  JUST_ASSERT_EQUAL(
      frame(type("fib<8>"), _, _, instantiation_kind::template_instantiation),
      mi.command("frame 0").front());

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      mi.command("frame 1").front());

  JUST_ASSERT_EQUAL(
      frame(type("int_<fib<10>::value>")), mi.command("frame 2").front());
}

JUST_TEST_CASE(test_mdb_frame_fib_at_end)
{
  metashell_instance mi;
  mi.command("#msh mdb int");

  JUST_ASSERT_EQUAL(
      raw_text("Metaprogram finished"), mi.command("continue").front());
  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int")), to_json_string(prompt("(mdb)"))},
      mi.command("frame 0"));
}

JUST_TEST_CASE(test_mdb_frame_fib_no_argument)
{
  metashell_instance mi;
  mi.command("#msh mdb int");

  JUST_ASSERT_EQUAL(
      error("Argument parsing failed"), mi.command("frame").front());
}

JUST_TEST_CASE(test_mdb_frame_fib_garbage_argument)
{
  metashell_instance mi;
  mi.command("#msh mdb int");

  JUST_ASSERT_EQUAL(
      error("Argument parsing failed"), mi.command("frame asd").front());
}

JUST_TEST_CASE(test_mdb_frame_fib_out_of_range_arg_negative)
{
  metashell_instance mi;
  mi.command("#msh mdb int");

  JUST_ASSERT_EQUAL(
      error("Frame index out of range"), mi.command("frame -1").front());
}

JUST_TEST_CASE(test_mdb_frame_fib_out_of_range_arg_too_large)
{
  metashell_instance mi;
  mi.command("#msh mdb int");

  JUST_ASSERT_EQUAL(
      error("Frame index out of range"), mi.command("frame 1").front());
}
