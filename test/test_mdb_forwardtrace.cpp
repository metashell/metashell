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

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_without_evaluation) {
  in_memory_displayer d;
  mdb_test_shell sh(d);

  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL_CONTAINER(d.errors(), {"Metaprogram not evaluated yet"});
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_garbage_argument) {
  in_memory_displayer d;
  mdb_test_shell sh(d);

  sh.line_available("evaluate int");
  sh.line_available("forwardtrace asd");

  JUST_ASSERT_EQUAL_CONTAINER(d.errors(), {"Argument parsing failed"});
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_int) {
  in_memory_displayer d;
  mdb_test_shell sh(d);

  sh.line_available("evaluate int");
  sh.line_available("forwardtrace");

  const type int_("int");

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(int_), 0, 1},
      {frame(int_, instantiation_kind::non_template_type), 1, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_int_in_full_mode) {
  in_memory_displayer d;
  mdb_test_shell sh(d);

  sh.line_available("evaluate -full int");
  sh.line_available("forwardtrace");

  const type int_("int");

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(int_), 0, 1},
      {frame(int_), 1, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_when_metaprogram_finished) {
  in_memory_displayer d;
  mdb_test_shell sh(d);

  sh.line_available("evaluate int");
  sh.line_available("continue");

  d.clear();
  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_when_metaprogram_finished_in_full_mode) {
  in_memory_displayer d;
  mdb_test_shell sh(d);

  sh.line_available("evaluate -full int");
  sh.line_available("continue");

  d.clear();
  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_int_with_ft) {
  in_memory_displayer d;
  mdb_test_shell sh(d);

  sh.line_available("evaluate int");
  sh.line_available("ft");

  const type int_("int");

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(int_), 0, 1},
      {frame(int_, instantiation_kind::non_template_type), 1, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_from_root) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>");
  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(type("int_<fib<5>::value>")), 0, 3},
      {frame( fib<5>(), instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<3>(), instantiation_kind::template_instantiation), 2, 2},
      {frame(   fib<1>(), instantiation_kind::memoization), 3, 0},
      {frame(   fib<2>(), instantiation_kind::template_instantiation), 3, 2},
      {frame(    fib<0>(), instantiation_kind::memoization), 4, 0},
      {frame(    fib<1>(), instantiation_kind::memoization), 4, 0},
      {frame(  fib<4>(), instantiation_kind::template_instantiation), 2, 2},
      {frame(   fib<2>(), instantiation_kind::memoization), 3, 0},
      {frame(   fib<3>(), instantiation_kind::memoization), 3, 0},
      {frame( fib<5>(), instantiation_kind::memoization), 1, 0},
      {frame( type("int_<5>"), instantiation_kind::template_instantiation), 1,0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_from_root_in_full_mode) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate -full int_<fib<5>::value>");
  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
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
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_from_memoization) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>");
  sh.line_available("rbreak fib<5>");
  sh.line_available("continue 2");
  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame( fib<5>(), instantiation_kind::memoization), 0, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_1) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>");
  sh.line_available("step");
  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(fib<5>(), instantiation_kind::template_instantiation), 0, 2},
      {frame( fib<3>(), instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<1>(), instantiation_kind::memoization), 2, 0},
      {frame(  fib<2>(), instantiation_kind::template_instantiation), 2, 2},
      {frame(   fib<0>(), instantiation_kind::memoization), 3, 0},
      {frame(   fib<1>(), instantiation_kind::memoization), 3, 0},
      {frame( fib<4>(), instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<2>(), instantiation_kind::memoization), 2, 0},
      {frame(  fib<3>(), instantiation_kind::memoization), 2, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_1_in_full_mode) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate -full int_<fib<5>::value>");
  sh.line_available("step");
  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
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
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_1_with_limit_0) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>");
  sh.line_available("step");
  sh.line_available("forwardtrace 0");

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame( fib<5>(), instantiation_kind::template_instantiation), 0, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_1_with_limit_1) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>");
  sh.line_available("step");
  sh.line_available("forwardtrace 1");

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(fib<5>(), instantiation_kind::template_instantiation), 0, 2},
      {frame( fib<3>(), instantiation_kind::template_instantiation), 1, 0},
      {frame( fib<4>(), instantiation_kind::template_instantiation), 1, 0}
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
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate -full int_<fib<5>::value>");
  sh.line_available("step");
  sh.line_available("forwardtrace 1");

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(fib<5>()), 0, 2},
      {frame( fib<3>()), 1, 0},
      {frame( fib<4>()), 1, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_2_with_limit_2) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>");
  sh.line_available("step");
  sh.line_available("forwardtrace 2");

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(fib<5>(), instantiation_kind::template_instantiation), 0, 2},
      {frame( fib<3>(), instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<1>(), instantiation_kind::memoization), 2, 0},
      {frame(  fib<2>(), instantiation_kind::template_instantiation), 2, 0},
      {frame( fib<4>(), instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<2>(), instantiation_kind::memoization), 2, 0},
      {frame(  fib<3>(), instantiation_kind::memoization), 2, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_2_with_limit_100) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>");
  sh.line_available("step");
  sh.line_available("forwardtrace 100");

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(fib<5>(), instantiation_kind::template_instantiation), 0, 2},
      {frame( fib<3>(), instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<1>(), instantiation_kind::memoization), 2, 0},
      {frame(  fib<2>(), instantiation_kind::template_instantiation), 2, 2},
      {frame(   fib<0>(), instantiation_kind::memoization), 3, 0},
      {frame(   fib<1>(), instantiation_kind::memoization), 3, 0},
      {frame( fib<4>(), instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<2>(), instantiation_kind::memoization), 2, 0},
      {frame(  fib<3>(), instantiation_kind::memoization), 2, 0}
    },
    d.call_graphs().front()
  );
}
#endif

