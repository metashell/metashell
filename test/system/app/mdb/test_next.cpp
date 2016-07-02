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

JUST_TEST_CASE(test_mdb_next_without_evaluation)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  JUST_ASSERT_EQUAL(
      error("Metaprogram not evaluated yet"), mi.command("next").front());
}

JUST_TEST_CASE(test_mdb_next_garbage_argument)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<2>::value>");

  JUST_ASSERT_EQUAL(
      error("Argument parsing failed"), mi.command("next asd").front());
}

JUST_TEST_CASE(test_mdb_next_fib_from_root)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int_<55>")), to_json_string(prompt("(mdb)"))},
      mi.command("next"));
}

JUST_TEST_CASE(test_mdb_next_minus_1_multi_fib_from_after_step)
{
  metashell_instance mi;
  mi.command(multi_fibonacci_mp);
  mi.command("#msh mdb int_<multi_fib<10>::value>");

  JUST_ASSERT_EQUAL(
      frame(type("multi_fib<4>"), _, _, instantiation_kind::memoization),
      mi.command("step 15").front());

  JUST_ASSERT_EQUAL(
      frame(type("multi_fib<5>"), _, _, instantiation_kind::memoization),
      mi.command("next").front());

  JUST_ASSERT_EQUAL(
      frame(type("multi_fib<6>"), _, _, instantiation_kind::memoization),
      mi.command("next").front());

  JUST_ASSERT_EQUAL(frame(type("multi_fib<6>"), _, _,
                          instantiation_kind::template_instantiation),
                    mi.command("next -1").front());
}
