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

#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell;

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_without_evaluated_metaprogram) {
  in_memory_displayer d;
  mdb_test_shell sh(d);

  sh.line_available("rbreak int");

  JUST_ASSERT_EQUAL_CONTAINER(d.errors(), {"Metaprogram not evaluated yet"});

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 0u);
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_with_no_arguments) {
  in_memory_displayer d;
  mdb_test_shell sh(d);

  sh.line_available("evaluate int");

  d.clear();
  sh.line_available("rbreak");

  JUST_ASSERT_EQUAL_CONTAINER(d.errors(), {"Argument expected"});

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 0u);
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_with_no_arguments_with_trailing_whitespace) {
  in_memory_displayer d;
  mdb_test_shell sh(d);

  sh.line_available("evaluate int");

  d.clear();
  sh.line_available("rbreak ");

  JUST_ASSERT_EQUAL_CONTAINER(d.errors(), {"Argument expected"});

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 0u);
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_with_invalid_regex) {
  in_memory_displayer d;
  mdb_test_shell sh(d);

  sh.line_available("evaluate int");

  d.clear();
  sh.line_available("rbreak [");

  JUST_ASSERT_EQUAL_CONTAINER(d.errors(), {"\"[\" is not a valid regex"});

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 0u);
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_with_valid_regex_no_match) {
  in_memory_displayer d;
  mdb_test_shell sh(d);

  sh.line_available("evaluate int");

  d.clear();
  sh.line_available("rbreak xyz");

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Breakpoint \"xyz\" will never stop the execution"},
    d.raw_texts()
  );

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 0u);
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_with_valid_regex_with_one_match) {
  in_memory_displayer d;
  mdb_test_shell sh(d);

  sh.line_available("evaluate int");

  d.clear();
  sh.line_available("rbreak int");

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Breakpoint \"int\" will stop the execution on 1 location"},
    d.raw_texts()
  );

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 1u);
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_with_valid_regex_with_two_matches) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>");

  d.clear();
  sh.line_available("rbreak fib<3>");

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Breakpoint \"fib<3>\" will stop the execution on 2 locations"},
    d.raw_texts()
  );

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 1u);
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_does_not_count_stops_in_unreachable_subgraphs) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp + "int __x = fib<10>::value;");

  sh.line_available("evaluate int_<fib<2>::value>");

  d.clear();
  sh.line_available("rbreak fib");

  // When precompiled headers are used in the outer metashell,
  // then there are 4 break locations
  JUST_ASSERT_EQUAL_CONTAINER(
    {"Breakpoint \"fib\" will stop the execution on 3 locations"},
    d.raw_texts()
  );

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 1u);
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_with_valid_regex_in_full_mode) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate -full int_<fib<5>::value>");

  d.clear();
  sh.line_available("rbreak fib<3>");

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Breakpoint \"fib<3>\" will stop the execution on 2 locations"},
    d.raw_texts()
  );

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 1u);
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_with_valid_regex_in_full_mode_match_only_root) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate -full int_<fib<5>::value>");

  d.clear();
  sh.line_available("rbreak int_<fib<5>::value>");

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Breakpoint \"int_<fib<5>::value>\" will never stop the execution"},
    d.raw_texts()
  );

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 0u);
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_rbreak_with_valid_regex_in_full_mode_match_also_root) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate -full int_<fib<5>::value>");

  d.clear();
  sh.line_available("rbreak (int_<fib<5>::value>)|(fib<3>)");

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      "Breakpoint \"(int_<fib<5>::value>)|(fib<3>)\" "
      "will stop the execution on 2 locations"
    },
    d.raw_texts()
  );

  JUST_ASSERT_EQUAL(sh.get_breakpoints().size(), 1u);
}
#endif
