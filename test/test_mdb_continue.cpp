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

#include "test_fibonacci.hpp"

#include <just/test.hpp>

using namespace metashell;

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_without_evaluation) {
  mdb_test_shell sh;

  sh.line_available("continue");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Metaprogram not evaluated yet\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_fibonacci_no_breakpoint) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("continue");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Metaprogram finished\n"
      "int_<55>\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_fibonacci_1_breakpoint) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<0>");

  sh.clear_output();
  sh.line_available("continue");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Breakpoint reached\n"
      "fib<0> (Memoization)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_2_fibonacci_1_breakpoint) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<5>");

  sh.clear_output();
  sh.line_available("continue 2");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Breakpoint reached\n"
      "fib<5> (Memoization)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_twice_fibonacci_1_breakpoint) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<5>");

  sh.clear_output();
  sh.line_available("continue");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Breakpoint reached\n"
      "fib<5> (TemplateInstantiation)\n");

  sh.clear_output();
  sh.line_available("continue");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Breakpoint reached\n"
      "fib<5> (Memoization)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_fibonacci_2_breakpoints) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<5>");
  sh.line_available("rbreak fib<6>");

  sh.clear_output();
  sh.line_available("continue");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Breakpoint reached\n"
      "fib<6> (TemplateInstantiation)\n");

  sh.clear_output();
  sh.line_available("continue");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Breakpoint reached\n"
      "fib<5> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_2_fibonacci_2_breakpoints) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<5>");
  sh.line_available("rbreak fib<6>");

  sh.clear_output();
  sh.line_available("continue 2");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Breakpoint reached\n"
      "fib<5> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_10_fibonacci_2_breakpoints) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<5>");
  sh.line_available("rbreak fib<6>");

  sh.clear_output();
  sh.line_available("continue 10");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Metaprogram finished\n"
      "int_<55>\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_0_fibonacci_1_breakpoint) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<6>");

  sh.clear_output();
  sh.line_available("continue 0");

  JUST_ASSERT_EQUAL(sh.get_output(), "");
}
#endif
