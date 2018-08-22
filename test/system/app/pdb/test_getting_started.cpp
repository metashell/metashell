// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/backtrace.hpp>
#include <metashell/system_test/call_graph.hpp>
#include <metashell/system_test/cpp_code.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/frame.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/raw_text.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

using pattern::_;

// If one of these TCs fail, then getting_started.md modification might be
// needed

TEST(getting_started, using_the_preprocessor_debugger)
{
  metashell_instance mi;

  mi.command("#msh preprocessor mode");
  mi.command("#define CONST_PTR(x) const x*");

  ASSERT_EQ(cpp_code("const int*"), mi.command("CONST_PTR(int)").front());

  ASSERT_EQ(cpp_code("const double*"), mi.command("CONST_PTR(double)").front());

  mi.command("#define REPEAT(n, m) REPEAT ## n(m)");
  mi.command("#define REPEAT0(m)");
  mi.command("#define REPEAT1(m) m");
  mi.command("#define REPEAT2(m) m, REPEAT1(m)");
  mi.command("#define REPEAT3(m) m, REPEAT2(m)");
  mi.command("#define REPEAT4(m) m, REPEAT3(m)");
  mi.command("#define REPEAT5(m) m, REPEAT4(m)");

  ASSERT_EQ(cpp_code("int, int, int"), mi.command("REPEAT(3, int)").front());

  ASSERT_EQ(cpp_code("const int*, const int*, const int*"),
            mi.command("REPEAT(3, CONST_PTR(int))").front());

  {
    const auto launch_pdb = mi.command("#msh pdb REPEAT(3, CONST_PTR(int))");
    ASSERT_EQ(2u, launch_pdb.size());
    ASSERT_EQ(raw_text("Metaprogram started"), launch_pdb[0]);
    ASSERT_EQ(prompt("(pdb)"), launch_pdb[1]);
  }

  // Stepping
  ///////////

  ASSERT_EQ(frame(type("const int*"), _, _, event_kind::rescanning),
            mi.command("step 3").front());
  ASSERT_EQ(frame(type("CONST_PTR(int)"), _, _, event_kind::macro_expansion),
            mi.command("step -1").front());

  // Backtrace
  ////////////

  // clang-format off

  ASSERT_EQ(
    backtrace(
      {
        frame(type("CONST_PTR(int)"), _, _, event_kind::macro_expansion),
        frame(type("REPEAT(3, CONST_PTR(int))"), _, _, event_kind::macro_expansion),
        frame(type("REPEAT(3, CONST_PTR(int))"))
      }
    ),
    mi.command("bt").front()
  );

  // clang-format on

  // Forwardtrace
  ///////////////

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame(type("CONST_PTR(int)"), _, _, event_kind::macro_expansion), 0, 1},
        {frame(type("const int*"), _, _, event_kind::rescanning), 1, 1},
        {frame(type("const int*"), _, _, event_kind::expanded_code), 2, 0}
      }
    ),
    mi.command("ft").front()
  );

  // clang-format on

  // Breakpoints and continue
  ///////////////////////////

  ASSERT_EQ(
      raw_text(
          "Breakpoint \"const int\" will stop the execution on 13 locations"),
      mi.command("rbreak const int").front());

  // clang-format off

  const std::vector<std::pair<std::string, frame>> cmd_frames{
    {"continue", frame(type("const int*"), _, _, event_kind::rescanning)},
    {"c", frame(type("const int*"), _, _, event_kind::expanded_code)},
    {"", frame(type("REPEAT3( const int*)"), _, _, event_kind::rescanning)},
    {"", frame(type("REPEAT3( const int*)"), _, _, event_kind::macro_expansion)},
    {"", frame(type(" const int*, REPEAT2( const int*)"), _, _, event_kind::rescanning)},
    {"", frame(type("REPEAT2( const int*)"), _, _, event_kind::macro_expansion)},
    {"", frame(type(" const int*, REPEAT1( const int*)"), _, _, event_kind::rescanning)},
    {"", frame(type("REPEAT1( const int*)"), _, _, event_kind::macro_expansion)},
    {"", frame(type(" const int*"), _, _, event_kind::rescanning)},
    {"", frame(type("const int*"), _, _, event_kind::expanded_code)},
    {"", frame(type("const int*, const int*"), _, _, event_kind::expanded_code)},
    {"", frame(type("const int*, const int*, const int*"), _, _, event_kind::expanded_code)},
    {"", frame(type("const int*, const int*, const int*"), _, _, event_kind::expanded_code)}
  };

  // clang-format on

  for (const auto& cmd_frame : cmd_frames)
  {
    const auto cont = mi.command(cmd_frame.first);
    ASSERT_GT(cont.size(), 2u);
    ASSERT_EQ(raw_text("Breakpoint 1: regex(\"const int\") reached"), cont[0]);
    ASSERT_EQ(cmd_frame.second, cont[1]);
  }

  {
    const auto fin = mi.command("");
    ASSERT_GT(fin.size(), 2u);
    ASSERT_EQ(raw_text("Metaprogram finished"), fin[0]);
    ASSERT_EQ(cpp_code("const int*, const int*, const int*"), fin[1]);
  }

  mi.command("evaluate REPEAT(3, CONST_PTR(int))");

  // Using step over
  //////////////////

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame(type("REPEAT(3, CONST_PTR(int))")), 0, 18},
        {frame( type("REPEAT(3, CONST_PTR(int))"), _, _, event_kind::macro_expansion), 1, 2},
        {frame(  type("CONST_PTR(int)"), _, _, event_kind::macro_expansion), 2, 1},
        {frame(   type("const int*"), _, _, event_kind::rescanning), 3, 1},
        {frame(    type("const int*"), _, _, event_kind::expanded_code), 4, 0},
        {frame(  type("REPEAT3( const int*)"), _, _, event_kind::rescanning), 2, 2},
        {frame(   type("REPEAT3( const int*)"), _, _, event_kind::macro_expansion), 3, 1},
        {frame(    type(" const int*, REPEAT2( const int*)"), _, _, event_kind::rescanning), 4, 2},
        {frame(     type("REPEAT2( const int*)"), _, _, event_kind::macro_expansion), 5, 1},
        {frame(      type(" const int*, REPEAT1( const int*)"), _, _, event_kind::rescanning), 6, 2},
        {frame(       type("REPEAT1( const int*)"), _, _, event_kind::macro_expansion), 7, 1},
        {frame(        type(" const int*"), _, _, event_kind::rescanning), 8, 1},
        {frame(         type("const int*"), _, _, event_kind::expanded_code), 9, 0},
        {frame(       type("const int*, const int*"), _, _, event_kind::expanded_code), 7, 0},
        {frame(     type("const int*, const int*, const int*"), _, _, event_kind::expanded_code), 5, 0},
        {frame(   type("const int*, const int*, const int*"), _, _, event_kind::expanded_code), 3, 0},
        {frame( type("const"), _, _, event_kind::generated_token), 1, 0},
        {frame( type(" "), _, _, event_kind::generated_token), 1, 0},
        {frame( type("int"), _, _, event_kind::generated_token), 1, 0},
        {frame( type("*"), _, _, event_kind::generated_token), 1, 0},
        {frame( type(","), _, _, event_kind::generated_token), 1, 0},
        {frame( type(" "), _, _, event_kind::generated_token), 1, 0},
        {frame( type("const"), _, _, event_kind::generated_token), 1, 0},
        {frame( type(" "), _, _, event_kind::generated_token), 1, 0},
        {frame( type("int"), _, _, event_kind::generated_token), 1, 0},
        {frame( type("*"), _, _, event_kind::generated_token), 1, 0},
        {frame( type(","), _, _, event_kind::generated_token), 1, 0},
        {frame( type(" "), _, _, event_kind::generated_token), 1, 0},
        {frame( type("const"), _, _, event_kind::generated_token), 1, 0},
        {frame( type(" "), _, _, event_kind::generated_token), 1, 0},
        {frame( type("int"), _, _, event_kind::generated_token), 1, 0},
        {frame( type("*"), _, _, event_kind::generated_token), 1, 0},
        {frame( type("\\n"), _, _, event_kind::generated_token), 1, 0}
      }
    ),
    mi.command("ft").front()
  );

  // clang-format on

  ASSERT_EQ(
      frame(type("REPEAT1( const int*)"), _, _, event_kind::macro_expansion),
      mi.command("step 10").front());
  ASSERT_EQ(
      frame(type("const int*, const int*"), _, _, event_kind::expanded_code),
      mi.command("step over").front());
  ASSERT_EQ(
      frame(type("REPEAT1( const int*)"), _, _, event_kind::macro_expansion),
      mi.command("step over -1").front());
  ASSERT_EQ(frame(type("const int*"), _, _, event_kind::expanded_code),
            mi.command("step 2").front());
  ASSERT_EQ(
      frame(type("const int*, const int*"), _, _, event_kind::expanded_code),
      mi.command("step over").front());
  ASSERT_EQ(
      frame(type("REPEAT1( const int*)"), _, _, event_kind::macro_expansion),
      mi.command("step over -1").front());

  // Quit the preprocessor debugger
  /////////////////////////////////

  ASSERT_EQ(prompt(">"), mi.command("quit").back());

  // Evaluating failing macros
  ////////////////////////////

  mi.command("#undef REPEAT");
  mi.command("#define REPEAT(n, m) REPEAT ## n(n, m)");

  ASSERT_EQ(error(_), mi.command("REPEAT(3, int)").front());

  {
    const auto launch_pdb = mi.command("#msh pdb REPEAT(3, int)");
    ASSERT_EQ(2u, launch_pdb.size());
    ASSERT_EQ(raw_text("Metaprogram started"), launch_pdb[0]);
    ASSERT_EQ(prompt("(pdb)"), launch_pdb[1]);
  }

  {
    const auto fail = mi.command("continue");
    ASSERT_GT(fail.size(), 2u);
    ASSERT_EQ(raw_text("Metaprogram finished"), fail[0]);
    ASSERT_EQ(error(_), fail[1]);
  }

  // clang-format off

  ASSERT_EQ(
    backtrace(
      {
        frame(type("REPEAT3(3,  int)"), _, _, event_kind::rescanning),
        frame(type("REPEAT(3, int)"), _, _, event_kind::macro_expansion),
        frame(type("REPEAT(3, int)"))
      }
    ),
    mi.command("bt").front()
  );

  // clang-format on
}
