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

#include <metashell/mdb_shell.hpp>

#include <just/test.hpp>

#include "mdb_test_shell.hpp"
#include "util.hpp"

using namespace metashell;

// If one of these TCs fail, then README modification might be needed

JUST_TEST_CASE(test_readme_continue_abbreviated_as_c) {
  mdb_command command;
  std::string args;

  std::tie(command, args) =
    get_command_from_map(mdb_shell::command_handler, "c");

  auto keys = command.get_keys();

  JUST_ASSERT(std::find(keys.begin(), keys.end(), "continue") != keys.end());
}

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_readme_getting_started) {
  mdb_test_shell sh(
  "template <int N>"
  "struct fib"
  "{"
  "  static constexpr int value = fib<N - 1>::value + fib<N - 2>::value;"
  "};"
  "template <>"
  "struct fib<0>"
  "{ "
  "  static constexpr int value = 1;"
  "};"
  "template <>"
  "struct fib<1>"
  "{"
  "  static constexpr int value = 1;"
  "};"
  "template<int N>"
  "struct int_ {};");

  sh.line_available("evaluate int_<fib<6>::value>");

  JUST_ASSERT_EQUAL(sh.get_output(), "Metaprogram started\n");

  sh.clear_output();
  sh.line_available("step 3");

  JUST_ASSERT_EQUAL(sh.get_output(), "fib<4> (TemplateInstantiation)\n");

  sh.clear_output();
  sh.line_available("step -1");

  JUST_ASSERT_EQUAL(sh.get_output(), "fib<5> (TemplateInstantiation)\n");

  sh.clear_output();
  sh.line_available("bt");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "#0 fib<5> (TemplateInstantiation)\n"
      "#1 fib<6> (TemplateInstantiation)\n"
      "#2 int_<fib<6>::value>\n");

  sh.clear_output();
  sh.line_available("ft");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "fib<5> (TemplateInstantiation)\n"
      "+ fib<4> (TemplateInstantiation)\n"
      "| + fib<3> (TemplateInstantiation)\n"
      "| | + fib<2> (TemplateInstantiation)\n"
      "| | | + fib<1> (Memoization)\n"
      "| | | ` fib<0> (Memoization)\n"
      "| | ` fib<1> (Memoization)\n"
      "| ` fib<2> (Memoization)\n"
      "` fib<3> (Memoization)\n");

  sh.clear_output();
  sh.line_available("rbreak fib<3>");

  JUST_ASSERT_EQUAL(sh.get_output(), "Breakpoint \"fib<3>\" added\n");

  sh.clear_output();
  sh.line_available("continue");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Breakpoint \"fib<3>\" reached\n"
      "fib<3> (TemplateInstantiation)\n");

  sh.clear_output();
  sh.line_available("c");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Breakpoint \"fib<3>\" reached\n"
      "fib<3> (Memoization)\n");

  sh.clear_output();
  sh.line_available("");

  JUST_ASSERT_EQUAL(sh.get_output(),
      "Metaprogram finished\n"
      "int_<13>\n");
}
#endif
