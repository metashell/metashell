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

#include <metashell_system_test/type.hpp>
#include <metashell_system_test/error.hpp>
#include <metashell_system_test/frame.hpp>
#include <metashell_system_test/prompt.hpp>
#include <metashell_system_test/raw_text.hpp>
#include <metashell_system_test/run_metashell.hpp>
#include <metashell_system_test/json_generator.hpp>

#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell_system_test;

JUST_TEST_CASE(test_mdb_step_without_evaluation) {
  const auto r =
    run_metashell(
      {
        command("#msh mdb"),
        command("step")
      }
    );

  auto i = r.begin() + 2;

  JUST_ASSERT_EQUAL(error("Metaprogram not evaluated yet"), *i);
}

JUST_TEST_CASE(test_mdb_step_fibonacci) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<10>::value>"),
        command("step")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), instantiation_kind::template_instantiation),
      *i);
}

JUST_TEST_CASE(test_mdb_step_2_fibonacci) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<10>::value>"),
        command("step 2")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
      frame(type("fib<8>"), instantiation_kind::template_instantiation),
      *i);
}

JUST_TEST_CASE(test_mdb_step_fibonacci_twice) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<10>::value>"),
        command("step"),
        command("step")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), instantiation_kind::template_instantiation),
      *i);

  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<8>"), instantiation_kind::template_instantiation),
      *i);
}

JUST_TEST_CASE(test_mdb_step_fibonacci_twice_with_empty_second_line) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<10>::value>"),
        command("step"),
        command("")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), instantiation_kind::template_instantiation),
      *i++);

  ++i;
  JUST_ASSERT_EQUAL(
      frame(type("fib<8>"), instantiation_kind::template_instantiation),
      *i);
}

JUST_TEST_CASE(test_mdb_step_fibonacci_twice_with_space_second_line) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<10>::value>"),
        command("step"),
        command(" ")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), instantiation_kind::template_instantiation),
      *i++);

  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i++);
  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i); // space doesn't repeat
}

JUST_TEST_CASE(test_mdb_step_0_fibonacci_at_start) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<10>::value>"),
        command("step 0")
      }
    );

  auto i = r.begin() + 4;

  // step 0 doesn't print anything at start
  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
}

JUST_TEST_CASE(test_mdb_step_0_fibonacci_after_step) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<10>::value>"),
        command("step"),
        command("step 0")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), instantiation_kind::template_instantiation),
      *i);

  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), instantiation_kind::template_instantiation),
      *i++);
}

JUST_TEST_CASE(test_mdb_step_0_fibonacci_at_end) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<10>::value>"),
        command("continue"),
        command("step 0")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int_<55>"), *i);

  // step 0 doesn't print anything at end
  i += 2;
  JUST_ASSERT_EQUAL(prompt("(mdb)"), *i);
}


JUST_TEST_CASE(test_mdb_step_over_the_whole_metaprogram_one_step) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<10>::value>"),
        command("step 31")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int_<55>"), *i++);
}

JUST_TEST_CASE(test_mdb_step_int_non_template_type) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int"),
        command("step"),
        command("step")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
      frame(type("int"), instantiation_kind::non_template_type),
      *i);

  i +=2 ;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int"), *i++);
}

JUST_TEST_CASE(test_mdb_step_over_the_whole_metaprogram_multiple_steps) {
  std::vector<json_string> commands = {
    command(fibonacci_mp),
    command("#msh mdb int_<fib<10>::value>")
  };

  for (auto i = 0; i < 22; ++i) {
    commands.push_back(command("step"));
  }

  const auto r = run_metashell(commands);

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), instantiation_kind::template_instantiation), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<8>"), instantiation_kind::template_instantiation), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<6>"), instantiation_kind::template_instantiation), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<4>"), instantiation_kind::template_instantiation), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<2>"), instantiation_kind::template_instantiation), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<0>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<1>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<3>"), instantiation_kind::template_instantiation), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<1>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<2>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), instantiation_kind::template_instantiation), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<3>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<4>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<7>"), instantiation_kind::template_instantiation), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<6>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<9>"), instantiation_kind::template_instantiation), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<7>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<8>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("int_<55>"), instantiation_kind::template_instantiation), *i);
  i += 2;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int_<55>"), *i++);
}

JUST_TEST_CASE(test_mdb_step_over_environment_multiple_steps) {
  std::vector<json_string> commands = {
    command(
      fibonacci_mp +
      "int_<fib<5>::value> x;"
      "int_<fib<6>::value> y;"
    ),
    command("#msh mdb -")
  };
  for (auto i = 0; i < 19; ++i) {
    commands.push_back(command("step"));
  }

  const auto r = run_metashell(commands);

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), instantiation_kind::template_instantiation), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<3>"), instantiation_kind::template_instantiation), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<1>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<2>"), instantiation_kind::template_instantiation), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<0>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<1>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<4>"), instantiation_kind::template_instantiation), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<2>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<3>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("int_<5>"), instantiation_kind::template_instantiation), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("int_<5>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<6>"), instantiation_kind::template_instantiation), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<4>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<6>"), instantiation_kind::memoization), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("int_<8>"), instantiation_kind::template_instantiation), *i);
  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("int_<8>"), instantiation_kind::memoization), *i);
  i += 2;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
}

JUST_TEST_CASE(
    test_mdb_step_over_the_whole_metaprogram_multiple_steps_in_full_mode)
{
  std::vector<json_string> commands = {
    command(fibonacci_mp),
    command("#msh mdb -full int_<fib<4>::value>")
  };

  for (auto i = 0; i < 11; ++i) {
    commands.push_back(command("step"));
  }

  const auto r = run_metashell(commands);

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(frame(type("fib<4>")), *i); i += 2;
  JUST_ASSERT_EQUAL(frame(type("fib<2>")), *i); i += 2;
  JUST_ASSERT_EQUAL(frame(type("fib<0>")), *i); i += 2;
  JUST_ASSERT_EQUAL(frame(type("fib<1>")), *i); i += 2;
  JUST_ASSERT_EQUAL(frame(type("fib<3>")), *i); i += 2;
  JUST_ASSERT_EQUAL(frame(type("fib<1>")), *i); i += 2;
  JUST_ASSERT_EQUAL(frame(type("fib<2>")), *i); i += 2;
  JUST_ASSERT_EQUAL(frame(type("fib<0>")), *i); i += 2;
  JUST_ASSERT_EQUAL(frame(type("fib<1>")), *i); i += 2;
  JUST_ASSERT_EQUAL(frame(type("int_<3>")), *i); i += 2;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int_<3>"), *i++);
}

JUST_TEST_CASE(test_mdb_step_minus_1_at_start) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<10>::value>"),
        command("step -1")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"), *i);
}

JUST_TEST_CASE(test_mdb_step_minus_1_after_step) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<10>::value>"),
        command("step 1"),
        command("step -1")
      }
    );

  auto i = r.begin() + 4;

  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"), *i);
}

JUST_TEST_CASE(test_mdb_step_minus_1_after_step_in_full_mode) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb -full int_<fib<10>::value>"),
        command("step 1"),
        command("step -1")
      }
    );

  auto i = r.begin() + 4;

  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"), *i);
}

JUST_TEST_CASE(test_mdb_step_minus_1_after_step_2) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<10>::value>"),
        command("step 2"),
        command("step -1")
      }
    );

  auto i = r.begin() + 4;

  i += 2;

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), instantiation_kind::template_instantiation), *i);
}

JUST_TEST_CASE(test_mdb_step_minus_1_after_step_2_in_full_mode) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb -full int_<fib<10>::value>"),
        command("step 2"),
        command("step -1")
      }
    );

  auto i = r.begin() + 4;

  i += 2;
  JUST_ASSERT_EQUAL(frame(type("fib<10>")), *i);
}

JUST_TEST_CASE(test_mdb_step_over_fib_from_root) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<10>::value>"),
        command("step over")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int_<55>"), *i++);
}

JUST_TEST_CASE(test_mdb_step_over_fib_from_after_step) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<10>::value>"),
        command("step"),
        command("step over")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), instantiation_kind::template_instantiation),
      *i);

  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), instantiation_kind::memoization),
      *i);
}

JUST_TEST_CASE(test_mdb_step_over_minus_1_fib_from_after_step) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<10>::value>"),
        command("step"),
        command("step over"),
        command("step over -1")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), instantiation_kind::template_instantiation),
      *i);

  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), instantiation_kind::memoization),
      *i);

  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<10>"), instantiation_kind::template_instantiation),
      *i);

}

JUST_TEST_CASE(test_mdb_step_over_minus_1_multi_fib_from_after_step) {
  const auto r =
    run_metashell(
      {
        command(multi_fibonacci_mp),
        command("#msh mdb int_<multi_fib<10>::value>"),
        command("step 4"),
        command("step over"),
        command("step over"),
        command("step over -1")
      }
    );

  auto i = r.begin() + 4;
  JUST_ASSERT_EQUAL(
      frame(type("multi_fib<4>"), instantiation_kind::template_instantiation),
      *i);

  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("multi_fib<5>"), instantiation_kind::template_instantiation),
      *i);

  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("multi_fib<8>"), instantiation_kind::template_instantiation),
      *i);

  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("multi_fib<6>"), instantiation_kind::template_instantiation),
      *i);
}

JUST_TEST_CASE(test_mdb_step_out_fib_from_root) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<5>::value>"),
        command("step out")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int_<5>"), *i);
}

JUST_TEST_CASE(test_mdb_step_out_fib_after_one_step) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<5>::value>"),
        command("step"),
        command("step out")
      }
    );

  auto i = r.begin() + 4;

  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int_<5>"), *i);
}

JUST_TEST_CASE(test_mdb_step_out_fib_after_two_steps) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<5>::value>"),
        command("step 2"),
        command("step out")
      }
    );

  auto i = r.begin() + 4;

  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), instantiation_kind::memoization), *i);
}

JUST_TEST_CASE(test_mdb_step_out_fib_after_three_steps) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<5>::value>"),
        command("step 3"),
        command("step out")
      }
    );

  auto i = r.begin() + 4;

  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<4>"), instantiation_kind::template_instantiation), *i);
}

JUST_TEST_CASE(test_mdb_step_out_fib_twice_after_five_steps) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<5>::value>"),
        command("step 5"),
        command("step out 2")
      }
    );

  auto i = r.begin() + 4;

  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<5>"), instantiation_kind::memoization), *i);
}

JUST_TEST_CASE(test_mdb_step_out_fib_three_after_five_steps) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<5>::value>"),
        command("step 5"),
        command("step out 3")
      }
    );

  auto i = r.begin() + 4;

  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int_<5>"), *i);
}

JUST_TEST_CASE(test_mdb_step_out_fib_four_after_five_steps) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<5>::value>"),
        command("step 5"),
        command("step out 4")
      }
    );

  auto i = r.begin() + 4;

  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int_<5>"), *i);
}

JUST_TEST_CASE(test_mdb_step_out_minus_1_at_root_of_fib) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<5>::value>"),
        command("step out -1")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram reached the beginning"), *i);
}

JUST_TEST_CASE(test_mdb_step_out_minus_1_after_step_4_in_fib) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<5>::value>"),
        command("step 4"),
        command("step out -1")
      }
    );

  auto i = r.begin() + 4;

  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("fib<3>"), instantiation_kind::template_instantiation), *i);
}

JUST_TEST_CASE(test_mdb_step_over_template_spec_no_deduced_event) {
  const auto r =
    run_metashell(
      {
        command(template_specialization_mp),
        command("#msh mdb int_<foo<3, 1>::value>"),
        command("step"),
        command("step"),
        command("step"),
        command("step")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(
      frame(type("foo<3, 1>"), instantiation_kind::template_instantiation), *i);

  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("foo<3, 1>"), instantiation_kind::memoization), *i);

  i += 2;
  JUST_ASSERT_EQUAL(
      frame(type("int_<45>"), instantiation_kind::template_instantiation), *i);

  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(type("int_<45>"), *i);
}

JUST_TEST_CASE(test_mdb_step_garbage_argument) {
  const auto r =
    run_metashell(
      {
        command(fibonacci_mp),
        command("#msh mdb int_<fib<2>::value>"),
        command("step asd")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(error("Argument parsing failed"), *i);
}

JUST_TEST_CASE(test_mdb_step_cant_step_forward_when_metaprogram_errored) {
  const auto r =
    run_metashell(
      {
        command(missing_value_fibonacci_mp),
        command("#msh mdb int_<fib<5>::value>"),
        command("continue"),
        command("step")
      }
    );

  auto i = r.begin() + 4;

  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(error(_), *i);

  i += 2;
  JUST_ASSERT_EQUAL(raw_text("Metaprogram finished"), *i++);
  JUST_ASSERT_EQUAL(error(_), *i);
}
