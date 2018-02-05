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

#include "fib.hpp"
#include "test_metaprograms.hpp"

#include <gtest/gtest.h>

using namespace metashell::system_test;

using pattern::_;

TEST(mdb_forwardtrace, without_evaluation)
{
  metashell_instance mi;
  mi.command("#msh mdb");

  ASSERT_EQ(error("Metaprogram not evaluated yet"),
            mi.command("forwardtrace").front());
}

TEST(mdb_forwardtrace, garbage_argument)
{
  metashell_instance mi;
  mi.command("#msh mdb");
  mi.command("evaluate int");

  ASSERT_EQ(
      error("Argument parsing failed"), mi.command("forwardtrace asd").front());
}

TEST(mdb_forwardtrace, int_type)
{
  metashell_instance mi;
  mi.command("#msh mdb");
  mi.command("evaluate int");

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame(type("int")), 0, 1},
        {frame(type("int"), _, _, event_kind::non_template_type), 1, 0}
      }
    ),
    mi.command("forwardtrace").front()
  );

  // clang-format on
}

TEST(mdb_forwardtrace, int_in_full_mode)
{
  metashell_instance mi;
  mi.command("#msh mdb");
  mi.command("evaluate -full int");

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame(type("int")), 0, 1},
        {frame(type("int")), 1, 0}
      }
    ),
    mi.command("forwardtrace").front()
  );

  // clang-format on
}

TEST(mdb_forwardtrace, metaprogram_finished)
{
  metashell_instance mi;
  mi.command("#msh mdb");
  mi.command("evaluate int");
  mi.command("continue");

  ASSERT_EQ((std::vector<json_string>{
                to_json_string(raw_text("Metaprogram finished")),
                to_json_string(type("int")), to_json_string(prompt("(mdb)"))}),
            mi.command("forwardtrace"));
}

TEST(mdb_forwardtrace, metaprogram_finished_in_full_mode)
{
  metashell_instance mi;
  mi.command("#msh mdb");
  mi.command("evaluate -full int");
  mi.command("continue");

  ASSERT_EQ((std::vector<json_string>{
                to_json_string(raw_text("Metaprogram finished")),
                to_json_string(type("int")), to_json_string(prompt("(mdb)"))}),
            mi.command("forwardtrace"));
}

TEST(mdb_forwardtrace, int_with_ft)
{
  metashell_instance mi;
  mi.command("#msh mdb");
  mi.command("evaluate int");

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame(type("int")), 0, 1},
        {frame(type("int"), _, _, event_kind::non_template_type), 1, 0}
      }
    ),
    mi.command("ft").front()
  );

  // clang-format on
}

TEST(mdb_forwardtrace, from_root)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<5>::value>");

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame(type("int_<fib<5>::value>")), 0, 3},
        {frame( fib<5>(), _, _, event_kind::template_instantiation), 1, 4},
        {frame(  fib<3>(), _, _, event_kind::template_instantiation), 2, 3},
        {frame(   fib<1>(), _, _, event_kind::memoization), 3, 0},
        {frame(   fib<2>(), _, _, event_kind::template_instantiation), 3, 2},
        {frame(    fib<0>(), _, _, event_kind::memoization), 4, 0},
        {frame(    fib<1>(), _, _, event_kind::memoization), 4, 0},
        {frame(   fib<2>(), _, _, event_kind::memoization), 3, 0},
        {frame(  fib<3>(), _, _, event_kind::memoization), 2, 0},
        {frame(  fib<4>(), _, _, event_kind::template_instantiation), 2, 2},
        {frame(   fib<2>(), _, _, event_kind::memoization), 3, 0},
        {frame(   fib<3>(), _, _, event_kind::memoization), 3, 0},
        {frame(  fib<4>(), _, _, event_kind::memoization), 2, 0},
        {frame( fib<5>(), _, _, event_kind::memoization), 1, 0},
        {frame( type("int_<5>"), _, _, event_kind::template_instantiation), 1, 0}
      }
    ),
    mi.command("forwardtrace").front()
  );

  // clang-format on
}

TEST(mdb_forwardtrace, from_root_in_full_mode)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate -full int_<fib<5>::value>");

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame(type("int_<fib<5>::value>")), 0, 2},
        {frame( fib<5>()), 1, 2},
        {frame(  fib<3>()), 2, 2},
        {frame(   fib<1>()), 3, 0},
        {frame(   fib<2>()), 3, 2},
        {frame(    fib<0>()), 4, 0},
        {frame(    fib<1>()), 4, 0},
        {frame(  fib<4>()), 2, 2},
        {frame(   fib<2>()), 3, 2},
        {frame(    fib<0>()), 4, 0},
        {frame(    fib<1>()), 4, 0},
        {frame(   fib<3>()), 3, 2},
        {frame(    fib<1>()), 4, 0},
        {frame(    fib<2>()), 4, 2},
        {frame(     fib<0>()), 5, 0},
        {frame(     fib<1>()), 5, 0},
        {frame( type("int_<5>")), 1,0}
      }
    ),
    mi.command("forwardtrace").front()
  );

  // clang-format on
}

TEST(mdb_forwardtrace, from_memoization)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<5>::value>");
  mi.command("rbreak fib<5>");
  mi.command("continue 2");

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame( fib<5>(), _, _, event_kind::memoization), 0, 0}
      }
    ),
    mi.command("forwardtrace").front()
  );

  // clang-format on
}

TEST(mdb_forwardtrace, ft_from_step_1)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<5>::value>");
  mi.command("step");

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame(fib<5>(), _, _, event_kind::template_instantiation), 0, 4},
        {frame( fib<3>(), _, _, event_kind::template_instantiation), 1, 3},
        {frame(  fib<1>(), _, _, event_kind::memoization), 2, 0},
        {frame(  fib<2>(), _, _, event_kind::template_instantiation), 2, 2},
        {frame(   fib<0>(), _, _, event_kind::memoization), 3, 0},
        {frame(   fib<1>(), _, _, event_kind::memoization), 3, 0},
        {frame(  fib<2>(), _, _, event_kind::memoization), 2, 0},
        {frame( fib<3>(), _, _, event_kind::memoization), 1, 0},
        {frame( fib<4>(), _, _, event_kind::template_instantiation), 1, 2},
        {frame(  fib<2>(), _, _, event_kind::memoization), 2, 0},
        {frame(  fib<3>(), _, _, event_kind::memoization), 2, 0},
        {frame( fib<4>(), _, _, event_kind::memoization), 1, 0}
      }
    ),
    mi.command("forwardtrace").front()
  );

  // clang-format on
}

TEST(mdb_forwardtrace, ft_from_step_1_in_full_mode)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate -full int_<fib<5>::value>");
  mi.command("step");

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame(fib<5>()), 0, 2},
        {frame( fib<3>()), 1, 2},
        {frame(  fib<1>()), 2, 0},
        {frame(  fib<2>()), 2, 2},
        {frame(   fib<0>()), 3, 0},
        {frame(   fib<1>()), 3, 0},
        {frame( fib<4>()), 1, 2},
        {frame(  fib<2>()), 2, 2},
        {frame(   fib<0>()), 3, 0},
        {frame(   fib<1>()), 3, 0},
        {frame(  fib<3>()), 2, 2},
        {frame(   fib<1>()), 3, 0},
        {frame(   fib<2>()), 3, 2},
        {frame(    fib<0>()), 4, 0},
        {frame(    fib<1>()), 4, 0}
      }
    ),
    mi.command("forwardtrace").front()
  );

  // clang-format on
}

TEST(mdb_forwardtrace, ft_from_step_1_with_limit_0)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<5>::value>");
  mi.command("step");

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame( fib<5>(), _, _, event_kind::template_instantiation), 0, 0}
      }
    ),
    mi.command("forwardtrace 0").front()
  );

  // clang-format on
}

TEST(mdb_forwardtrace, ft_from_step_1_with_limit_1)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<5>::value>");
  mi.command("step");

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame(fib<5>(), _, _, event_kind::template_instantiation), 0, 4},
        {frame( fib<3>(), _, _, event_kind::template_instantiation), 1, 0},
        {frame( fib<3>(), _, _, event_kind::memoization), 1, 0},
        {frame( fib<4>(), _, _, event_kind::template_instantiation), 1, 0},
        {frame( fib<4>(), _, _, event_kind::memoization), 1, 0}
      }
    ),
    mi.command("forwardtrace 1").front()
  );

  // clang-format on
}

TEST(mdb_forwardtrace, ft_from_step_1_with_limit_1_in_full_mode)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate -full int_<fib<5>::value>");
  mi.command("step");

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame(fib<5>()), 0, 2},
        {frame( fib<3>()), 1, 0},
        {frame( fib<4>()), 1, 0}
      }
    ),
    mi.command("forwardtrace 1").front()
  );

  // clang-format on
}

TEST(mdb_forwardtrace, ft_from_step_2_with_limit_2)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<5>::value>");
  mi.command("step");

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame(fib<5>(), _, _, event_kind::template_instantiation), 0, 4},
        {frame( fib<3>(), _, _, event_kind::template_instantiation), 1, 3},
        {frame(  fib<1>(), _, _, event_kind::memoization), 2, 0},
        {frame(  fib<2>(), _, _, event_kind::template_instantiation), 2, 0},
        {frame(  fib<2>(), _, _, event_kind::memoization), 2, 0},
        {frame( fib<3>(), _, _, event_kind::memoization), 1, 0},
        {frame( fib<4>(), _, _, event_kind::template_instantiation), 1, 2},
        {frame(  fib<2>(), _, _, event_kind::memoization), 2, 0},
        {frame(  fib<3>(), _, _, event_kind::memoization), 2, 0},
        {frame( fib<4>(), _, _, event_kind::memoization), 1, 0}
      }
    ),
    mi.command("forwardtrace 2").front()
  );

  // clang-format on
}

TEST(mdb_forwardtrace, ft_from_step_2_with_limit_100)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<5>::value>");
  mi.command("step");

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame(fib<5>(), _, _, event_kind::template_instantiation), 0, 4},
        {frame( fib<3>(), _, _, event_kind::template_instantiation), 1, 3},
        {frame(  fib<1>(), _, _, event_kind::memoization), 2, 0},
        {frame(  fib<2>(), _, _, event_kind::template_instantiation), 2, 2},
        {frame(   fib<0>(), _, _, event_kind::memoization), 3, 0},
        {frame(   fib<1>(), _, _, event_kind::memoization), 3, 0},
        {frame(  fib<2>(), _, _, event_kind::memoization), 2, 0},
        {frame( fib<3>(), _, _, event_kind::memoization), 1, 0},
        {frame( fib<4>(), _, _, event_kind::template_instantiation), 1, 2},
        {frame(  fib<2>(), _, _, event_kind::memoization), 2, 0},
        {frame(  fib<3>(), _, _, event_kind::memoization), 2, 0},
        {frame( fib<4>(), _, _, event_kind::memoization), 1, 0}
      }
    ),
    mi.command("forwardtrace 100").front()
  );

  // clang-format on
}

TEST(mdb_forwardtrace, from_root_on_errored_metaprogram)
{
  metashell_instance mi;
  mi.command(missing_value_fibonacci_mp);
  mi.command("#msh mdb");
  mi.command("evaluate int_<fib<5>::value>");

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame(type("int_<fib<5>::value>")), 0, 1},
        {frame(fib<5>(), _, _, event_kind::template_instantiation), 1, 1},
        {frame( fib<3>(), _, _, event_kind::template_instantiation), 2, 2},
        {frame(  fib<1>(), _, _, event_kind::memoization), 3, 0},
        {frame(  fib<2>(), _, _, event_kind::template_instantiation), 3, 1},
        {frame(   fib<0>(), _, _, event_kind::memoization), 4, 0}
      }
    ),
    mi.command("forwardtrace").front()
  );

  // clang-format on
}

TEST(mdb_forwardtrace, evaluating_environment_fib_2_3)
{
  metashell_instance mi;
  mi.command(fibonacci_mp +
             "int_<fib<5>::value> x;"
             "int_<fib<6>::value> y;");
  mi.command("#msh mdb");
  mi.command("evaluate -");

  // clang-format off

  ASSERT_EQ(
    call_graph(
      {
        {frame(type("<environment>")), 0, 8},
        {frame( fib<5>(), _, _, event_kind::template_instantiation), 1, 4},
        {frame(  fib<3>(), _, _, event_kind::template_instantiation), 2, 3},
        {frame(   fib<1>(), _, _, event_kind::memoization), 3, 0},
        {frame(   fib<2>(), _, _, event_kind::template_instantiation), 3, 2},
        {frame(    fib<0>(), _, _, event_kind::memoization), 4, 0},
        {frame(    fib<1>(), _, _, event_kind::memoization), 4, 0},
        {frame(   fib<2>(), _, _, event_kind::memoization), 3, 0},
        {frame(  fib<3>(), _, _, event_kind::memoization), 2, 0},
        {frame(  fib<4>(), _, _, event_kind::template_instantiation), 2, 2},
        {frame(   fib<2>(), _, _, event_kind::memoization), 3, 0},
        {frame(   fib<3>(), _, _, event_kind::memoization), 3, 0},
        {frame(  fib<4>(), _, _, event_kind::memoization), 2, 0},
        {frame( fib<5>(), _, _, event_kind::memoization), 1, 0},
        {frame( type("int_<5>"), _, _, event_kind::template_instantiation), 1, 0},
        {frame( type("int_<5>"), _, _, event_kind::memoization), 1, 0},
        {frame( fib<6>(), _, _, event_kind::template_instantiation), 1, 2},
        {frame(  fib<4>(), _, _, event_kind::memoization), 2, 0},
        {frame(  fib<5>(), _, _, event_kind::memoization), 2, 0},
        {frame( fib<6>(), _, _, event_kind::memoization), 1, 0},
        {frame( type("int_<8>"), _, _, event_kind::template_instantiation), 1, 0},
        {frame( type("int_<8>"), _, _, event_kind::memoization), 1, 0}
      }
    ),
    mi.command("forwardtrace").front()
  );

  // clang-format on
}

TEST(mdb_forwardtrace, sfinae_v1)
{
  metashell_instance mi;
  mi.command(fibonacci_sfinae_mp);
  mi.command("#msh mdb");
  mi.command("evaluate decltype(foo<4>())");

  // clang-format off
   
  ASSERT_EQ(
    call_graph(
      {
        {frame(type("decltype(foo<4>())")), 0, 5},
        {frame( type("foo"), _, _, event_kind::explicit_template_argument_substitution), 1, 4},
        {frame(  v2::fib<4>(), _, _, event_kind::template_instantiation), 2, 4},
        {frame(   v2::fib<2>(), _, _, event_kind::template_instantiation), 3, 2},
        {frame(    v2::fib<0>(), _, _, event_kind::memoization), 4, 0},
        {frame(    v2::fib<1>(), _, _, event_kind::memoization), 4, 0},
        {frame(   v2::fib<2>(), _, _, event_kind::memoization), 3, 0},
        {frame(   v2::fib<3>(), _, _, event_kind::template_instantiation), 3, 2},
        {frame(    v2::fib<1>(), _, _, event_kind::memoization), 4, 0},
        {frame(    v2::fib<2>(), _, _, event_kind::memoization), 4, 0},
        {frame(   v2::fib<3>(), _, _, event_kind::memoization), 3, 0},
        {frame(  v2::fib<4>(), _, _, event_kind::memoization), 2, 0},
        {frame(  type("enable_if<false, char>"), _, _, event_kind::template_instantiation), 2, 0},
        {frame(  type("enable_if<false, char>"), _, _, event_kind::memoization), 2, 0},
        {frame( type("foo"), _, _, event_kind::explicit_template_argument_substitution), 1, 5},
        {frame(  v1::fib<4>(), _, _, event_kind::template_instantiation), 2, 4},
        {frame(   v1::fib<2>(), _, _, event_kind::template_instantiation), 3, 2},
        {frame(    v1::fib<0>(), _, _, event_kind::memoization), 4, 0},
        {frame(    v1::fib<1>(), _, _, event_kind::memoization), 4, 0},
        {frame(   v1::fib<2>(), _, _, event_kind::memoization), 3, 0},
        {frame(   v1::fib<3>(), _, _, event_kind::template_instantiation), 3, 2},
        {frame(    v1::fib<1>(), _, _, event_kind::memoization), 4, 0},
        {frame(    v1::fib<2>(), _, _, event_kind::memoization), 4, 0},
        {frame(   v1::fib<3>(), _, _, event_kind::memoization), 3, 0},
        {frame(  v1::fib<4>(), _, _, event_kind::memoization), 2, 0},
        {frame(  type("enable_if<true, void>"), _, _, event_kind::template_instantiation), 2, 1},
        {frame(    type("enable_if<true, T>"), _, _, event_kind::deduced_template_argument_substitution), 3, 0},
        {frame(  type("enable_if<true, void>"), _, _, event_kind::memoization), 2, 0},
        {frame(  type("enable_if<true, void>"), _, _, event_kind::memoization), 2, 0},
        {frame( type("foo"), _, _, event_kind::deduced_template_argument_substitution), 1, 0},
        {frame( type("foo<4>"), _, _, event_kind::template_instantiation), 1, 0},
        {frame( type("void"), _, _, event_kind::non_template_type), 1, 0}
      }
    ),
    mi.command("forwardtrace").front()
  );

  // clang-format on
}
