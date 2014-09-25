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
JUST_TEST_CASE(test_mdb_step_without_evaluation) {
  mdb_test_shell sh;

  sh.line_available("step");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Metaprogram not evaluated yet\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_fibonacci) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("step");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<10> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_2_fibonacci) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("step 2");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<8> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_fibonacci_twice) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("step");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<10> (TemplateInstantiation)\n");

  sh.clear_output();
  sh.line_available("step");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<8> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_fibonacci_twice_with_empty_second_line) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("step");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<10> (TemplateInstantiation)\n");

  sh.clear_output();
  sh.line_available("");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<8> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_fibonacci_twice_with_space_second_line) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("step");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<10> (TemplateInstantiation)\n");

  sh.clear_output();
  sh.line_available(" ");

  JUST_ASSERT_EQUAL(sh.get_output(), "");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_0_fibonacci_at_start) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("step 0");

  JUST_ASSERT_EQUAL(sh.get_output(), "");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_0_fibonacci_after_step) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("step");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<10> (TemplateInstantiation)\n");

  sh.clear_output();
  sh.line_available("step 0");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<10> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_over_the_whole_metaprogram_one_step) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("step 31");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Metaprogram finished\n"
      "int_<55>\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_over_the_whole_metaprogram_multiple_steps) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.clear_output();

  std::string expected_output =
    "fib<10> (TemplateInstantiation)\n"
    "fib<8> (TemplateInstantiation)\n"
    "fib<6> (TemplateInstantiation)\n"
    "fib<4> (TemplateInstantiation)\n"
    "fib<2> (TemplateInstantiation)\n"
    "fib<0> (Memoization)\n"
    "fib<1> (Memoization)\n"
    "fib<3> (TemplateInstantiation)\n"
    "fib<1> (Memoization)\n"
    "fib<2> (Memoization)\n"
    "fib<5> (TemplateInstantiation)\n"
    "fib<3> (Memoization)\n"
    "fib<4> (Memoization)\n"
    "fib<7> (TemplateInstantiation)\n"
    "fib<5> (Memoization)\n"
    "fib<6> (Memoization)\n"
    "fib<9> (TemplateInstantiation)\n"
    "fib<7> (Memoization)\n"
    "fib<8> (Memoization)\n"
    "fib<10> (Memoization)\n";

  for (unsigned i = 0; i < 20; ++i) {
    sh.line_available("step");
  }

  JUST_ASSERT_EQUAL(sh.get_output(), expected_output);
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_minus_1_at_start) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("step -1");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Metaprogram reached the beginning\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_minus_1_after_step) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("step 1");

  sh.clear_output();
  sh.line_available("step -1");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Metaprogram reached the beginning\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_0_at_start) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("step 0");

  JUST_ASSERT_EQUAL(sh.get_output(), "");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_0_at_end) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("continue");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Metaprogram finished\n"
      "int_<55>\n");

  sh.clear_output();
  sh.line_available("step 0");

  JUST_ASSERT_EQUAL(sh.get_output(), "");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_minus_1_after_step_2) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("step 2");

  sh.clear_output();
  sh.line_available("step -1");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<10> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_garbage_argument) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("step asd");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Argument parsing failed\n");
}
#endif
