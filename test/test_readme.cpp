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

#include "test_shell.hpp"
#include "mdb_test_shell.hpp"
#include "util.hpp"

using namespace metashell;

// If one of these TCs fail, then README modification might be needed

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_readme_continue_abbreviated_as_c) {
  mdb_command command;
  std::string args;

  std::tie(command, args) =
    get_command_from_map(mdb_shell::command_handler, "c");

  auto keys = command.get_keys();

  JUST_ASSERT(std::find(keys.begin(), keys.end(), "continue") != keys.end());
}

JUST_TEST_CASE(test_readme_getting_started) {
  test_shell sh;

  JUST_ASSERT_EQUAL(sh.prompt(), "> ");
  sh.line_available("template <int N> \\");
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("struct fib \\");
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("{ \\");
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("  static constexpr int value = fib<N - 1>::value + fib<N - 2>::value; \\");
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("};");
  JUST_ASSERT_EQUAL(sh.prompt(), "> ");
  sh.line_available("template <> \\");
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("struct fib<0> \\");
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("{ \\");
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("  static constexpr int value = 1; \\");
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("};");
  JUST_ASSERT_EQUAL(sh.prompt(), "> ");
  sh.line_available("template <> \\");
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("struct fib<1> \\");
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("{ \\");
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("  static constexpr int value = 1; \\");
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("};");
  JUST_ASSERT_EQUAL(sh.prompt(), "> ");

  JUST_ASSERT_EQUAL(sh.info(), "");
  JUST_ASSERT_EQUAL(sh.error(), "");
  JUST_ASSERT_EQUAL(sh.output(), "");

  sh.line_available("fib<6>::value");

  JUST_ASSERT_EQUAL(sh.info(), "");
  JUST_ASSERT(!sh.error().empty());
  JUST_ASSERT_EQUAL(sh.output(), "");

  sh.clear_output();

  sh.line_available("#include <boost/mpl/int.hpp>");

  JUST_ASSERT_EQUAL(sh.info(), "");
  JUST_ASSERT_EQUAL(sh.error(), "");
  JUST_ASSERT_EQUAL(sh.output(), "");

  sh.line_available("boost::mpl::int_<fib<6>::value>");

  JUST_ASSERT_EQUAL(sh.info(), "");
  JUST_ASSERT_EQUAL(sh.error(), "");
  JUST_ASSERT_EQUAL(sh.output(), "mpl_::int_<13>");

  sh.clear_output();

  sh.line_available("#include <metashell/scalar.hpp>");

  JUST_ASSERT_EQUAL(sh.info(), "");
  JUST_ASSERT_EQUAL(sh.error(), "");
  JUST_ASSERT_EQUAL(sh.output(), "");

  sh.line_available("SCALAR(fib<6>::value)");

  JUST_ASSERT_EQUAL(sh.info(), "");
  JUST_ASSERT_EQUAL(sh.error(), "");
  JUST_ASSERT(is_integral_constant("int", "13", sh.output()));

  sh.clear_output();

  sh.line_available("#include <boost/mpl/vector.hpp>");

  JUST_ASSERT_EQUAL(sh.info(), "");
  JUST_ASSERT_EQUAL(sh.error(), "");
  JUST_ASSERT_EQUAL(sh.output(), "");

  sh.line_available("using namespace boost::mpl;");

  JUST_ASSERT_EQUAL(sh.info(), "");
  JUST_ASSERT_EQUAL(sh.error(), "");
  JUST_ASSERT_EQUAL(sh.output(), "");

  sh.line_available("vector<int, double, char>");

  JUST_ASSERT_EQUAL(sh.info(), "");
  JUST_ASSERT_EQUAL(sh.error(), "");
  JUST_ASSERT_EQUAL(sh.output(),
      "boost::mpl::vector<int, double, char, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mp"
      "l_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mp"
      "l_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na>");

  sh.clear_output();

  sh.line_available("#include <boost/mpl/push_front.hpp>");

  JUST_ASSERT_EQUAL(sh.info(), "");
  JUST_ASSERT_EQUAL(sh.error(), "");
  JUST_ASSERT_EQUAL(sh.output(), "");

  sh.line_available("push_front<vector<int, double, char>, void>::type");

  JUST_ASSERT_EQUAL(sh.info(), "");
  JUST_ASSERT_EQUAL(sh.error(), "");
  JUST_ASSERT_EQUAL(sh.output(),
      "boost::mpl::v_item<void, boost::mpl::vector<int, double, char, mpl_::na, mpl_::n"
      "a, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::n"
      "a, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na>, 1>");

  sh.clear_output();

  sh.line_available("#include <metashell/formatter.hpp>");

  JUST_ASSERT_EQUAL(sh.info(), "");
  JUST_ASSERT_EQUAL(sh.error(), "");
  JUST_ASSERT_EQUAL(sh.output(), "");

  sh.line_available("push_front<vector<int, double, char>, void>::type");

  JUST_ASSERT_EQUAL(sh.info(), "");
  JUST_ASSERT_EQUAL(sh.error(), "");
  JUST_ASSERT_EQUAL(sh.output(),
      "boost_::mpl::vector<void, int, double, char>");

  mdb_test_shell mdb_sh(sh);

  mdb_sh.line_available("evaluate int_<fib<6>::value>");

  JUST_ASSERT_EQUAL(mdb_sh.get_output(), "Metaprogram started\n");

  mdb_sh.clear_output();
  mdb_sh.line_available("step 3");

  JUST_ASSERT_EQUAL(mdb_sh.get_output(), "fib<4> (TemplateInstantiation)\n");

  mdb_sh.clear_output();
  mdb_sh.line_available("step -1");

  JUST_ASSERT_EQUAL(mdb_sh.get_output(), "fib<5> (TemplateInstantiation)\n");

  mdb_sh.clear_output();
  mdb_sh.line_available("bt");

  JUST_ASSERT_EQUAL(mdb_sh.get_output(),
      "#0 fib<5> (TemplateInstantiation)\n"
      "#1 fib<6> (TemplateInstantiation)\n"
      "#2 int_<fib<6>::value>\n");

  mdb_sh.clear_output();
  mdb_sh.line_available("ft");

  JUST_ASSERT_EQUAL(mdb_sh.get_output(),
      "fib<5> (TemplateInstantiation)\n"
      "+ fib<4> (TemplateInstantiation)\n"
      "| + fib<3> (TemplateInstantiation)\n"
      "| | + fib<2> (TemplateInstantiation)\n"
      "| | | + fib<1> (Memoization)\n"
      "| | | ` fib<0> (Memoization)\n"
      "| | ` fib<1> (Memoization)\n"
      "| ` fib<2> (Memoization)\n"
      "` fib<3> (Memoization)\n");

  mdb_sh.clear_output();
  mdb_sh.line_available("rbreak fib<3>");

  JUST_ASSERT_EQUAL(mdb_sh.get_output(),
      "Breakpoint \"fib<3>\" will stop the execution on 2 locations\n");

  mdb_sh.clear_output();
  mdb_sh.line_available("continue");

  JUST_ASSERT_EQUAL(mdb_sh.get_output(),
      "Breakpoint \"fib<3>\" reached\n"
      "fib<3> (TemplateInstantiation)\n");

  mdb_sh.clear_output();
  mdb_sh.line_available("c");

  JUST_ASSERT_EQUAL(mdb_sh.get_output(),
      "Breakpoint \"fib<3>\" reached\n"
      "fib<3> (Memoization)\n");

  mdb_sh.clear_output();
  mdb_sh.line_available("");

  JUST_ASSERT_EQUAL(mdb_sh.get_output(),
      "Metaprogram finished\n"
      "mpl_::int_<13>\n");

  mdb_sh.clear_output();
  mdb_sh.line_available("e");

  JUST_ASSERT_EQUAL(mdb_sh.get_output(),
      "Metaprogram started\n");

  mdb_sh.clear_output();
  mdb_sh.line_available("evaluate -full int_<fib<4>::value>");

  JUST_ASSERT_EQUAL(mdb_sh.get_output(),
      "Metaprogram started\n");

  mdb_sh.clear_output();
  mdb_sh.line_available("ft");

  JUST_ASSERT_EQUAL(mdb_sh.get_output(),
      "int_<fib<4>::value>\n"
      "+ fib<4>\n"
      "| + fib<3>\n"
      "| | + fib<2>\n"
      "| | | + fib<1>\n"
      "| | | ` fib<0>\n"
      "| | ` fib<1>\n"
      "| ` fib<2>\n"
      "|   + fib<1>\n"
      "|   ` fib<0>\n"
      "` mpl_::int_<5>\n");
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_readme_how_to_template_argument_deduction) {
  test_shell sh;

  sh.line_available("#include <vector>");
  sh.line_available("template<class T> void foo(const T& t) { /* ... */ }");

  mdb_test_shell mdb_sh(sh);

  mdb_sh.line_available("evaluate decltype(foo(13))");

  JUST_ASSERT_EQUAL(mdb_sh.get_output(), "Metaprogram started\n");

  mdb_sh.clear_output();
  mdb_sh.line_available("ft");

  JUST_ASSERT_EQUAL(mdb_sh.get_output(),
      "decltype(foo(13))\n"
      "+ foo<int> (TemplateInstantiation)\n"
      "` void (NonTemplateType)\n");

  mdb_sh.clear_output();
  mdb_sh.line_available("eval decltype(foo(std::vector<int>{}))");

  JUST_ASSERT_EQUAL(mdb_sh.get_output(), "Metaprogram started\n");

  mdb_sh.clear_output();
  mdb_sh.line_available("rbreak foo");

  JUST_ASSERT_EQUAL(mdb_sh.get_output(),
      "Breakpoint \"foo\" will stop the execution on 1 location\n");

  mdb_sh.clear_output();
  mdb_sh.line_available("continue");

  const auto o = mdb_sh.get_output();
  JUST_ASSERT(
    o ==
      "Breakpoint \"foo\" reached\n"
      "foo<std::vector<int, std::allocator<int> > > (TemplateInstantiation)\n"
    ||
    o ==
      "Breakpoint \"foo\" reached\n"
      "foo<std::__1::vector<int, std::__1::allocator<int> > > (TemplateInstantiation)\n"
  );
}
#endif
