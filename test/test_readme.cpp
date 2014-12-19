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
#include <metashell/in_memory_displayer.hpp>
#include <metashell/shell.hpp>

#include <just/test.hpp>

#include "test_config.hpp"
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
  in_memory_displayer d;
  shell sh(test_config());

  JUST_ASSERT_EQUAL(sh.prompt(), "> ");
  sh.line_available("template <int N> \\", d);
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("struct fib \\", d);
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("{ \\", d);
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available(
    "  static constexpr int value = fib<N - 1>::value + fib<N - 2>::value; \\",
    d
  );
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("};", d);
  JUST_ASSERT_EQUAL(sh.prompt(), "> ");
  sh.line_available("template <> \\", d);
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("struct fib<0> \\", d);
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("{ \\", d);
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("  static constexpr int value = 1; \\", d);
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("};", d);
  JUST_ASSERT_EQUAL(sh.prompt(), "> ");
  sh.line_available("template <> \\", d);
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("struct fib<1> \\", d);
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("{ \\", d);
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("  static constexpr int value = 1; \\", d);
  JUST_ASSERT_EQUAL(sh.prompt(), "...> ");
  sh.line_available("};", d);
  JUST_ASSERT_EQUAL(sh.prompt(), "> ");

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EMPTY_CONTAINER(d.types());

  sh.line_available("fib<6>::value", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
  JUST_ASSERT(!d.errors().empty());
  JUST_ASSERT_EMPTY_CONTAINER(d.types());

  d.clear();

  sh.line_available("#include <boost/mpl/int.hpp>", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EMPTY_CONTAINER(d.types());

  sh.line_available("boost::mpl::int_<fib<6>::value>", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EQUAL_CONTAINER({type("mpl_::int_<13>")}, d.types());

  d.clear();

  sh.line_available("#include <metashell/scalar.hpp>", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EMPTY_CONTAINER(d.types());

  sh.line_available("SCALAR(fib<6>::value)", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EQUAL(1u, d.types().size());
  JUST_ASSERT(d.types().front().is_integral_constant(type("int"), "13"));

  d.clear();

  sh.line_available("#include <boost/mpl/vector.hpp>", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EMPTY_CONTAINER(d.types());

  sh.line_available("using namespace boost::mpl;", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EMPTY_CONTAINER(d.types());

  sh.line_available("vector<int, double, char>", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EQUAL_CONTAINER(
    {
      type(
        "boost::mpl::vector<int, double, char, mpl_::na, mpl_::na, mpl_::na,"
        " mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na,"
        " mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na>"
      )
    },
    d.types()
  );

  d.clear();

  sh.line_available("#include <boost/mpl/push_front.hpp>", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EMPTY_CONTAINER(d.types());

  sh.line_available("push_front<vector<int, double, char>, void>::type", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EQUAL_CONTAINER(
    {
      type(
        "boost::mpl::v_item<void, boost::mpl::vector<int, double, char,"
        " mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na,"
        " mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na,"
        " mpl_::na, mpl_::na, mpl_::na>, 1>"
      )
    },
    d.types()
  );

  d.clear();

  sh.line_available("#include <metashell/formatter.hpp>", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EMPTY_CONTAINER(d.types());

  sh.line_available("push_front<vector<int, double, char>, void>::type", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
  JUST_ASSERT_EMPTY_CONTAINER(d.errors());
  JUST_ASSERT_EQUAL_CONTAINER(
    {type("boost_::mpl::vector<void, int, double, char>")},
    d.types()
  );

  mdb_test_shell mdb_sh(sh);

  mdb_sh.line_available("evaluate int_<fib<6>::value>", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());

  d.clear();
  mdb_sh.line_available("step 3", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<4>"), instantiation_kind::template_instantiation)},
    d.frames()
  );

  d.clear();
  mdb_sh.line_available("step -1", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<5>"), instantiation_kind::template_instantiation)},
    d.frames()
  );

  d.clear();
  mdb_sh.line_available("bt", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      backtrace{
        frame(type("fib<5>"), instantiation_kind::template_instantiation),
        frame(type("fib<6>"), instantiation_kind::template_instantiation),
        frame(type("int_<fib<6>::value>"))
      }
    },
    d.backtraces()
  );

  d.clear();
  mdb_sh.line_available("ft", d);

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(fib<5>(), instantiation_kind::template_instantiation), 0, 2},
      {frame( fib<4>(), instantiation_kind::template_instantiation), 1, 2},
      {frame(  fib<3>(), instantiation_kind::template_instantiation), 2, 2},
      {frame(   fib<2>(), instantiation_kind::template_instantiation), 3, 2},
      {frame(    fib<1>(), instantiation_kind::memoization), 4, 0},
      {frame(    fib<0>(), instantiation_kind::memoization), 4, 0},
      {frame(   fib<1>(), instantiation_kind::memoization), 3, 0},
      {frame(  fib<2>(), instantiation_kind::memoization), 2, 0},
      {frame( fib<3>(), instantiation_kind::memoization), 1, 0}
    },
    d.call_graphs().front()
  );

  d.clear();
  mdb_sh.line_available("rbreak fib<3>", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Breakpoint \"fib<3>\" will stop the execution on 2 locations"},
    d.raw_texts()
  );

  d.clear();
  mdb_sh.line_available("continue", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Breakpoint \"fib<3>\" reached"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<3>"), instantiation_kind::template_instantiation)},
    d.frames()
  );

  d.clear();
  mdb_sh.line_available("c", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Breakpoint \"fib<3>\" reached"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<3>"), instantiation_kind::memoization)},
    d.frames()
  );

  d.clear();
  mdb_sh.line_available("", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("mpl_::int_<13>")}, d.types());

  d.clear();
  mdb_sh.line_available("e", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());

  d.clear();
  mdb_sh.line_available("evaluate -full int_<fib<4>::value>", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());

  d.clear();
  mdb_sh.line_available("ft", d);

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(type("int_<fib<4>::value>")), 0, 2},
      {frame( fib<4>()), 1, 2},
      {frame(  fib<3>()), 2, 2},
      {frame(   fib<2>()), 3, 2},
      {frame(    fib<1>()), 4, 0},
      {frame(    fib<0>()), 4, 0},
      {frame(   fib<1>()), 3, 0},
      {frame(  fib<2>()), 2, 2},
      {frame(   fib<1>()), 3, 0},
      {frame(   fib<0>()), 3, 0},
      {frame( type("mpl_::int_<5>")), 1, 0}
    },
    d.call_graphs().front()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_readme_how_to_template_argument_deduction) {
  in_memory_displayer d;
  shell sh(test_config());

  sh.line_available("#include <vector>", d);
  sh.line_available("template<class T> void foo(const T& t) { /* ... */ }", d);

  mdb_test_shell mdb_sh(sh);

  mdb_sh.line_available("evaluate decltype(foo(13))", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());

  d.clear();
  mdb_sh.line_available("ft", d);

  JUST_ASSERT_EQUAL(1u, d.call_graphs().size());
  JUST_ASSERT_EQUAL_CONTAINER(
    in_memory_displayer::call_graph{
      {frame(type("decltype(foo(13))")), 0, 2},
      {frame( type("foo<int>"),instantiation_kind::template_instantiation),1,0},
      {frame( type("void"), instantiation_kind::non_template_type), 1, 0}
    },
    d.call_graphs().front()
  );

  d.clear();
  mdb_sh.line_available("eval decltype(foo(std::vector<int>{}))", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram started"}, d.raw_texts());

  d.clear();
  mdb_sh.line_available("rbreak foo", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Breakpoint \"foo\" will stop the execution on 1 location"},
    d.raw_texts()
  );

  d.clear();
  mdb_sh.line_available("continue", d);

  JUST_ASSERT_EQUAL(1u, d.raw_texts().size());
  JUST_ASSERT_EQUAL("Breakpoint \"foo\" reached", d.raw_texts()[0]);

  const auto fs = d.frames();
  JUST_ASSERT_EQUAL(1u, fs.size());
  JUST_ASSERT_EQUAL(
    instantiation_kind::template_instantiation,
    fs.front().kind()
  );
  JUST_ASSERT(
    fs.front().name() ==
      type("foo<std::vector<int, std::allocator<int> > >")
    || fs.front().name() ==
      type("foo<std::__1::vector<int, std::__1::allocator<int> > >")
  );
}
#endif

