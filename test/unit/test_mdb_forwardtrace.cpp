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

#include <metashell/in_memory_displayer.hpp>

#include "mdb_test_shell.hpp"
#include "util.hpp"

#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell;
using namespace metashell::data;

namespace {

const file_location f{};

} // anonymous namespace

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_without_evaluation) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("forwardtrace", d);

  JUST_ASSERT_EQUAL_CONTAINER(d.errors(), {"Metaprogram not evaluated yet"});
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_garbage_argument) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("evaluate int", d);
  sh.line_available("forwardtrace asd", d);

  JUST_ASSERT_EQUAL_CONTAINER(d.errors(), {"Argument parsing failed"});
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_int) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("evaluate int", d);
  sh.line_available("forwardtrace", d);

  const type int_("int");

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(int_, f), 0, 1},
      {frame(int_, f, f, instantiation_kind::non_template_type), 1, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_int_in_full_mode) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("evaluate -full int", d);
  sh.line_available("forwardtrace", d);

  const type int_("int");

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(int_, f), 0, 1},
      {frame(int_, f), 1, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_when_metaprogram_finished) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("evaluate int", d);
  sh.line_available("continue", d);

  d.clear();
  sh.line_available("forwardtrace", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_when_metaprogram_finished_in_full_mode) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("evaluate -full int", d);
  sh.line_available("continue", d);

  d.clear();
  sh.line_available("forwardtrace", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_int_with_ft) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("evaluate int", d);
  sh.line_available("ft", d);

  const type int_("int");

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(int_, f), 0, 1},
      {frame(int_, f, f, instantiation_kind::non_template_type), 1, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_from_root) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);
  sh.line_available("forwardtrace", d);

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(type("int_<fib<5>::value>"), f), 0, 3},
      {frame( fib<5>(), f, f, instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<3>(), f, f, instantiation_kind::template_instantiation), 2, 2},
      {frame(   fib<1>(), f, f, instantiation_kind::memoization), 3, 0},
      {frame(   fib<2>(), f, f, instantiation_kind::template_instantiation), 3, 2},
      {frame(    fib<0>(), f, f, instantiation_kind::memoization), 4, 0},
      {frame(    fib<1>(), f, f, instantiation_kind::memoization), 4, 0},
      {frame(  fib<4>(), f, f, instantiation_kind::template_instantiation), 2, 2},
      {frame(   fib<2>(), f, f, instantiation_kind::memoization), 3, 0},
      {frame(   fib<3>(), f, f, instantiation_kind::memoization), 3, 0},
      {frame( fib<5>(), f, f, instantiation_kind::memoization), 1, 0},
      {frame( type("int_<5>"), f, f, instantiation_kind::template_instantiation), 1,0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_from_root_in_full_mode) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate -full int_<fib<5>::value>", d);
  sh.line_available("forwardtrace", d);

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(type("int_<fib<5>::value>"), f), 0, 2},
      {frame( fib<5>(), f), 1, 2},
      {frame(  fib<3>(), f), 2, 2},
      {frame(   fib<1>(), f), 3, 0},
      {frame(   fib<2>(), f), 3, 2},
      {frame(    fib<0>(), f), 4, 0},
      {frame(    fib<1>(), f), 4, 0},
      {frame(  fib<4>(), f), 2, 2},
      {frame(   fib<2>(), f), 3, 2},
      {frame(    fib<0>(), f), 4, 0},
      {frame(    fib<1>(), f), 4, 0},
      {frame(   fib<3>(), f), 3, 2},
      {frame(    fib<1>(), f), 4, 0},
      {frame(    fib<2>(), f), 4, 2},
      {frame(     fib<0>(), f), 5, 0},
      {frame(     fib<1>(), f), 5, 0},
      {frame( type("int_<5>"), f), 1,0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_from_memoization) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);
  sh.line_available("rbreak fib<5>", d);
  sh.line_available("continue 2", d);
  sh.line_available("forwardtrace", d);

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame( fib<5>(), f, f, instantiation_kind::memoization), 0, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_1) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);
  sh.line_available("step", d);
  sh.line_available("forwardtrace", d);

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(fib<5>(), f, f, instantiation_kind::template_instantiation), 0, 2},
      {frame( fib<3>(), f, f, instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<1>(), f, f, instantiation_kind::memoization), 2, 0},
      {frame(  fib<2>(), f, f, instantiation_kind::template_instantiation), 2, 2},
      {frame(   fib<0>(), f, f, instantiation_kind::memoization), 3, 0},
      {frame(   fib<1>(), f, f, instantiation_kind::memoization), 3, 0},
      {frame( fib<4>(), f, f, instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<2>(), f, f, instantiation_kind::memoization), 2, 0},
      {frame(  fib<3>(), f, f, instantiation_kind::memoization), 2, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_1_in_full_mode) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate -full int_<fib<5>::value>", d);
  sh.line_available("step", d);
  sh.line_available("forwardtrace", d);

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(fib<5>(), f), 0, 2},
      {frame( fib<3>(), f), 1, 2},
      {frame(  fib<1>(), f), 2, 0},
      {frame(  fib<2>(), f), 2, 2},
      {frame(   fib<0>(), f), 3, 0},
      {frame(   fib<1>(), f), 3, 0},
      {frame( fib<4>(), f), 1, 2},
      {frame(  fib<2>(), f), 2, 2},
      {frame(   fib<0>(), f), 3, 0},
      {frame(   fib<1>(), f), 3, 0},
      {frame(  fib<3>(), f), 2, 2},
      {frame(   fib<1>(), f), 3, 0},
      {frame(   fib<2>(), f), 3, 2},
      {frame(    fib<0>(), f), 4, 0},
      {frame(    fib<1>(), f), 4, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_1_with_limit_0) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);
  sh.line_available("step", d);
  sh.line_available("forwardtrace 0", d);

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame( fib<5>(), f, f, instantiation_kind::template_instantiation), 0, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_1_with_limit_1) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);
  sh.line_available("step", d);
  sh.line_available("forwardtrace 1", d);

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(fib<5>(), f, f, instantiation_kind::template_instantiation), 0, 2},
      {frame( fib<3>(), f, f, instantiation_kind::template_instantiation), 1, 0},
      {frame( fib<4>(), f, f, instantiation_kind::template_instantiation), 1, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(
    test_mdb_forwardtrace_ft_from_step_1_with_limit_1_in_full_mode)
{
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate -full int_<fib<5>::value>", d);
  sh.line_available("step", d);
  sh.line_available("forwardtrace 1", d);

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(fib<5>(), f), 0, 2},
      {frame( fib<3>(), f), 1, 0},
      {frame( fib<4>(), f), 1, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_2_with_limit_2) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);
  sh.line_available("step", d);
  sh.line_available("forwardtrace 2", d);

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(fib<5>(), f, f, instantiation_kind::template_instantiation), 0, 2},
      {frame( fib<3>(), f, f, instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<1>(), f, f, instantiation_kind::memoization), 2, 0},
      {frame(  fib<2>(), f, f, instantiation_kind::template_instantiation), 2, 0},
      {frame( fib<4>(), f, f, instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<2>(), f, f, instantiation_kind::memoization), 2, 0},
      {frame(  fib<3>(), f, f, instantiation_kind::memoization), 2, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_2_with_limit_100) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);
  sh.line_available("step", d);
  sh.line_available("forwardtrace 100", d);

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(fib<5>(), f, f, instantiation_kind::template_instantiation), 0, 2},
      {frame( fib<3>(), f, f, instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<1>(), f, f, instantiation_kind::memoization), 2, 0},
      {frame(  fib<2>(), f, f, instantiation_kind::template_instantiation), 2, 2},
      {frame(   fib<0>(), f, f, instantiation_kind::memoization), 3, 0},
      {frame(   fib<1>(), f, f, instantiation_kind::memoization), 3, 0},
      {frame( fib<4>(), f, f, instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<2>(), f, f, instantiation_kind::memoization), 2, 0},
      {frame(  fib<3>(), f, f, instantiation_kind::memoization), 2, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_from_root_on_errored_metaprogram) {
  in_memory_displayer d;
  mdb_test_shell sh(missing_value_fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);
  sh.line_available("forwardtrace", d);

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(type("int_<fib<5>::value>"), f), 0, 1},
      {frame(fib<5>(), f, f, instantiation_kind::template_instantiation), 1, 1},
      {frame( fib<3>(), f, f, instantiation_kind::template_instantiation), 2, 2},
      {frame(  fib<1>(), f, f, instantiation_kind::memoization), 3, 0},
      {frame(  fib<2>(), f, f, instantiation_kind::template_instantiation), 3, 1},
      {frame(   fib<0>(), f, f, instantiation_kind::memoization), 4, 0},
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_when_evaluating_environment_fib_2_3) {
  in_memory_displayer d;
  mdb_test_shell sh(
    fibonacci_mp +
    "int_<fib<5>::value> x;"
    "int_<fib<6>::value> y;"
  );

  sh.line_available("evaluate -", d);
  sh.line_available("forwardtrace", d);

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(type("<environment>"), f), 0, 8},
      {frame( fib<5>(), f, f, instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<3>(), f, f, instantiation_kind::template_instantiation), 2, 2},
      {frame(   fib<1>(), f, f, instantiation_kind::memoization), 3, 0},
      {frame(   fib<2>(), f, f, instantiation_kind::template_instantiation), 3, 2},
      {frame(    fib<0>(), f, f, instantiation_kind::memoization), 4, 0},
      {frame(    fib<1>(), f, f, instantiation_kind::memoization), 4, 0},
      {frame(  fib<4>(), f, f, instantiation_kind::template_instantiation), 2, 2},
      {frame(   fib<2>(), f, f, instantiation_kind::memoization), 3, 0},
      {frame(   fib<3>(), f, f, instantiation_kind::memoization), 3, 0},
      {frame( fib<5>(), f, f, instantiation_kind::memoization), 1, 0},
      {frame( type("int_<5>"), f, f, instantiation_kind::template_instantiation), 1, 0},
      {frame( type("int_<5>"), f, f, instantiation_kind::memoization), 1, 0},
      {frame( fib<6>(), f, f, instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<4>(), f, f, instantiation_kind::memoization), 2, 0},
      {frame(  fib<5>(), f, f, instantiation_kind::memoization), 2, 0},
      {frame( fib<6>(), f, f, instantiation_kind::memoization), 1, 0},
      {frame( type("int_<8>"), f, f, instantiation_kind::template_instantiation), 1, 0},
      {frame( type("int_<8>"), f, f, instantiation_kind::memoization), 1, 0},
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_sfinae_v1) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_sfinae_mp);

  sh.line_available("evaluate decltype(foo<4>())", d);
  sh.line_available("forwardtrace", d);

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(type("decltype(foo<4>())"), f), 0, 5},
      {frame( type("foo"), f, f, instantiation_kind::explicit_template_argument_substitution), 1, 2},
      {frame(  v2fib<4>(), f, f, instantiation_kind::template_instantiation), 2, 2},
      {frame(   v2fib<2>(), f, f, instantiation_kind::template_instantiation), 3, 2},
      {frame(    v2fib<0>(), f, f, instantiation_kind::memoization), 4, 0},
      {frame(    v2fib<1>(), f, f, instantiation_kind::memoization), 4, 0},
      {frame(   v2fib<3>(), f, f, instantiation_kind::template_instantiation), 3, 2},
      {frame(    v2fib<1>(), f, f, instantiation_kind::memoization), 4, 0},
      {frame(    v2fib<2>(), f, f, instantiation_kind::memoization), 4, 0},
      {frame(  type("enable_if<false, char>"), f, f, instantiation_kind::template_instantiation), 2, 0},
      {frame( type("foo"), f, f, instantiation_kind::explicit_template_argument_substitution), 1, 5},
      {frame(  v1fib<4>(), f, f, instantiation_kind::template_instantiation), 2, 2},
      {frame(   v1fib<2>(), f, f, instantiation_kind::template_instantiation), 3, 2},
      {frame(    v1fib<0>(), f, f, instantiation_kind::memoization), 4, 0},
      {frame(    v1fib<1>(), f, f, instantiation_kind::memoization), 4, 0},
      {frame(   v1fib<3>(), f, f, instantiation_kind::template_instantiation), 3, 2},
      {frame(    v1fib<1>(), f, f, instantiation_kind::memoization), 4, 0},
      {frame(    v1fib<2>(), f, f, instantiation_kind::memoization), 4, 0},
      {frame(  type("enable_if<true, type-parameter-0-0>"), f, f, instantiation_kind::deduced_template_argument_substitution), 2, 0},
      {frame(  type("enable_if<true, void>"), f, f, instantiation_kind::template_instantiation), 2, 0},
      {frame(  v1fib<4>(), f, f, instantiation_kind::memoization), 2, 0},
      {frame(  type("enable_if<true, void>"), f, f, instantiation_kind::memoization), 2, 0},
      {frame( type("foo"), f, f, instantiation_kind::deduced_template_argument_substitution), 1, 0},
      {frame( type("foo<4>"), f, f, instantiation_kind::template_instantiation), 1, 0},
      {frame( type("void"), f, f, instantiation_kind::non_template_type), 1, 0}
    },
    d.call_graphs().front()
  );
}
#endif
