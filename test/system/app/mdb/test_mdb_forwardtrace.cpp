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

#include <metashell/system_test/call_graph.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/json_generator.hpp>
#include <metashell/system_test/raw_text.hpp>
#include <metashell/system_test/run_metashell.hpp>

#include "fib.hpp"
#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell::system_test;

using pattern::_;

// clang-format off

JUST_TEST_CASE(test_mdb_forwardtrace_without_evaluation) {
  const auto r =
    run_metashell(
      {
        command("#msh mdb"),
        command("forwardtrace")
      }
    );

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(error("Metaprogram not evaluated yet"), *i);
}

JUST_TEST_CASE(test_mdb_forwardtrace_garbage_argument) {
  const auto r =
    run_metashell(
      {
        command("#msh mdb"),
        command("evaluate int"),
        command("forwardtrace asd")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(error("Argument parsing failed"), *i);
}

JUST_TEST_CASE(test_mdb_forwardtrace_int) {
  const auto r =
    run_metashell(
      {
        command("#msh mdb"),
        command("evaluate int"),
        command("forwardtrace")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(type("int")), 0, 1},
        {frame(type("int"), _, _, instantiation_kind::non_template_type), 1, 0}
      }
    ),
    *i
  );
}

JUST_TEST_CASE(test_mdb_forwardtrace_int_in_full_mode) {
  const auto r =
    run_metashell(
      {
        command("#msh mdb"),
        command("evaluate -full int"),
        command("forwardtrace")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(type("int")), 0, 1},
        {frame(type("int")), 1, 0}
      }
    ),
    *i
  );
}

JUST_TEST_CASE(test_mdb_forwardtrace_when_metaprogram_finished) {
  const auto r =
    run_metashell(
      {
        command("#msh mdb"),
        command("evaluate int"),
        command("continue"),
        command("forwardtrace")
      }
    );

  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i); ++i;
  JUST_ASSERT_EQUAL(type("int"), *i);
}

JUST_TEST_CASE(test_mdb_forwardtrace_when_metaprogram_finished_in_full_mode) {
  const auto r =
    run_metashell(
      {
        command("#msh mdb"),
        command("evaluate -full int"),
        command("continue"),
        command("forwardtrace")
      }
    );

  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i); ++i;
  JUST_ASSERT_EQUAL(type("int"), *i);
}

JUST_TEST_CASE(test_mdb_forwardtrace_int_with_ft) {
  const auto r =
    run_metashell(
      {
        command("#msh mdb"),
        command("evaluate int"),
        command("ft")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(type("int")), 0, 1},
        {frame(type("int"), _, _, instantiation_kind::non_template_type), 1, 0}
      }
    ),
    *i
  );
}

JUST_TEST_CASE(test_mdb_forwardtrace_from_root) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb"),
        command("evaluate int_<fib<5>::value>"),
        command("forwardtrace")
      }
    );

  auto i = r.begin() + 5;

  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(type("int_<fib<5>::value>")), 0, 3},
        {frame( fib<5>(), _, _, instantiation_kind::template_instantiation), 1, 4},
        {frame(  fib<3>(), _, _, instantiation_kind::template_instantiation), 2, 3},
        {frame(   fib<1>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(   fib<2>(), _, _, instantiation_kind::template_instantiation), 3, 2},
        {frame(    fib<0>(), _, _, instantiation_kind::memoization), 4, 0},
        {frame(    fib<1>(), _, _, instantiation_kind::memoization), 4, 0},
        {frame(   fib<2>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(  fib<3>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame(  fib<4>(), _, _, instantiation_kind::template_instantiation), 2, 2},
        {frame(   fib<2>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(   fib<3>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(  fib<4>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame( fib<5>(), _, _, instantiation_kind::memoization), 1, 0},
        {frame( type("int_<5>"), _, _, instantiation_kind::template_instantiation), 1, 0}
      }
    ),
    *i
  );
}

JUST_TEST_CASE(test_mdb_forwardtrace_from_root_in_full_mode) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb"),
        command("evaluate -full int_<fib<5>::value>"),
        command("forwardtrace")
      }
    );

  auto i = r.begin() + 5;

  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(type("int_<fib<5>::value>")), 0, 2},
        {frame( fib<5>()), 1, 2},
        {frame(  fib<3>()), 2, 2},
        {frame(   fib<1>()), 3, 0},
        {frame(   fib<2>()), 3, 2},
        {frame(    fib<0>()), 4, 0},
        {frame(    fib<1>()), 4, 0},
        {frame(  fib<4>()), 2, 2},
        {frame(   fib<2>()), 3, 2},
        {frame(    fib<0>()), 4, 0},
        {frame(    fib<1>()), 4, 0},
        {frame(   fib<3>()), 3, 2},
        {frame(    fib<1>()), 4, 0},
        {frame(    fib<2>()), 4, 2},
        {frame(     fib<0>()), 5, 0},
        {frame(     fib<1>()), 5, 0},
        {frame( type("int_<5>")), 1,0}
      }
    ),
    *i
  );
}

JUST_TEST_CASE(test_mdb_forwardtrace_from_memoization) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb"),
        command("evaluate int_<fib<5>::value>"),
        command("rbreak fib<5>"),
        command("continue 2"),
        command("forwardtrace")
      }
    );

  auto i = r.begin() + 10;

  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame( fib<5>(), _, _, instantiation_kind::memoization), 0, 0}
      }
    ),
    *i
  );
}

JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_1) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb"),
        command("evaluate int_<fib<5>::value>"),
        command("step"),
        command("forwardtrace")
      }
    );

  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(fib<5>(), _, _, instantiation_kind::template_instantiation), 0, 4},
        {frame( fib<3>(), _, _, instantiation_kind::template_instantiation), 1, 3},
        {frame(  fib<1>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame(  fib<2>(), _, _, instantiation_kind::template_instantiation), 2, 2},
        {frame(   fib<0>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(   fib<1>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(  fib<2>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame( fib<3>(), _, _, instantiation_kind::memoization), 1, 0},
        {frame( fib<4>(), _, _, instantiation_kind::template_instantiation), 1, 2},
        {frame(  fib<2>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame(  fib<3>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame( fib<4>(), _, _, instantiation_kind::memoization), 1, 0}
      }
    ),
    *i
  );
}

JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_1_in_full_mode) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb"),
        command("evaluate -full int_<fib<5>::value>"),
        command("step"),
        command("forwardtrace")
      }
    );

  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(fib<5>()), 0, 2},
        {frame( fib<3>()), 1, 2},
        {frame(  fib<1>()), 2, 0},
        {frame(  fib<2>()), 2, 2},
        {frame(   fib<0>()), 3, 0},
        {frame(   fib<1>()), 3, 0},
        {frame( fib<4>()), 1, 2},
        {frame(  fib<2>()), 2, 2},
        {frame(   fib<0>()), 3, 0},
        {frame(   fib<1>()), 3, 0},
        {frame(  fib<3>()), 2, 2},
        {frame(   fib<1>()), 3, 0},
        {frame(   fib<2>()), 3, 2},
        {frame(    fib<0>()), 4, 0},
        {frame(    fib<1>()), 4, 0}
      }
    ),
    *i
  );
}

JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_1_with_limit_0) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb"),
        command("evaluate int_<fib<5>::value>"),
        command("step"),
        command("forwardtrace 0")
      }
    );

  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame( fib<5>(), _, _, instantiation_kind::template_instantiation), 0, 0}
      }
    ),
    *i
  );
}

JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_1_with_limit_1) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb"),
        command("evaluate int_<fib<5>::value>"),
        command("step"),
        command("forwardtrace 1")
      }
    );

  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(fib<5>(), _, _, instantiation_kind::template_instantiation), 0, 4},
        {frame( fib<3>(), _, _, instantiation_kind::template_instantiation), 1, 0},
        {frame( fib<3>(), _, _, instantiation_kind::memoization), 1, 0},
        {frame( fib<4>(), _, _, instantiation_kind::template_instantiation), 1, 0},
        {frame( fib<4>(), _, _, instantiation_kind::memoization), 1, 0}
      }
    ),
    *i
  );
}

JUST_TEST_CASE(
    test_mdb_forwardtrace_ft_from_step_1_with_limit_1_in_full_mode)
{
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb"),
        command("evaluate -full int_<fib<5>::value>"),
        command("step"),
        command("forwardtrace 1")
      }
    );

  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(fib<5>()), 0, 2},
        {frame( fib<3>()), 1, 0},
        {frame( fib<4>()), 1, 0}
      }
    ),
    *i
  );
}

JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_2_with_limit_2) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb"),
        command("evaluate int_<fib<5>::value>"),
        command("step"),
        command("forwardtrace 2")
      }
    );

  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(fib<5>(), _, _, instantiation_kind::template_instantiation), 0, 4},
        {frame( fib<3>(), _, _, instantiation_kind::template_instantiation), 1, 3},
        {frame(  fib<1>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame(  fib<2>(), _, _, instantiation_kind::template_instantiation), 2, 0},
        {frame(  fib<2>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame( fib<3>(), _, _, instantiation_kind::memoization), 1, 0},
        {frame( fib<4>(), _, _, instantiation_kind::template_instantiation), 1, 2},
        {frame(  fib<2>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame(  fib<3>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame( fib<4>(), _, _, instantiation_kind::memoization), 1, 0}
      }
    ),
    *i
  );
}

JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_2_with_limit_100) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb"),
        command("evaluate int_<fib<5>::value>"),
        command("step"),
        command("forwardtrace 100")
      }
    );

  auto i = r.begin() + 7;

  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(fib<5>(), _, _, instantiation_kind::template_instantiation), 0, 4},
        {frame( fib<3>(), _, _, instantiation_kind::template_instantiation), 1, 3},
        {frame(  fib<1>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame(  fib<2>(), _, _, instantiation_kind::template_instantiation), 2, 2},
        {frame(   fib<0>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(   fib<1>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(  fib<2>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame( fib<3>(), _, _, instantiation_kind::memoization), 1, 0},
        {frame( fib<4>(), _, _, instantiation_kind::template_instantiation), 1, 2},
        {frame(  fib<2>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame(  fib<3>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame( fib<4>(), _, _, instantiation_kind::memoization), 1, 0}
      }
    ),
    *i
  );
}

JUST_TEST_CASE(test_mdb_forwardtrace_from_root_on_errored_metaprogram) {
  const auto r =
    run_metashell(
      {
        command(missing_value_fibonacci_mp),
        command("#msh mdb"),
        command("evaluate int_<fib<5>::value>"),
        command("forwardtrace")
      }
    );

  auto i = r.begin() + 5;

  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(type("int_<fib<5>::value>")), 0, 1},
        {frame(fib<5>(), _, _, instantiation_kind::template_instantiation), 1, 1},
        {frame( fib<3>(), _, _, instantiation_kind::template_instantiation), 2, 2},
        {frame(  fib<1>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(  fib<2>(), _, _, instantiation_kind::template_instantiation), 3, 1},
        {frame(   fib<0>(), _, _, instantiation_kind::memoization), 4, 0}
      }
    ),
    *i
  );
}

JUST_TEST_CASE(test_mdb_forwardtrace_when_evaluating_environment_fib_2_3) {
  const auto r =
    run_metashell(
      {
        command(
          fibonacci_mp +
         "int_<fib<5>::value> x;"
         "int_<fib<6>::value> y;"
        ),
        command("#msh mdb"),
        command("evaluate -"),
        command("forwardtrace")
      }
    );

  auto i = r.begin() + 5;

  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(type("<environment>")), 0, 8},
        {frame( fib<5>(), _, _, instantiation_kind::template_instantiation), 1, 4},
        {frame(  fib<3>(), _, _, instantiation_kind::template_instantiation), 2, 3},
        {frame(   fib<1>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(   fib<2>(), _, _, instantiation_kind::template_instantiation), 3, 2},
        {frame(    fib<0>(), _, _, instantiation_kind::memoization), 4, 0},
        {frame(    fib<1>(), _, _, instantiation_kind::memoization), 4, 0},
        {frame(   fib<2>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(  fib<3>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame(  fib<4>(), _, _, instantiation_kind::template_instantiation), 2, 2},
        {frame(   fib<2>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(   fib<3>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(  fib<4>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame( fib<5>(), _, _, instantiation_kind::memoization), 1, 0},
        {frame( type("int_<5>"), _, _, instantiation_kind::template_instantiation), 1, 0},
        {frame( type("int_<5>"), _, _, instantiation_kind::memoization), 1, 0},
        {frame( fib<6>(), _, _, instantiation_kind::template_instantiation), 1, 2},
        {frame(  fib<4>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame(  fib<5>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame( fib<6>(), _, _, instantiation_kind::memoization), 1, 0},
        {frame( type("int_<8>"), _, _, instantiation_kind::template_instantiation), 1, 0},
        {frame( type("int_<8>"), _, _, instantiation_kind::memoization), 1, 0}
      }
    ),
    *i
  );
}

JUST_TEST_CASE(test_mdb_forwardtrace_sfinae_v1) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_sfinae_mp),
        command("#msh mdb"),
        command("evaluate decltype(foo<4>())"),
        command("forwardtrace")
      }
    );

  auto i = r.begin() + 5;

  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(type("decltype(foo<4>())")), 0, 5},
        {frame( type("foo"), _, _, instantiation_kind::explicit_template_argument_substitution), 1, 4},
        {frame(  v2::fib<4>(), _, _, instantiation_kind::template_instantiation), 2, 4},
        {frame(   v2::fib<2>(), _, _, instantiation_kind::template_instantiation), 3, 2},
        {frame(    v2::fib<0>(), _, _, instantiation_kind::memoization), 4, 0},
        {frame(    v2::fib<1>(), _, _, instantiation_kind::memoization), 4, 0},
        {frame(   v2::fib<2>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(   v2::fib<3>(), _, _, instantiation_kind::template_instantiation), 3, 2},
        {frame(    v2::fib<1>(), _, _, instantiation_kind::memoization), 4, 0},
        {frame(    v2::fib<2>(), _, _, instantiation_kind::memoization), 4, 0},
        {frame(   v2::fib<3>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(  v2::fib<4>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame(  type("enable_if<false, char>"), _, _, instantiation_kind::template_instantiation), 2, 0},
        {frame(  type("enable_if<false, char>"), _, _, instantiation_kind::memoization), 2, 0},
        {frame( type("foo"), _, _, instantiation_kind::explicit_template_argument_substitution), 1, 6},
        {frame(  v1::fib<4>(), _, _, instantiation_kind::template_instantiation), 2, 4},
        {frame(   v1::fib<2>(), _, _, instantiation_kind::template_instantiation), 3, 2},
        {frame(    v1::fib<0>(), _, _, instantiation_kind::memoization), 4, 0},
        {frame(    v1::fib<1>(), _, _, instantiation_kind::memoization), 4, 0},
        {frame(   v1::fib<2>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(   v1::fib<3>(), _, _, instantiation_kind::template_instantiation), 3, 2},
        {frame(    v1::fib<1>(), _, _, instantiation_kind::memoization), 4, 0},
        {frame(    v1::fib<2>(), _, _, instantiation_kind::memoization), 4, 0},
        {frame(   v1::fib<3>(), _, _, instantiation_kind::memoization), 3, 0},
        {frame(  v1::fib<4>(), _, _, instantiation_kind::memoization), 2, 0},
        {frame(  type("enable_if<true, type-parameter-0-0>"), _, _, instantiation_kind::deduced_template_argument_substitution), 2, 0},
        {frame(  type("enable_if<true, void>"), _, _, instantiation_kind::template_instantiation), 2, 0},
        {frame(  type("enable_if<true, void>"), _, _, instantiation_kind::memoization), 2, 0},
        {frame(  type("enable_if<true, void>"), _, _, instantiation_kind::memoization), 2, 0},
        {frame( type("foo"), _, _, instantiation_kind::deduced_template_argument_substitution), 1, 0},
        {frame( type("foo<4>"), _, _, instantiation_kind::template_instantiation), 1, 0},
        {frame( type("void"), _, _, instantiation_kind::non_template_type), 1, 0}
      }
    ),
    *i
  );
}
