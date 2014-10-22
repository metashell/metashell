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

#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell;

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_without_evaluation) {
  mdb_test_shell sh;

  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Metaprogram not evaluated yet\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_garbage_argument) {
  mdb_test_shell sh;

  sh.line_available("evaluate int");

  sh.clear_output();
  sh.line_available("forwardtrace asd");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Argument parsing failed\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_int) {
  mdb_test_shell sh;

  sh.line_available("evaluate int");

  sh.clear_output();
  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "int\n"
      "` int (NonTemplateType)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_when_metaprogram_finished) {
  mdb_test_shell sh;

  sh.line_available("evaluate int");
  sh.line_available("continue");

  sh.clear_output();
  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Metaprogram finished\n"
      "int\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_when_metaprogram_finished_in_full_mode) {
  mdb_test_shell sh;

  sh.line_available("evaluate -full int");
  sh.line_available("continue");

  sh.clear_output();
  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Metaprogram finished\n"
      "int\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_int_with_ft) {
  mdb_test_shell sh;

  sh.line_available("evaluate int");

  sh.clear_output();
  sh.line_available("ft");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "int\n"
      "` int (NonTemplateType)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_from_root) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>");

  sh.clear_output();
  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "int_<fib<5>::value>\n"
      "+ fib<5> (TemplateInstantiation)\n"
      "| + fib<3> (TemplateInstantiation)\n"
      "| | + fib<1> (Memoization)\n"
      "| | ` fib<2> (TemplateInstantiation)\n"
      "| |   + fib<0> (Memoization)\n"
      "| |   ` fib<1> (Memoization)\n"
      "| ` fib<4> (TemplateInstantiation)\n"
      "|   + fib<2> (Memoization)\n"
      "|   ` fib<3> (Memoization)\n"
      "+ fib<5> (Memoization)\n"
      "` int_<5> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_from_root_in_full_mode) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate -full int_<fib<5>::value>");

  sh.clear_output();
  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "int_<fib<5>::value>\n"
      "+ fib<5>\n"
      "| + fib<3>\n"
      "| | + fib<1>\n"
      "| | ` fib<2>\n"
      "| |   + fib<0>\n"
      "| |   ` fib<1>\n"
      "| ` fib<4>\n"
      "|   + fib<2>\n"
      "|   | + fib<0>\n"
      "|   | ` fib<1>\n"
      "|   ` fib<3>\n"
      "|     + fib<1>\n"
      "|     ` fib<2>\n"
      "|       + fib<0>\n"
      "|       ` fib<1>\n"
      "+ fib<5>\n"
      "| + fib<3>\n"
      "| | + fib<1>\n"
      "| | ` fib<2>\n"
      "| |   + fib<0>\n"
      "| |   ` fib<1>\n"
      "| ` fib<4>\n"
      "|   + fib<2>\n"
      "|   | + fib<0>\n"
      "|   | ` fib<1>\n"
      "|   ` fib<3>\n"
      "|     + fib<1>\n"
      "|     ` fib<2>\n"
      "|       + fib<0>\n"
      "|       ` fib<1>\n"
      "` int_<5>\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_from_memoization) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>");
  sh.line_available("rbreak fib<5>");
  sh.line_available("continue 2");

  sh.clear_output();
  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL(sh.get_output(), "fib<5> (Memoization)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_1) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>");
  sh.line_available("step");

  sh.clear_output();
  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<5> (TemplateInstantiation)\n"
      "+ fib<3> (TemplateInstantiation)\n"
      "| + fib<1> (Memoization)\n"
      "| ` fib<2> (TemplateInstantiation)\n"
      "|   + fib<0> (Memoization)\n"
      "|   ` fib<1> (Memoization)\n"
      "` fib<4> (TemplateInstantiation)\n"
      "  + fib<2> (Memoization)\n"
      "  ` fib<3> (Memoization)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_1_in_full_mode) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate -full int_<fib<5>::value>");
  sh.line_available("step");

  sh.clear_output();
  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<5>\n"
      "+ fib<3>\n"
      "| + fib<1>\n"
      "| ` fib<2>\n"
      "|   + fib<0>\n"
      "|   ` fib<1>\n"
      "` fib<4>\n"
      "  + fib<2>\n"
      "  | + fib<0>\n"
      "  | ` fib<1>\n"
      "  ` fib<3>\n"
      "    + fib<1>\n"
      "    ` fib<2>\n"
      "      + fib<0>\n"
      "      ` fib<1>\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_1_with_limit_0) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>");
  sh.line_available("step");

  sh.clear_output();
  sh.line_available("forwardtrace 0");

  JUST_ASSERT_EQUAL(sh.get_output(), "fib<5> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_1_with_limit_1) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>");
  sh.line_available("step");

  sh.clear_output();
  sh.line_available("forwardtrace 1");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<5> (TemplateInstantiation)\n"
      "+ fib<3> (TemplateInstantiation)\n"
      "` fib<4> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(
    test_mdb_forwardtrace_ft_from_step_1_with_limit_1_in_full_mode)
{
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate -full int_<fib<5>::value>");
  sh.line_available("step");

  sh.clear_output();
  sh.line_available("forwardtrace 1");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<5>\n"
      "+ fib<3>\n"
      "` fib<4>\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_2_with_limit_2) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>");
  sh.line_available("step");

  sh.clear_output();
  sh.line_available("forwardtrace 2");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<5> (TemplateInstantiation)\n"
      "+ fib<3> (TemplateInstantiation)\n"
      "| + fib<1> (Memoization)\n"
      "| ` fib<2> (TemplateInstantiation)\n"
      "` fib<4> (TemplateInstantiation)\n"
      "  + fib<2> (Memoization)\n"
      "  ` fib<3> (Memoization)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_ft_from_step_2_with_limit_100) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>");
  sh.line_available("step");

  sh.clear_output();
  sh.line_available("forwardtrace 100");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<5> (TemplateInstantiation)\n"
      "+ fib<3> (TemplateInstantiation)\n"
      "| + fib<1> (Memoization)\n"
      "| ` fib<2> (TemplateInstantiation)\n"
      "|   + fib<0> (Memoization)\n"
      "|   ` fib<1> (Memoization)\n"
      "` fib<4> (TemplateInstantiation)\n"
      "  + fib<2> (Memoization)\n"
      "  ` fib<3> (Memoization)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_forwardtrace_from_root_on_narrow_terminal) {
  mdb_test_shell sh(fibonacci_mp);
  sh.set_terminal_width(25);

  sh.line_available("evaluate int_<fib<5>::value>");

  sh.clear_output();
  sh.line_available("forwardtrace");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "int_<fib<5>::value>\n"
      "+ fib<5> (TemplateInstant\n"
      "| iation)\n"
      "| + fib<3> (TemplateInsta\n"
      "| | ntiation)\n"
      "| | + fib<1> (Memoization\n"
      "| | | )\n"
      "| | ` fib<2> (TemplateIns\n"
      "| |   tantiation)\n"
      "| |   + fib<0> (Memoizati\n"
      "| |   | on)\n"
      "| |   ` fib<1> (Memoizati\n"
      "| |     on)\n"
      "| ` fib<4> (TemplateInsta\n"
      "|   ntiation)\n"
      "|   + fib<2> (Memoization\n"
      "|   | )\n"
      "|   ` fib<3> (Memoization\n"
      "|     )\n"
      "+ fib<5> (Memoization)\n"
      "` int_<5> (TemplateInstan\n"
      "  tiation)\n");
}
#endif
