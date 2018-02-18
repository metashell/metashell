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

#include <metashell/system_test/call_graph.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/raw_text.hpp>
#include <metashell/system_test/type.hpp>

#include "fib.hpp"
#include "test_metaprograms.hpp"

#include <gtest/gtest.h>

using namespace metashell::system_test;

using pattern::_;

TEST(mdb_evaluate, int_type)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  ASSERT_EQ(
      raw_text("Metaprogram started"), mi.command("evaluate int").front());
  ASSERT_EQ((std::vector<json_string>{
                to_json_string(raw_text("Metaprogram finished")),
                to_json_string(type("int")), to_json_string(prompt("(mdb)"))}),
            mi.command("continue"));
}

TEST(mdb_evaluate, fib_10)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");

  ASSERT_EQ(raw_text("Metaprogram started"),
            mi.command("evaluate int_<fib<10>::value>").front());
  ASSERT_EQ(
      (std::vector<json_string>{
          to_json_string(raw_text("Metaprogram finished")),
          to_json_string(type("int_<55>")), to_json_string(prompt("(mdb)"))}),
      mi.command("continue"));
}

TEST(mdb_evaluate, empty_environment)
{
  metashell_instance mi;
  mi.command("#msh mdb");
  mi.command("continue");

  ASSERT_EQ(raw_text("Metaprogram started"), mi.command("evaluate -").front());
  ASSERT_EQ((std::vector<json_string>{
                to_json_string(raw_text("Metaprogram finished")),
                to_json_string(prompt("(mdb)"))}),
            mi.command("continue"));
}

TEST(mdb_evaluate, no_arguments_no_evaluation)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  ASSERT_EQ(
      error("Nothing has been evaluated yet."), mi.command("evaluate").front());
}

TEST(mdb_evaluate, no_arguments_with_trailing_spaces_no_evaluation)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  ASSERT_EQ(error("Nothing has been evaluated yet."),
            mi.command("evaluate  ").front());
}

TEST(mdb_evaluate, failure_will_reset_metaprogram_state)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  ASSERT_EQ(
      raw_text("Metaprogram started"), mi.command("evaluate int").front());
  ASSERT_EQ(raw_text("Metaprogram started"), mi.command("evaluate in").front());
  ASSERT_EQ(error(_), mi.command("step")[1]);
  ASSERT_EQ(
      raw_text("Metaprogram started"), mi.command("evaluate int").front());
}

TEST(mdb_evaluate, instantiation_failure_will_start_metaprogram)
{
  metashell_instance mi;
  mi.command(missing_value_fibonacci_mp);
  mi.command("#msh mdb");

  ASSERT_EQ(raw_text("Metaprogram started"),
            mi.command("evaluate int_<fib<5>::value>").front());
}

TEST(mdb_evaluate, missing_argument_will_run_last_metaprogram)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  ASSERT_EQ(
      raw_text("Metaprogram started"), mi.command("evaluate int").front());
  ASSERT_EQ(raw_text("Metaprogram started"), mi.command("evaluate").front());
  ASSERT_EQ((std::vector<json_string>{
                to_json_string(raw_text("Metaprogram finished")),
                to_json_string(type("int")), to_json_string(prompt("(mdb)"))}),
            mi.command("continue"));
}

TEST(mdb_evaluate, missing_argument_will_reset_metaprogram_state)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<10>::value>");
  const std::vector<json_string> first_ft = mi.command("ft");
  mi.command("step 5");
  mi.command("evaluate");
  ASSERT_EQ(first_ft, mi.command("ft"));
}

TEST(mdb_evaluate, reevaluate_environment)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  ASSERT_EQ(raw_text("Metaprogram started"), mi.command("evaluate -").front());
  ASSERT_EQ((std::vector<json_string>{
                to_json_string(raw_text("Metaprogram finished")),
                to_json_string(prompt("(mdb)"))}),
            mi.command("continue"));
  ASSERT_EQ(raw_text("Metaprogram started"), mi.command("evaluate").front());
  ASSERT_EQ((std::vector<json_string>{
                to_json_string(raw_text("Metaprogram finished")),
                to_json_string(prompt("(mdb)"))}),
            mi.command("continue"));
}

TEST(mdb_evaluate, filters_similar_edges)
{
  metashell_instance mi;
  mi.command(fibonacci_with_enum_mp);
  mi.command("#msh mdb");

  ASSERT_EQ(raw_text("Metaprogram started"),
            mi.command("evaluate int_<fib<2>::value>").front());

// On Windows clang tries to be compatible with MSVC, and this affects the Sema
// code to take slightly different paths. Probably because of this, Memoization
// events are not generated for the nested enum type.
#ifdef _WIN32
  ASSERT_EQ(
      call_graph(
          {{frame(type("int_<fib<2>::value>")), 0, 3},
           {frame(fib<2>(), _, _, event_kind::template_instantiation), 1, 2},
           {frame(fib<0>(), _, _, event_kind::memoization), 2, 0},
           {frame(fib<1>(), _, _, event_kind::memoization), 2, 0},
           {frame(fib<2>(), _, _, event_kind::memoization), 1, 0},
           {frame(type("int_<1>"), _, _, event_kind::template_instantiation), 1,
            0}}),
      mi.command("forwardtrace").front());
#else
  ASSERT_EQ(
      call_graph(
          {{frame(type("int_<fib<2>::value>")), 0, 4},
           {frame(fib<2>(), _, _, event_kind::template_instantiation), 1, 4},
           {frame(fib<0>(), _, _, event_kind::memoization), 2, 0},
           {frame(fib<1>(), _, _, event_kind::memoization), 2, 0},
           {frame(type("fib<1>::ENUM"), _, _, event_kind::memoization), 2, 0},
           {frame(type("fib<0>::ENUM"), _, _, event_kind::memoization), 2, 0},
           {frame(fib<2>(), _, _, event_kind::memoization), 1, 0},
           {frame(type("fib<2>::ENUM"), _, _, event_kind::memoization), 1, 0},
           {frame(type("int_<1>"), _, _, event_kind::template_instantiation), 1,
            0}}),
      mi.command("forwardtrace").front());
#endif
}

TEST(mdb_evaluate, clears_breakpoints)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  ASSERT_EQ(
      raw_text("Metaprogram started"), mi.command("evaluate int").front());
  ASSERT_EQ(
      raw_text("Breakpoint \"int\" will stop the execution on 1 location"),
      mi.command("rbreak int").front());
  ASSERT_EQ(raw_text("Breakpoint 1: regex(\"int\")"),
            mi.command("break list").front());
  ASSERT_EQ(raw_text("Metaprogram started"),
            mi.command("evaluate unsigned int").front());
  ASSERT_EQ(raw_text("No breakpoints currently set"),
            mi.command("break list").front());
}

TEST(mdb_evaluate, reevaluate_clears_breakpoints)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  ASSERT_EQ(
      raw_text("Metaprogram started"), mi.command("evaluate int").front());
  ASSERT_EQ(
      raw_text("Breakpoint \"int\" will stop the execution on 1 location"),
      mi.command("rbreak int").front());
  ASSERT_EQ(raw_text("Breakpoint 1: regex(\"int\")"),
            mi.command("break list").front());
  ASSERT_EQ(raw_text("Metaprogram started"), mi.command("evaluate").front());
  ASSERT_EQ(raw_text("No breakpoints currently set"),
            mi.command("break list").front());
}

TEST(mdb_evaluate, failure_clears_breakpoints)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  ASSERT_EQ(
      raw_text("Metaprogram started"), mi.command("evaluate int").front());
  ASSERT_EQ(
      raw_text("Breakpoint \"int\" will stop the execution on 1 location"),
      mi.command("rbreak int").front());
  ASSERT_EQ(raw_text("Breakpoint 1: regex(\"int\")"),
            mi.command("break list").front());
  ASSERT_EQ(
      raw_text("Metaprogram started"), mi.command("evaluate asd").front());
  ASSERT_EQ(error(_), mi.command("step")[1]);
  ASSERT_EQ(raw_text("No breakpoints currently set"),
            mi.command("break list").front());
}
