// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/system_test/error.hpp>
#include <metashell/system_test/frame.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/raw_text.hpp>
#include <metashell/system_test/type.hpp>

#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell::system_test;

using pattern::_;

namespace
{
  json_string step(metashell_instance& mi_)
  {
    return mi_.command("step").front();
  }
}

JUST_TEST_CASE(test_mdb_step_without_evaluation)
{
  metashell_instance mi;

  mi.command("#msh mdb");

  JUST_ASSERT_EQUAL(error("Metaprogram not evaluated yet"), step(mi));
}

JUST_TEST_CASE(test_mdb_step_fibonacci)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
}

JUST_TEST_CASE(test_mdb_step_2_fibonacci)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(
      frame(type("fib<8>"), _, _, instantiation_kind::template_instantiation),
      mi.command("step 2").front());
}

JUST_TEST_CASE(test_mdb_step_fibonacci_twice)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      step(mi));

  JUST_ASSERT_EQUAL(
      frame(type("fib<8>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
}

JUST_TEST_CASE(test_mdb_step_fibonacci_twice_with_empty_second_line)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      step(mi));

  JUST_ASSERT_EQUAL(
      frame(type("fib<8>"), _, _, instantiation_kind::template_instantiation),
      mi.command("").front());
}

JUST_TEST_CASE(test_mdb_step_fibonacci_twice_with_space_second_line)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      step(mi));

  // space doesn't repeat
  JUST_ASSERT_EQUAL(prompt("(mdb)"), mi.command(" ").front());
}

JUST_TEST_CASE(test_mdb_step_0_fibonacci_at_start)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  // step 0 doesn't print anything at start
  JUST_ASSERT_EQUAL(prompt("(mdb)"), mi.command("step 0").front());
}

JUST_TEST_CASE(test_mdb_step_0_fibonacci_after_step)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      step(mi));

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      mi.command("step 0").front());
}

JUST_TEST_CASE(test_mdb_step_0_fibonacci_at_end)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int_<55>")), to_json_string(prompt("(mdb)"))},
      mi.command("continue"));

  // step 0 doesn't print anything at end
  JUST_ASSERT_EQUAL(prompt("(mdb)"), mi.command("step 0").front());
}

JUST_TEST_CASE(test_mdb_step_over_the_whole_metaprogram_one_step)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int_<55>")), to_json_string(prompt("(mdb)"))},
      mi.command("step 31"));
}

JUST_TEST_CASE(test_mdb_step_int_non_template_type)
{
  metashell_instance mi;
  mi.command("#msh mdb int");

  JUST_ASSERT_EQUAL(
      frame(type("int"), _, _, instantiation_kind::non_template_type),
      step(mi));

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int")), to_json_string(prompt("(mdb)"))},
      mi.command("step"));
}

JUST_TEST_CASE(test_mdb_step_over_the_whole_metaprogram_multiple_steps)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<8>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<6>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<4>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<2>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<0>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<1>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<2>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<3>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<1>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<2>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<3>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<4>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<3>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<4>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<6>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<7>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<6>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<7>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<8>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<9>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<7>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<8>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<9>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("int_<55>"), _, _, instantiation_kind::template_instantiation),
      step(mi));

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int_<55>")), to_json_string(prompt("(mdb)"))},
      mi.command("step"));
}

JUST_TEST_CASE(test_mdb_step_over_environment_multiple_steps)
{
  metashell_instance mi;

  mi.command(fibonacci_mp +
             "int_<fib<5>::value> x;"
             "int_<fib<6>::value> y;");
  mi.command("#msh mdb -");

  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<3>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<1>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<2>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<0>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<1>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<2>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<3>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<4>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<2>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<3>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<4>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("int_<5>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("int_<5>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<6>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<4>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("fib<6>"), _, _, instantiation_kind::memoization), step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("int_<8>"), _, _, instantiation_kind::template_instantiation),
      step(mi));
  JUST_ASSERT_EQUAL(
      frame(type("int_<8>"), _, _, instantiation_kind::memoization), step(mi));

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), step(mi));
}

JUST_TEST_CASE(
    test_mdb_step_over_the_whole_metaprogram_multiple_steps_in_full_mode)
{
  metashell_instance mi;

  mi.command(fibonacci_mp);
  mi.command("#msh mdb -full int_<fib<4>::value>");

  JUST_ASSERT_EQUAL(frame(type("fib<4>")), step(mi));
  JUST_ASSERT_EQUAL(frame(type("fib<2>")), step(mi));
  JUST_ASSERT_EQUAL(frame(type("fib<0>")), step(mi));
  JUST_ASSERT_EQUAL(frame(type("fib<1>")), step(mi));
  JUST_ASSERT_EQUAL(frame(type("fib<3>")), step(mi));
  JUST_ASSERT_EQUAL(frame(type("fib<1>")), step(mi));
  JUST_ASSERT_EQUAL(frame(type("fib<2>")), step(mi));
  JUST_ASSERT_EQUAL(frame(type("fib<0>")), step(mi));
  JUST_ASSERT_EQUAL(frame(type("fib<1>")), step(mi));
  JUST_ASSERT_EQUAL(frame(type("int_<3>")), step(mi));

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int_<3>")), to_json_string(prompt("(mdb)"))},
      mi.command("step"));
}

JUST_TEST_CASE(test_mdb_step_minus_1_at_start)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"),
                    mi.command("step -1").front());
}

JUST_TEST_CASE(test_mdb_step_minus_1_after_step)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("step 1");

  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"),
                    mi.command("step -1").front());
}

JUST_TEST_CASE(test_mdb_step_minus_1_after_step_in_full_mode)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb -full int_<fib<10>::value>");
  mi.command("step 1");

  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"),
                    mi.command("step -1").front());
}

JUST_TEST_CASE(test_mdb_step_minus_1_after_step_2)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");
  mi.command("step 2");

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      mi.command("step -1").front());
}

JUST_TEST_CASE(test_mdb_step_minus_1_after_step_2_in_full_mode)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb -full int_<fib<10>::value>");
  mi.command("step 2");

  JUST_ASSERT_EQUAL(frame(type("fib<10>")), mi.command("step -1").front());
}

JUST_TEST_CASE(test_mdb_step_over_fib_from_root)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int_<55>")), to_json_string(prompt("(mdb)"))},
      mi.command("step over"));
}

JUST_TEST_CASE(test_mdb_step_over_fib_from_after_step)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      mi.command("step").front());

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::memoization),
      mi.command("step over").front());
}

JUST_TEST_CASE(test_mdb_step_over_minus_1_fib_from_after_step)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<10>::value>");

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      mi.command("step").front());

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::memoization),
      mi.command("step over").front());

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), _, _, instantiation_kind::template_instantiation),
      mi.command("step over -1").front());
}

JUST_TEST_CASE(test_mdb_step_over_minus_1_multi_fib_from_after_step)
{
  metashell_instance mi;
  mi.command(multi_fibonacci_mp);
  mi.command("#msh mdb int_<multi_fib<10>::value>");

  JUST_ASSERT_EQUAL(frame(type("multi_fib<4>"), _, _,
                          instantiation_kind::template_instantiation),
                    mi.command("step 4").front());

  JUST_ASSERT_EQUAL(
      frame(type("multi_fib<4>"), _, _, instantiation_kind::memoization),
      mi.command("step over").front());

  JUST_ASSERT_EQUAL(
      frame(type("multi_fib<3>"), _, _, instantiation_kind::memoization),
      mi.command("step over").front());

  JUST_ASSERT_EQUAL(
      frame(type("multi_fib<4>"), _, _, instantiation_kind::memoization),
      mi.command("step over -1").front());
}

JUST_TEST_CASE(test_mdb_step_out_fib_from_root)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<5>::value>");

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int_<5>")), to_json_string(prompt("(mdb)"))},
      mi.command("step out"));
}

JUST_TEST_CASE(test_mdb_step_out_fib_after_one_step)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<5>::value>");
  mi.command("step");

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int_<5>")), to_json_string(prompt("(mdb)"))},
      mi.command("step out"));
}

JUST_TEST_CASE(test_mdb_step_out_fib_after_two_steps)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<5>::value>");
  mi.command("step 2");

  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), _, _, instantiation_kind::memoization),
      mi.command("step out").front());
}

JUST_TEST_CASE(test_mdb_step_out_fib_after_three_steps)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<5>::value>");
  mi.command("step 3");

  JUST_ASSERT_EQUAL(
      frame(type("fib<3>"), _, _, instantiation_kind::memoization),
      mi.command("step out").front());
}

JUST_TEST_CASE(test_mdb_step_out_fib_twice_after_five_steps)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<5>::value>");
  mi.command("step 5");

  JUST_ASSERT_EQUAL(
      frame(type("fib<3>"), _, _, instantiation_kind::memoization),
      mi.command("step out 2").front());
}

JUST_TEST_CASE(test_mdb_step_out_fib_four_times_after_five_steps)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<5>::value>");
  mi.command("step 5");

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int_<5>")), to_json_string(prompt("(mdb)"))},
      mi.command("step out 4"));
}

JUST_TEST_CASE(test_mdb_step_out_fib_four_after_five_steps)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<5>::value>");
  mi.command("step 5");

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int_<5>")), to_json_string(prompt("(mdb)"))},
      mi.command("step out 4"));
}

JUST_TEST_CASE(test_mdb_step_out_minus_1_at_root_of_fib)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<5>::value>");

  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"),
                    mi.command("step out -1").front());
}

JUST_TEST_CASE(test_mdb_step_out_minus_1_after_step_4_in_fib)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<5>::value>");
  mi.command("step 4");

  JUST_ASSERT_EQUAL(
      frame(type("fib<3>"), _, _, instantiation_kind::template_instantiation),
      mi.command("step out -1").front());
}

JUST_TEST_CASE(test_mdb_step_over_template_spec_no_deduced_event)
{
  metashell_instance mi;
  mi.command(template_specialization_mp);
  mi.command("#msh mdb int_<foo<3, 1>::value>");

  JUST_ASSERT_EQUAL(
      frame(type("foo<N, 1>"), _, _,
            instantiation_kind::deduced_template_argument_substitution),
      step(mi));

  JUST_ASSERT_EQUAL(frame(type("foo<3, 1>"), _, _,
                          instantiation_kind::template_instantiation),
                    step(mi));

  JUST_ASSERT_EQUAL(
      frame(type("foo<3, 1>"), _, _, instantiation_kind::memoization),
      step(mi));

  JUST_ASSERT_EQUAL(
      frame(type("int_<45>"), _, _, instantiation_kind::template_instantiation),
      step(mi));

  JUST_ASSERT_EQUAL_CONTAINER(
      {to_json_string(raw_text("Metaprogram finished")),
       to_json_string(type("int_<45>")), to_json_string(prompt("(mdb)"))},
      mi.command("step"));
}

JUST_TEST_CASE(test_mdb_step_garbage_argument)
{
  metashell_instance mi;
  mi.command(fibonacci_mp);
  mi.command("#msh mdb int_<fib<2>::value>");

  JUST_ASSERT_EQUAL(
      error("Argument parsing failed"), mi.command("step asd").front());
}

JUST_TEST_CASE(test_mdb_step_cant_step_forward_when_metaprogram_errored)
{
  metashell_instance mi;
  mi.command(missing_value_fibonacci_mp);
  mi.command("#msh mdb int_<fib<5>::value>");

  const std::vector<json_string> r_continue = mi.command("continue");

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), r_continue[0]);
  JUST_ASSERT_EQUAL(error(_), r_continue[1]);

  const std::vector<json_string> r_step = mi.command("step");

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), r_step[0]);
  JUST_ASSERT_EQUAL(error(_), r_step[1]);
}
