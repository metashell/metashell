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

#include <metashell/system_test/any_of.hpp>
#include <metashell/system_test/backtrace.hpp>
#include <metashell/system_test/call_graph.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/frame.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/raw_text.hpp>
#include <metashell/system_test/type.hpp>

#include "test_metaprograms.hpp"

#include <gtest/gtest.h>

using namespace metashell::system_test;

using pattern::_;

// If one of these TCs fail, then README modification might be needed

TEST(readme, getting_started)
{
  metashell_instance mi;

  mi.command("template <int N> \\");
  mi.command("struct fib \\");
  mi.command("{ \\");
  mi.command(
      "  static constexpr int value = fib<N - 1>::value + fib<N - 2>::value; "
      "\\");
  mi.command("};");

  mi.command("template <> \\");
  mi.command("struct fib<0> \\");
  mi.command("{ \\");
  mi.command("  static constexpr int value = 1; \\");
  mi.command("};");

  mi.command("template <> \\");
  mi.command("struct fib<1> \\");
  mi.command("{ \\");
  mi.command("  static constexpr int value = 1; \\");
  mi.command("};");

  ASSERT_EQ(error(_), mi.command("fib<6>::value").front());

  mi.command("#include <boost/mpl/int.hpp>");

  ASSERT_EQ(type("mpl_::int_<13>"),
            mi.command("boost::mpl::int_<fib<6>::value>").front());

  mi.command("#include <metashell/scalar.hpp>");

  const json_string ic13 = mi.command("SCALAR(fib<6>::value)").front();
  ASSERT_EQ(any_of<type>("std::integral_constant<int, 13>",
                         "std::__1::integral_constant<int, 13>"),
            ic13);

  // clang-format off

  mi.command("#include <boost/mpl/vector.hpp>");
  mi.command("using namespace boost::mpl;");
  ASSERT_EQ(
    any_of<type>(
      type{
        "boost::mpl::vector<"
          "int, double, char,"
          " mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na,"
          " mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na,"
          " mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na"
        ">"
      },
      type{"boost::mpl::vector<int, double, char>"}
    ),
    mi.command("vector<int, double, char>").front()
  );

  mi.command("#include <boost/mpl/push_front.hpp>");
  ASSERT_EQ(
    any_of<type>(
    type{
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
    },
    type{"boost::mpl::v_item<void, boost::mpl::vector<int, double, char>, 1>"}
    ),
    mi.command("push_front<vector<int, double, char>, void>::type").front()
  );

  // clang-format on

  mi.command("#include <metashell/formatter.hpp>");
  ASSERT_EQ(
      type("boost_::mpl::vector<void, int, double, char>"),
      mi.command("push_front<vector<int, double, char>, void>::type").front());

  ASSERT_EQ(std::vector<json_string>{to_json_string(prompt("(mdb)"))},
            mi.command("#msh mdb"));

  ASSERT_EQ(raw_text("Metaprogram started"),
            mi.command("evaluate int_<fib<6>::value>").front());

  ASSERT_EQ(any_of<frame>(
                frame{type{"fib<4>"}, _, _, event_kind::template_instantiation},
                frame{type{"fib<6>::value"}, _, _,
                      event_kind::template_instantiation}),
            mi.command("step 3").front());

  ASSERT_EQ(any_of<frame>(
                frame{type{"fib<5>"}, _, _, event_kind::template_instantiation},
                frame{type{"fib<6>"}, _, _, event_kind::memoization}),
            mi.command("step -1").front());

  ASSERT_EQ(
      any_of<backtrace>(
          backtrace{
              {frame{type{"fib<5>"}, _, _, event_kind::template_instantiation},
               frame{type{"fib<6>"}, _, _, event_kind::template_instantiation},
               frame{type{"int_<fib<6>::value>"}}}},
          backtrace{{frame{type{"fib<6>"}, _, _, event_kind::memoization},
                     frame{type{"int_<fib<6>::value>"}}}}),
      mi.command("bt").front());

  // clang-format off

  ASSERT_EQ(
    any_of<call_graph>(
      call_graph{
        {
          {frame{type{"fib<5>"}, _, _, event_kind::template_instantiation}, 0, 3},
          {frame{ type{"fib<4>"}, _, _, event_kind::template_instantiation}, 1, 3},
          {frame{  type{"fib<3>"}, _, _, event_kind::template_instantiation}, 2, 3},
          {frame{   type{"fib<2>"}, _, _, event_kind::template_instantiation}, 3, 2},
          {frame{    type{"fib<1>"}, _, _, event_kind::memoization}, 4, 0},
          {frame{    type{"fib<0>"}, _, _, event_kind::memoization}, 4, 0},
          {frame{   type{"fib<2>"}, _, _, event_kind::memoization}, 3, 0},
          {frame{   type{"fib<1>"}, _, _, event_kind::memoization}, 3, 0},
          {frame{  type{"fib<3>"}, _, _, event_kind::memoization}, 2, 0},
          {frame{  type{"fib<2>"}, _, _, event_kind::memoization}, 2, 0},
          {frame{ type{"fib<4>"}, _, _, event_kind::memoization}, 1, 0},
          {frame{ type{"fib<3>"}, _, _, event_kind::memoization}, 1, 0}
        }
      },
      call_graph{
        {
          {frame{type{"fib<6>"}, _, _, event_kind::memoization}, 0, 0}
        }
      }
    ),
    mi.command("ft").front()
  );

  // clang-format on

  ASSERT_EQ(any_of<raw_text>(
                "Breakpoint \"fib<3>\" will stop the execution on 3 locations",
                "Breakpoint \"fib<3>\" will stop the execution on 4 locations"),
            mi.command("rbreak fib<3>").front());

  ASSERT_EQ(
      (std::vector<json_string>{
          to_json_string(raw_text("Breakpoint 1: regex(\"fib<3>\") reached")),
          to_json_string(
              frame(type("fib<3>"), _, _, event_kind::template_instantiation)),
          to_json_string(prompt("(mdb)"))}),
      mi.command("continue"));

  ASSERT_EQ(any_of<std::vector<json_string>>(
                std::vector<json_string>{
                    to_json_string(
                        raw_text{"Breakpoint 1: regex(\"fib<3>\") reached"}),
                    to_json_string(
                        frame{type{"fib<3>"}, _, _, event_kind::memoization}),
                    to_json_string(prompt{"(mdb)"})},
                std::vector<json_string>{
                    to_json_string(
                        raw_text{"Breakpoint 1: regex(\"fib<3>\") reached"}),
                    to_json_string(frame{type{"fib<3>::value"}, _, _,
                                         event_kind::template_instantiation}),
                    to_json_string(prompt{"(mdb)"})}),
            mi.command("c 2"));

  ASSERT_EQ((std::vector<json_string>{
                to_json_string(raw_text("Metaprogram finished")),
                to_json_string(type("mpl_::int_<13>")),
                to_json_string(prompt("(mdb)"))}),
            mi.command(""));

  ASSERT_EQ(raw_text("Metaprogram started"), mi.command("e").front());

  ASSERT_EQ(raw_text("Metaprogram started"),
            mi.command("evaluate -full int_<fib<4>::value>").front());

  // clang-format off

  ASSERT_EQ(
    any_of<call_graph>(
      call_graph{
        {
          {frame{type{"int_<fib<4>::value>"}}, 0, 2},
          {frame{ type{"fib<4>"}}, 1, 2},
          {frame{  type{"fib<3>"}}, 2, 2},
          {frame{   type{"fib<2>"}}, 3, 2},
          {frame{    type{"fib<1>"}}, 4, 0},
          {frame{    type{"fib<0>"}}, 4, 0},
          {frame{   type{"fib<1>"}}, 3, 0},
          {frame{  type{"fib<2>"}}, 2, 2},
          {frame{   type{"fib<1>"}}, 3, 0},
          {frame{   type{"fib<0>"}}, 3, 0},
          {frame{ type{"mpl_::int_<5>"}}, 1, 0}
        }
      },
      call_graph{
        {
          {frame{type{"int_<fib<4>::value>"}}, 0, 3},
          {frame{ type{"fib<4>"}}, 1, 0},
          {frame{ type{"fib<4>::value"}}, 1, 3},
          {frame{  type{"fib<3>"}}, 2, 0},
          {frame{  type{"fib<3>::value"}}, 2, 3},
          {frame{   type{"fib<2>"}}, 3, 0},
          {frame{   type{"fib<2>::value"}}, 3, 2},
          {frame{    type{"fib<1>"}}, 4, 0},
          {frame{    type{"fib<0>"}}, 4, 0},
          {frame{   type{"fib<1>"}}, 3, 0},
          {frame{  type{"fib<2>"}}, 2, 0},
          {frame{ type{"mpl_::int_<5>"}}, 1, 0}
        }
      }
    ),
    mi.command("ft").front()
  );

  // clang-format on
}

TEST(readme, how_to_template_argument_deduction)
{
  metashell_instance mi;

  mi.command("#include <vector>");
  mi.command("template<class T> void foo(const T& t) { /* ... */ }");
  mi.command("#msh mdb");

  ASSERT_EQ(raw_text("Metaprogram started"),
            mi.command("evaluate decltype(foo(13))").front());

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame(type("decltype(foo(13))")), 0, 3},
        {frame(type("foo"), _, _, event_kind::deduced_template_argument_substitution), 1, 0},
        {frame( type("foo<int>"), _, _, event_kind::template_instantiation), 1, 0},
        {frame( type("void"), _, _, event_kind::non_template_type), 1, 0}
      }
    ),
  mi.command("ft").front());

  // clang-format on

  ASSERT_EQ(raw_text("Metaprogram started"),
            mi.command("eval decltype(foo(std::vector<int>{}))").front());

  ASSERT_EQ(
      raw_text("Breakpoint \"foo\" will stop the execution on 2 locations"),
      mi.command("rbreak foo").front());

  const std::vector<json_string> cont = mi.command("continue 2");

  ASSERT_EQ(raw_text("Breakpoint 1: regex(\"foo\") reached"), cont[0]);
  ASSERT_EQ(
      any_of<frame>(
          frame{type{"foo<std::vector<int>>"}, _, _,
                event_kind::template_instantiation},
          frame{type{"foo<std::vector<int, std::allocator<int>>>"}, _, _,
                event_kind::template_instantiation},
          frame{type{"foo<std::__1::vector<int, std::__1::allocator<int>>>"}, _,
                _, event_kind::template_instantiation},
          frame{type{"foo<std::vector<int, std::allocator<int> > >"}, _, _,
                event_kind::template_instantiation},
          frame{type{"foo<std::__1::vector<int, std::__1::allocator<int> > >"},
                _, _, event_kind::template_instantiation}),
      cont[1]);
}

TEST(readme, how_to_sfinae)
{
  metashell_instance mi;

  // clang-format off

  mi.command(make_unique_sfinae_mp);
  mi.command("#msh mdb decltype(make_unique<int>(15))");
  ASSERT_EQ(
    call_graph(
      {
        {frame(type("decltype(make_unique<int>(15))")), 0, 6},
        {frame( type("make_unique"), _, _, event_kind::explicit_template_argument_substitution), 1, 2},
        {frame(  type("unique_if<int>"), _, _, event_kind::template_instantiation), 2, 0},
        {frame(  type("unique_if<int>"), _, _, event_kind::memoization), 2, 0},
        {frame( type("make_unique"), _, _, event_kind::explicit_template_argument_substitution), 1, 1},
        {frame(  type("unique_if<int>"), _, _, event_kind::memoization), 2, 0},
        {frame( type("make_unique"), _, _, event_kind::explicit_template_argument_substitution), 1, 1},
        {frame(  type("unique_if<int>"), _, _, event_kind::memoization), 2, 0},
        {frame( type("make_unique"), _, _, event_kind::deduced_template_argument_substitution), 1, 1},
        {frame(  type("unique_if<int>"), _, _, event_kind::memoization), 2, 0},
        {frame( type("make_unique<int, int>"), _, _, event_kind::template_instantiation), 1, 0},
        {frame( type("_std::unique_ptr<int>"), _, _, event_kind::template_instantiation), 1, 0}
      }
    ),
  mi.command("ft").front());

  // clang-format on
}
