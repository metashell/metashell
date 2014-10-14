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

#include "mdb_test_shell.hpp"

#include <algorithm>
#include <boost/algorithm/string.hpp>

#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell;

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_int) {
  mdb_test_shell sh;

  sh.line_available("evaluate int");

  JUST_ASSERT_EQUAL(sh.get_output(), "Metaprogram started\n");
  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL(sh.get_metaprogram().get_evaluation_result(), "int");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_fib_10) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(sh.get_output(), "Metaprogram started\n");
  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL(sh.get_metaprogram().get_evaluation_result(), "int_<55>");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_no_arguments_no_evaluation) {
  mdb_test_shell sh;

  sh.line_available("evaluate");

  JUST_ASSERT(!sh.has_metaprogram());
  JUST_ASSERT_EQUAL(sh.get_output(), "Nothing has been evaluated yet.\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(
    test_mdb_evaluate_no_arguments_with_trailing_spaces_no_evaluation)
{
  mdb_test_shell sh;

  sh.line_available("evaluate  ");

  JUST_ASSERT(!sh.has_metaprogram());
  JUST_ASSERT_EQUAL(sh.get_output(), "Nothing has been evaluated yet.\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_failure_will_reset_metaprogram_state) {
  using boost::algorithm::icontains;

  mdb_test_shell sh;

  sh.line_available("evaluate int");

  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL(sh.get_output(), "Metaprogram started\n");

  sh.clear_output();
  sh.line_available("evaluate in");

  JUST_ASSERT(!sh.has_metaprogram());
  JUST_ASSERT(icontains(sh.get_output(), "error"));

  sh.clear_output();
  sh.line_available("evaluate int");

  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL(sh.get_output(), "Metaprogram started\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_missing_argument_will_run_last_metaprogram) {
  mdb_test_shell sh;

  sh.line_available("evaluate int");

  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL(sh.get_output(), "Metaprogram started\n");

  sh.clear_output();
  sh.line_available("evaluate");

  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL(sh.get_output(), "Metaprogram started\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(
    test_mdb_evaluate_missing_argument_will_reset_metaprogram_state)
{
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL(sh.get_output(), "Metaprogram started\n");
  auto first_state = sh.get_metaprogram().get_state();

  sh.line_available("step 5");

  sh.clear_output();
  sh.line_available("evaluate");

  JUST_ASSERT(sh.has_metaprogram());
  JUST_ASSERT_EQUAL(sh.get_output(), "Metaprogram started\n");

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
  mdb_test_shell sh(fibonacci_with_enum_mp);

  sh.line_available("evaluate int_<fib<2>::value>");

  JUST_ASSERT_EQUAL(sh.get_output(), "Metaprogram started\n");

  sh.clear_output();
  sh.line_available("forwardtrace");

  // Clang actually emits more than 50 fib<1>::ENUM and fib<0>::ENUM events
  // These are filtered out
  JUST_ASSERT_EQUAL(sh.get_output(),
      "int_<fib<2>::value>\n"
      "+ fib<2> (TemplateInstantiation)\n"
      "| + fib<0> (Memoization)\n"
      "| + fib<1> (Memoization)\n"
      "| + fib<1>::ENUM (Memoization)\n"
      "| ` fib<0>::ENUM (Memoization)\n"
      "+ fib<2> (Memoization)\n"
      "+ fib<2>::ENUM (Memoization)\n"
      "` int_<1> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_clears_breakpoints) {
  mdb_test_shell sh;

  sh.line_available("evaluate int");

  JUST_ASSERT_EQUAL(sh.get_output(), "Metaprogram started\n");

  sh.clear_output();
  sh.line_available("rbreak int");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Breakpoint \"int\" will stop the execution on 1 location\n");

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 1u);

  sh.clear_output();
  sh.line_available("evaluate float");

  JUST_ASSERT_EQUAL(sh.get_output(), "Metaprogram started\n");

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 0u);
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_reevaluate_clears_breakpoints) {
  mdb_test_shell sh;

  sh.line_available("evaluate int");

  JUST_ASSERT_EQUAL(sh.get_output(), "Metaprogram started\n");

  sh.clear_output();
  sh.line_available("rbreak int");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Breakpoint \"int\" will stop the execution on 1 location\n");

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 1u);

  sh.clear_output();
  sh.line_available("evaluate");

  JUST_ASSERT_EQUAL(sh.get_output(), "Metaprogram started\n");

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 0u);
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_evaluate_failure_clears_breakpoints) {
  mdb_test_shell sh;

  sh.line_available("evaluate int");

  JUST_ASSERT_EQUAL(sh.get_output(), "Metaprogram started\n");

  sh.clear_output();
  sh.line_available("rbreak int");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Breakpoint \"int\" will stop the execution on 1 location\n");

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 1u);

  sh.clear_output();
  sh.line_available("evaluate asd");

  JUST_ASSERT(boost::algorithm::icontains(sh.get_output(), "error"));

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 0u);
}
#endif
