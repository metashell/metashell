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

#include <algorithm>

#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell;

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_int) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("evaluate int", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());
  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL(
    sh.get_metaprogram().get_evaluation_result(),
    data::type_or_error::make_type(data::type("int"))
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_fib_10) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());
  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL(
    sh.get_metaprogram().get_evaluation_result(),
    data::type_or_error::make_type(data::type("int_<55>"))
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_no_arguments_no_evaluation) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("evaluate", d);

  JUST_ASSERT(!sh.has_metaprogram());
  JUST_ASSERT_EQUAL_CONTAINER(d.errors(), {"Nothing has been evaluated yet."});
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(
    test_mdb_evaluate_no_arguments_with_trailing_spaces_no_evaluation)
{
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("evaluate  ", d);

  JUST_ASSERT(!sh.has_metaprogram());
  JUST_ASSERT_EQUAL_CONTAINER(d.errors(), {"Nothing has been evaluated yet."});
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_failure_will_reset_metaprogram_state) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("evaluate int", d);

  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());

  d.clear();
  sh.line_available("evaluate in", d);

  JUST_ASSERT(!sh.has_metaprogram());
  JUST_ASSERT(!d.errors().empty());

  d.clear();
  sh.line_available("evaluate int", d);

  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_instantiation_failure_will_start_metaprogram)
{
  in_memory_displayer d;
  mdb_test_shell sh(missing_value_fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);

  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_missing_argument_will_run_last_metaprogram) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("evaluate int", d);

  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());

  d.clear();
  sh.line_available("evaluate", d);

  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(
    test_mdb_evaluate_missing_argument_will_reset_metaprogram_state)
{
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());
  auto first_state = sh.get_metaprogram().get_state();

  sh.line_available("step 5", d);

  d.clear();
  sh.line_available("evaluate", d);

  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());

  auto second_state = sh.get_metaprogram().get_state();

  // Comparing only the discovered vector should be enough
  JUST_ASSERT(first_state.discovered.size() == second_state.discovered.size());
  JUST_ASSERT(
      std::equal(
        first_state.discovered.begin(),
        first_state.discovered.end(),
        second_state.discovered.begin()));
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_filters_similar_edges) {
  using data::instantiation_kind;
  using data::type;
  using data::frame;
  using data::file_location;

  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_with_enum_mp);

  sh.line_available("evaluate int_<fib<2>::value>", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());

  d.clear();
  sh.line_available("forwardtrace", d);

  // Clang actually emits more than 50 fib<1>::ENUM and fib<0>::ENUM events
  // These are filtered out
  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(type("int_<fib<2>::value>")), 0, 4},
      {frame( fib<2>(), file_location(), instantiation_kind::template_instantiation), 1, 4},
      {frame(  fib<0>(), file_location(), instantiation_kind::memoization), 2, 0},
      {frame(  fib<1>(), file_location(), instantiation_kind::memoization), 2, 0},
      {frame(  type("fib<1>::ENUM"), file_location(), instantiation_kind::memoization), 2, 0},
      {frame(  type("fib<0>::ENUM"), file_location(), instantiation_kind::memoization), 2, 0},
      {frame( fib<2>(), file_location(), instantiation_kind::memoization), 1, 0},
      {frame( type("fib<2>::ENUM"), file_location(), instantiation_kind::memoization), 1, 0},
      {frame( type("int_<1>"), file_location(), instantiation_kind::template_instantiation), 1,0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_clears_breakpoints) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("evaluate int", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());

  d.clear();
  sh.line_available("rbreak int", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Breakpoint \"int\" will stop the execution on 1 location"},
    d.raw_texts()
  );

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 1u);

  d.clear();
  sh.line_available("evaluate float", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 0u);
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_reevaluate_clears_breakpoints) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("evaluate int", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());

  d.clear();
  sh.line_available("rbreak int", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Breakpoint \"int\" will stop the execution on 1 location"},
    d.raw_texts()
  );

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 1u);

  d.clear();
  sh.line_available("evaluate", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 0u);
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_failure_clears_breakpoints) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("evaluate int", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());

  d.clear();
  sh.line_available("rbreak int", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Breakpoint \"int\" will stop the execution on 1 location"},
    d.raw_texts()
  );

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 1u);

  d.clear();
  sh.line_available("evaluate asd", d);

  JUST_ASSERT(!d.errors().empty());

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 0u);
}
#endif
