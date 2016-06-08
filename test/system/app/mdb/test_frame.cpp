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
#include <metashell_system_test/frame.hpp>
#include <metashell_system_test/json_generator.hpp>
#include <metashell_system_test/prompt.hpp>
#include <metashell_system_test/raw_text.hpp>
#include <metashell_system_test/run_metashell.hpp>
#include <metashell_system_test/type.hpp>

#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell_system_test;

using pattern::_;

JUST_TEST_CASE(test_mdb_frame_without_evaluation)
{
  const auto r = run_metashell({command("#msh mdb"), command("frame")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(error("Metaprogram not evaluated yet"), *i);
}

JUST_TEST_CASE(test_mdb_frame_before_starting)
{
  const auto r = run_metashell({command("#msh mdb int"), command("frame 0")});

  auto i = r.begin() + 3;

  JUST_ASSERT_EQUAL(frame(type("int")), *i);
}

JUST_TEST_CASE(test_mdb_frame_fib_step_1)
{
  const auto r = run_metashell(
      {command(fibonacci_mp), command("#msh mdb int_<fib<10>::value>"),
       command("step"), command("frame 0"), command("frame 1")});

  auto i = r.begin() + 6;

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      *i);

  i += 2;

  JUST_ASSERT_EQUAL(frame(type("int_<fib<10>::value>")), *i);
}

JUST_TEST_CASE(test_mdb_frame_fib_step_2)
{
  const auto r = run_metashell({command(fibonacci_mp),
                                command("#msh mdb int_<fib<10>::value>"),
                                command("step 2"), command("frame 0"),
                                command("frame 1"), command("frame 2")});

  auto i = r.begin() + 6;

  JUST_ASSERT_EQUAL(
      frame(type("fib<8>"), _, _, instantiation_kind::template_instantiation),
      *i);

  i += 2;

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      *i);

  i += 2;

  JUST_ASSERT_EQUAL(frame(type("int_<fib<10>::value>")), *i);
}

JUST_TEST_CASE(test_mdb_frame_fib_at_end)
{
  const auto r = run_metashell(
      {command("#msh mdb int"), command("continue"), command("frame 0")});

  auto i = r.begin() + 6;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int"), *i);
}

JUST_TEST_CASE(test_mdb_frame_fib_no_argument)
{
  const auto r = run_metashell({command("#msh mdb int"), command("frame")});

  auto i = r.begin() + 3;

  JUST_ASSERT_EQUAL(error("Argument parsing failed"), *i);
}

JUST_TEST_CASE(test_mdb_frame_fib_garbage_argument)
{
  const auto r = run_metashell({command("#msh mdb int"), command("frame asd")});

  auto i = r.begin() + 3;

  JUST_ASSERT_EQUAL(error("Argument parsing failed"), *i);
}

JUST_TEST_CASE(test_mdb_frame_fib_out_of_range_arg_negative)
{
  const auto r = run_metashell({command("#msh mdb int"), command("frame -1")});

  auto i = r.begin() + 3;

  JUST_ASSERT_EQUAL(error("Frame index out of range"), *i);
}

JUST_TEST_CASE(test_mdb_frame_fib_out_of_range_arg_too_large)
{
  const auto r = run_metashell({command("#msh mdb int"), command("frame 1")});

  auto i = r.begin() + 3;

  JUST_ASSERT_EQUAL(error("Frame index out of range"), *i);
}
