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

#include "metadebugger_test_shell.hpp"

#include "test_fibonacci.hpp"

#include <just/test.hpp>

using namespace metashell;

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_without_evaluation) {
  metadebugger_test_shell sh = metadebugger_test_shell::create_default();

  sh.line_available("step");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Metaprogram finished or not evaluated yet\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_fibonacci) {
  metadebugger_test_shell sh =
    metadebugger_test_shell::create_with_environment(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("step");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<10> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_2_fibonacci) {
  metadebugger_test_shell sh =
    metadebugger_test_shell::create_with_environment(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("step 2");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<8> (TemplateInstantiation)\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_fibonacci_twice) {
  metadebugger_test_shell sh =
    metadebugger_test_shell::create_with_environment(fibonacci_mp);

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
JUST_TEST_CASE(test_mdb_step_0_fibonacci_at_start) {
  metadebugger_test_shell sh =
    metadebugger_test_shell::create_with_environment(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  sh.clear_output();
  sh.line_available("step 0");

  JUST_ASSERT_EQUAL(sh.get_output(), "");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_0_fibonacci_after_step) {
  metadebugger_test_shell sh =
    metadebugger_test_shell::create_with_environment(fibonacci_mp);

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
