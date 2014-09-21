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
JUST_TEST_CASE(test_mdb_backtrace_without_evaluation) {
  mdb_test_shell sh;

  sh.line_available("backtrace");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Metaprogram not evaluated yet\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_backtrace_unstepped_fibonacci) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("backtrace");

  JUST_ASSERT_EQUAL(sh.get_output(), "");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_backtrace_1_stepped_fibonacci) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("step");

  sh.clear_output();
  sh.line_available("backtrace");

  JUST_ASSERT_EQUAL(sh.get_output(), "#0 fib<10> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_backtrace_2_stepped_fibonacci) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("step 2");

  sh.clear_output();
  sh.line_available("backtrace");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "#0 fib<8> (TemplateInstantiation)\n"
      "#1 fib<10> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_backtrace_3_stepped_fibonacci) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("step 3");

  sh.clear_output();
  sh.line_available("backtrace");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "#0 fib<6> (TemplateInstantiation)\n"
      "#1 fib<8> (TemplateInstantiation)\n"
      "#2 fib<10> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_backtrace_garbage_argument) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("backtrace asd");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "This command doesn't accept arguments\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_backtrace_bt_alias) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("step");

  sh.clear_output();
  sh.line_available("backtrace");
  JUST_ASSERT_EQUAL(sh.get_output(), "#0 fib<10> (TemplateInstantiation)\n");

  sh.clear_output();
  sh.line_available("bt");
  JUST_ASSERT_EQUAL(sh.get_output(), "#0 fib<10> (TemplateInstantiation)\n");
}
#endif
