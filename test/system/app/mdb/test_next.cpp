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
#include <metashell_system_test/raw_text.hpp>
#include <metashell_system_test/run_metashell.hpp>
#include <metashell_system_test/type.hpp>

#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell_system_test;

using pattern::_;

JUST_TEST_CASE(test_mdb_next_without_evaluation)
{
  const auto r = run_metashell({command("#msh mdb"), command("next")});

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(error("Metaprogram not evaluated yet"), *i);
}

JUST_TEST_CASE(test_mdb_next_garbage_argument)
{
  const auto r = run_metashell({command(fibonacci_mp),
                                command("#msh mdb int_<fib<2>::value>"),
                                command("next asd")});

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(error("Argument parsing failed"), *i);
}

JUST_TEST_CASE(test_mdb_next_fib_from_root)
{
  const auto r = run_metashell({command(fibonacci_mp),
                                command("#msh mdb int_<fib<10>::value>"),
                                command("next")});

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int_<55>"), *i++);
}

JUST_TEST_CASE(test_mdb_next_minus_1_multi_fib_from_after_step)
{
  const auto r = run_metashell({command(multi_fibonacci_mp),
                                command("#msh mdb int_<multi_fib<10>::value>"),
                                command("step 15"), command("next"),
                                command("next"), command("next -1")});

  auto i = r.begin() + 4;
  JUST_ASSERT_EQUAL(
      frame(type("multi_fib<4>"), _, _, instantiation_kind::memoization), *i++);

  ++i;
  JUST_ASSERT_EQUAL(
      frame(type("multi_fib<5>"), _, _, instantiation_kind::memoization), *i++);

  ++i;
  JUST_ASSERT_EQUAL(
      frame(type("multi_fib<6>"), _, _, instantiation_kind::memoization), *i++);

  ++i;
  JUST_ASSERT_EQUAL(frame(type("multi_fib<6>"), _, _,
                          instantiation_kind::template_instantiation),
                    *i);
}
