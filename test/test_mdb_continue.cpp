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
JUST_TEST_CASE(test_mdb_continue_without_evaluation) {
  in_memory_displayer d;
  mdb_test_shell sh(d);

  sh.line_available("continue");

  JUST_ASSERT_EQUAL_CONTAINER(d.errors(), {"Metaprogram not evaluated yet"});
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_garbage_argument) {
  in_memory_displayer d;
  mdb_test_shell sh(d);

  sh.line_available("evaluate int");

  d.clear();
  sh.line_available("continue asd");

  JUST_ASSERT_EQUAL_CONTAINER(d.errors(), {"Argument parsing failed"});
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_fibonacci_no_breakpoint) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  d.clear();
  sh.line_available("continue");

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int_<55>")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_fibonacci_reevaulation_removes_breakpoints) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<0>");

  sh.line_available("evaluate int_<fib<10>::value>");

  d.clear();
  sh.line_available("continue");

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int_<55>")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_fibonacci_1_breakpoint) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<0>");

  d.clear();
  sh.line_available("continue");

  JUST_ASSERT_EQUAL_CONTAINER({"Breakpoint \"fib<0>\" reached"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<0>"), instantiation_kind::memoization)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_2_fibonacci_1_breakpoint) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<5>");

  d.clear();
  sh.line_available("continue 2");

  JUST_ASSERT_EQUAL_CONTAINER({"Breakpoint \"fib<5>\" reached"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<5>"), instantiation_kind::memoization)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_twice_fibonacci_1_breakpoint) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<5>");

  d.clear();
  sh.line_available("continue");

  JUST_ASSERT_EQUAL_CONTAINER({"Breakpoint \"fib<5>\" reached"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<5>"), instantiation_kind::template_instantiation)},
    d.frames()
  );

  d.clear();
  sh.line_available("continue");

  JUST_ASSERT_EQUAL_CONTAINER({"Breakpoint \"fib<5>\" reached"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<5>"), instantiation_kind::memoization)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_fibonacci_2_breakpoints) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<5>");
  sh.line_available("rbreak fib<6>");

  d.clear();
  sh.line_available("continue");

  JUST_ASSERT_EQUAL_CONTAINER({"Breakpoint \"fib<6>\" reached"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<6>"), instantiation_kind::template_instantiation)},
    d.frames()
  );

  d.clear();
  sh.line_available("continue");

  JUST_ASSERT_EQUAL_CONTAINER({"Breakpoint \"fib<5>\" reached"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<5>"), instantiation_kind::template_instantiation)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_2_fibonacci_2_breakpoints) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<5>");
  sh.line_available("rbreak fib<6>");

  d.clear();
  sh.line_available("continue 2");

  JUST_ASSERT_EQUAL_CONTAINER({"Breakpoint \"fib<5>\" reached"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<5>"), instantiation_kind::template_instantiation)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_10_fibonacci_2_breakpoints) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<5>");
  sh.line_available("rbreak fib<6>");

  d.clear();
  sh.line_available("continue 10");

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int_<55>")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_0_fibonacci_1_breakpoint) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<6>");

  d.clear();
  sh.line_available("continue 0");

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_minus_1_at_start) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  d.clear();
  sh.line_available("continue -1");

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Metaprogram reached the beginning"},
    d.raw_texts()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_minus_2_at_start) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");

  d.clear();
  sh.line_available("continue -2");

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Metaprogram reached the beginning"},
    d.raw_texts()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_minus_1_with_preceding_breakpoint) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<6>");
  sh.line_available("rbreak fib<5>");

  d.clear();
  sh.line_available("continue 2");

  JUST_ASSERT_EQUAL_CONTAINER({"Breakpoint \"fib<5>\" reached"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<5>"), instantiation_kind::template_instantiation)},
    d.frames()
  );

  d.clear();
  sh.line_available("continue -1");

  JUST_ASSERT_EQUAL_CONTAINER({"Breakpoint \"fib<6>\" reached"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<6>"), instantiation_kind::template_instantiation)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_minus_1_without_preceding_breakpoint) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<5>");

  d.clear();
  sh.line_available("continue 1");

  JUST_ASSERT_EQUAL_CONTAINER({"Breakpoint \"fib<5>\" reached"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<5>"), instantiation_kind::template_instantiation)},
    d.frames()
  );

  d.clear();
  sh.line_available("continue -1");

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Metaprogram reached the beginning"},
    d.raw_texts()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_to_end_and_back_to_start) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>");
  sh.line_available("rbreak fib<5>");

  d.clear();
  sh.line_available("continue 3");

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int_<55>")}, d.types());

  d.clear();
  sh.line_available("continue -3");

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Metaprogram reached the beginning"},
    d.raw_texts()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_continue_to_end_and_back_to_start_in_full_mode) {
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate -full int_<fib<10>::value>");

  d.clear();
  sh.line_available("rbreak fib<5>");

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Breakpoint \"fib<5>\" will stop the execution on 8 locations"},
    d.raw_texts()
  );

  d.clear();
  sh.line_available("continue 9");

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int_<55>")}, d.types());

  d.clear();
  sh.line_available("continue -9");

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Metaprogram reached the beginning"},
    d.raw_texts()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(
    test_mdb_continue_to_one_before_end_and_back_to_start_in_full_mode)
{
  in_memory_displayer d;
  mdb_test_shell sh(d, fibonacci_mp);

  sh.line_available("evaluate -full int_<fib<10>::value>");

  d.clear();
  sh.line_available("rbreak fib<5>");

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Breakpoint \"fib<5>\" will stop the execution on 8 locations"},
    d.raw_texts()
  );

  d.clear();
  sh.line_available("continue 8");

  JUST_ASSERT_EQUAL_CONTAINER({"Breakpoint \"fib<5>\" reached"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({frame(type("fib<5>"))}, d.frames());

  d.clear();
  sh.line_available("continue -8");

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Metaprogram reached the beginning"},
    d.raw_texts()
  );
}
#endif
