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
JUST_TEST_CASE(test_mdb_rbreak_with_no_arguments) {
  mdb_test_shell sh;

  sh.line_available("evaluate int");

  sh.clear_output();
  sh.line_available("rbreak");

  JUST_ASSERT_EQUAL(sh.get_output(), "Argument expected\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_with_no_arguments_with_trailing_whitespace) {
  mdb_test_shell sh;

  sh.line_available("evaluate int");

  sh.clear_output();
  sh.line_available("rbreak ");

  JUST_ASSERT_EQUAL(sh.get_output(), "Argument expected\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_with_invalid_regex) {
  mdb_test_shell sh;

  sh.line_available("evaluate int");

  sh.clear_output();
  sh.line_available("rbreak [");

  JUST_ASSERT_EQUAL(sh.get_output(), "\"[\" is not a valid regex\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_with_valid_regex_no_match) {
  mdb_test_shell sh;

  sh.line_available("evaluate int");

  sh.clear_output();
  sh.line_available("rbreak xyz");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Breakpoint \"xyz\" will never stop the execution\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_with_valid_regex_with_one_match) {
  mdb_test_shell sh;

  sh.line_available("evaluate int");

  sh.clear_output();
  sh.line_available("rbreak int");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Breakpoint \"int\" will stop the execution on 1 location\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_with_valid_regex_with_two_matches) {
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>");

  sh.clear_output();
  sh.line_available("rbreak fib<3>");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Breakpoint \"fib<3>\" will stop the execution on 2 locations\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_does_not_count_stops_in_unreachable_subgraphs) {
  mdb_test_shell sh(fibonacci_mp + "int __x = fib<10>::value;");

  sh.line_available("evaluate int_<fib<2>::value>");

  sh.clear_output();
  sh.line_available("rbreak fib");

  // When precompiled headers are used in the outer metashell,
  // then there are 4 break locations
  JUST_ASSERT_EQUAL(sh.get_output(),
      "Breakpoint \"fib\" will stop the execution on 3 locations\n");
}
#endif
