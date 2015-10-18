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

#include <metashell_system_test/prompt.hpp>
#include <metashell_system_test/error.hpp>
#include <metashell_system_test/type.hpp>
#include <metashell_system_test/raw_text.hpp>
#include <metashell_system_test/backtrace.hpp>
#include <metashell_system_test/call_graph.hpp>
#include <metashell_system_test/frame.hpp>
#include <metashell_system_test/json_generator.hpp>
#include <metashell_system_test/run_metashell.hpp>

#include <just/test.hpp>

using namespace metashell_system_test;

// If one of these TCs fail, then README modification might be needed

JUST_TEST_CASE(test_readme_getting_started)
{
  const auto r =
    run_metashell(
      {
        command("template <int N> \\"),
        command("struct fib \\"),
        command("{ \\"),
        command("  static constexpr int value = fib<N - 1>::value + fib<N - 2>::value; \\"),
        command("};"),

        command("template <> \\"),
        command("struct fib<0> \\"),
        command("{ \\"),
        command("  static constexpr int value = 1; \\"),
        command("};"),

        command("template <> \\"),
        command("struct fib<1> \\"),
        command("{ \\"),
        command("  static constexpr int value = 1; \\"),
        command("};"),

        command("fib<6>::value"),

        command("#include <boost/mpl/int.hpp>"),
        command("boost::mpl::int_<fib<6>::value>"),

        command("#include <metashell/scalar.hpp>"),
        command("SCALAR(fib<6>::value)"),

        command("#include <boost/mpl/vector.hpp>"),
        command("using namespace boost::mpl;"),
        command("vector<int, double, char>"),

        command("#include <boost/mpl/push_front.hpp>"),
        command("push_front<vector<int, double, char>, void>::type"),

        command("#include <metashell/formatter.hpp>"),
        command("push_front<vector<int, double, char>, void>::type"),

        command("#msh mdb"),
        command("evaluate int_<fib<6>::value>"),
        command("step 3"),
        command("step -1"),
        command("bt"),
        command("ft"),
        command("rbreak fib<3>"),
        command("continue"),
        command("c"),
        command(""),
        command("e"),
        command("evaluate -full int_<fib<4>::value>"),
        command("ft")
      }
    );

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("...>"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("...>"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("...>"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("...>"), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("...>"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("...>"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("...>"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("...>"), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("...>"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("...>"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("...>"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("...>"), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(error(_), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(type("mpl_::int_<13>"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT(
      type("std::integral_constant<int, 13>") == *i ||
      type("std::__1::integral_constant<int, 13>") == *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(
    type(
      "boost::mpl::vector<"
        "int, double, char,"
        " mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na,"
        " mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na,"
        " mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na"
      ">"
    ),
    *i
  );
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(
    type(
      "boost::mpl::v_item<"
        "void,"
        " boost::mpl::vector<"
          "int, double, char,"
          " mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na,"
          " mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na,"
          " mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na"
        ">,"
        " 1"
      ">"
    ),
    *i
  );
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(type("boost_::mpl::vector<void, int, double, char>"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;

  JUST_ASSERT_EQUAL(
    frame(type("fib<4>"), instantiation_kind::template_instantiation),
    *i
  );
  ++i;
  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;

  JUST_ASSERT_EQUAL(
    frame(type("fib<5>"), instantiation_kind::template_instantiation),
    *i
  );
  ++i;

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(
    backtrace(
      {
        frame(type("fib<5>"), instantiation_kind::template_instantiation),
        frame(type("fib<6>"), instantiation_kind::template_instantiation),
        frame(type("int_<fib<6>::value>"))
      }
    ),
    *i
  );
  ++i;

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(type("fib<5>"), instantiation_kind::template_instantiation), 0, 2},
        {frame( type("fib<4>"), instantiation_kind::template_instantiation), 1, 2},
        {frame(  type("fib<3>"), instantiation_kind::template_instantiation), 2, 2},
        {frame(   type("fib<2>"), instantiation_kind::template_instantiation), 3, 2},
        {frame(    type("fib<1>"), instantiation_kind::memoization), 4, 0},
        {frame(    type("fib<0>"), instantiation_kind::memoization), 4, 0},
        {frame(   type("fib<1>"), instantiation_kind::memoization), 3, 0},
        {frame(  type("fib<2>"), instantiation_kind::memoization), 2, 0},
        {frame( type("fib<3>"), instantiation_kind::memoization), 1, 0}
      }
    ),
    *i
  );
  ++i;

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(
    raw_text("Breakpoint \"fib<3>\" will stop the execution on 2 locations"),
    *i
  );
  ++i;

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(raw_text("Breakpoint \"fib<3>\" reached"), *i);
  ++i;
  JUST_ASSERT_EQUAL(
    frame(type("fib<3>"), instantiation_kind::template_instantiation),
    *i
  );
  ++i;

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(raw_text("Breakpoint \"fib<3>\" reached"), *i);
  ++i;
  JUST_ASSERT_EQUAL(frame(type("fib<3>"), instantiation_kind::memoization), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i);
  ++i;
  JUST_ASSERT_EQUAL(type("mpl_::int_<13>"), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(type("int_<fib<4>::value>")), 0, 2},
        {frame( type("fib<4>")), 1, 2},
        {frame(  type("fib<3>")), 2, 2},
        {frame(   type("fib<2>")), 3, 2},
        {frame(    type("fib<1>")), 4, 0},
        {frame(    type("fib<0>")), 4, 0},
        {frame(   type("fib<1>")), 3, 0},
        {frame(  type("fib<2>")), 2, 2},
        {frame(   type("fib<1>")), 3, 0},
        {frame(   type("fib<0>")), 3, 0},
        {frame( type("mpl_::int_<5>")), 1, 0}
      }
    ),
    *i
  );
  ++i;

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(raw_text(""), *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

JUST_TEST_CASE(test_readme_how_to_template_argument_deduction)
{
  const auto r =
    run_metashell(
      {
        command("#include <vector>"),
        command("template<class T> void foo(const T& t) { /* ... */ }"),
        command("#msh mdb"),
        command("evaluate decltype(foo(13))"),
        command("ft"),
        command("eval decltype(foo(std::vector<int>{}))"),
        command("rbreak foo"),
        command("continue 2")
      }
    );

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(
    call_graph(
      {
        {frame(type("decltype(foo(13))")), 0, 3},
        {frame(type("foo"), instantiation_kind::deduced_template_argument_substitution), 1, 0},
        {frame( type("foo<int>"), instantiation_kind::template_instantiation), 1, 0},
        {frame( type("void"), instantiation_kind::non_template_type), 1, 0}
      }
    ),
    *i
  );
  ++i;

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram started"), *i);
  ++i;

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(
    raw_text("Breakpoint \"foo\" will stop the execution on 2 locations"),
    *i
  );
  ++i;

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(raw_text("Breakpoint \"foo\" reached"), *i);
  ++i;
  JUST_ASSERT(
    frame(
      type("foo<std::vector<int, std::allocator<int> > >"),
      instantiation_kind::template_instantiation
    ) == *i
    ||
    frame(
      type("foo<std::__1::vector<int, std::__1::allocator<int> > >"),
      instantiation_kind::template_instantiation
    ) == *i
  );
  ++i;

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
  ++i;
  JUST_ASSERT_EQUAL(raw_text(""), *i);
  ++i;

  JUST_ASSERT(i == r.end());
}

