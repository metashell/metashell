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

#include <metashell/in_memory_displayer.hpp>

#include "mdb_test_shell.hpp"

#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell;

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_without_evaluation) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("step", d);

  JUST_ASSERT_EQUAL_CONTAINER(d.errors(), {"Metaprogram not evaluated yet"});
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_fibonacci) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("step", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<10>"), data::instantiation_kind::template_instantiation)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_2_fibonacci) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("step 2", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<8>"), data::instantiation_kind::template_instantiation)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_fibonacci_twice) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("step", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<10>"), data::instantiation_kind::template_instantiation)},
    d.frames()
  );

  d.clear();
  sh.line_available("step", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<8>"), data::instantiation_kind::template_instantiation)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_fibonacci_twice_with_empty_second_line) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("step", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<10>"), data::instantiation_kind::template_instantiation)},
    d.frames()
  );

  d.clear();
  sh.line_available("", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<8>"), data::instantiation_kind::template_instantiation)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_fibonacci_twice_with_space_second_line) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("step", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<10>"), data::instantiation_kind::template_instantiation)},
    d.frames()
  );

  d.clear();
  sh.line_available(" ", d);

  JUST_ASSERT(d.empty());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_0_fibonacci_at_start) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("step 0", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_0_fibonacci_after_step) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("step", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<10>"), data::instantiation_kind::template_instantiation)},
    d.frames()
  );

  d.clear();
  sh.line_available("step 0", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<10>"), data::instantiation_kind::template_instantiation)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_over_the_whole_metaprogram_one_step) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("step 31", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int_<55>")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_int_non_template_type) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int", d);

  d.clear();
  sh.line_available("step", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("int"), data::instantiation_kind::non_template_type)},
    d.frames()
  );

  d.clear();
  sh.line_available("step", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_over_the_whole_metaprogram_multiple_steps) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);
  d.clear();

  for (unsigned i = 0; i < 22; ++i) {
    sh.line_available("step", d);
  }

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      frame(type("fib<10>"), data::instantiation_kind::template_instantiation),
      frame(type("fib<8>"), data::instantiation_kind::template_instantiation),
      frame(type("fib<6>"), data::instantiation_kind::template_instantiation),
      frame(type("fib<4>"), data::instantiation_kind::template_instantiation),
      frame(type("fib<2>"), data::instantiation_kind::template_instantiation),
      frame(type("fib<0>"), data::instantiation_kind::memoization),
      frame(type("fib<1>"), data::instantiation_kind::memoization),
      frame(type("fib<3>"), data::instantiation_kind::template_instantiation),
      frame(type("fib<1>"), data::instantiation_kind::memoization),
      frame(type("fib<2>"), data::instantiation_kind::memoization),
      frame(type("fib<5>"), data::instantiation_kind::template_instantiation),
      frame(type("fib<3>"), data::instantiation_kind::memoization),
      frame(type("fib<4>"), data::instantiation_kind::memoization),
      frame(type("fib<7>"), data::instantiation_kind::template_instantiation),
      frame(type("fib<5>"), data::instantiation_kind::memoization),
      frame(type("fib<6>"), data::instantiation_kind::memoization),
      frame(type("fib<9>"), data::instantiation_kind::template_instantiation),
      frame(type("fib<7>"), data::instantiation_kind::memoization),
      frame(type("fib<8>"), data::instantiation_kind::memoization),
      frame(type("fib<10>"), data::instantiation_kind::memoization),
      frame(type("int_<55>"), data::instantiation_kind::template_instantiation)
    },
    d.frames()
  );

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int_<55>")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(
    test_mdb_step_over_the_whole_metaprogram_multiple_steps_in_full_mode)
{
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate -full int_<fib<4>::value>", d);
  d.clear();

  for (unsigned i = 0; i < 11; ++i) {
    sh.line_available("step", d);
  }

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      frame(type("fib<4>")),
      frame(type("fib<2>")),
      frame(type("fib<0>")),
      frame(type("fib<1>")),
      frame(type("fib<3>")),
      frame(type("fib<1>")),
      frame(type("fib<2>")),
      frame(type("fib<0>")),
      frame(type("fib<1>")),
      frame(type("int_<3>"))
    },
    d.frames()
  );

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int_<3>")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_minus_1_at_start) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("step -1", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Metaprogram reached the beginning"},
    d.raw_texts()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_minus_1_after_step) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);
  sh.line_available("step 1", d);

  d.clear();
  sh.line_available("step -1", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Metaprogram reached the beginning"},
    d.raw_texts()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_minus_1_after_step_in_full_mode) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate -full int_<fib<10>::value>", d);
  sh.line_available("step 1", d);

  d.clear();
  sh.line_available("step -1", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Metaprogram reached the beginning"},
    d.raw_texts()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_0_at_start) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("step 0", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_0_at_end) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("continue", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int_<55>")}, d.types());

  d.clear();
  sh.line_available("step 0", d);

  JUST_ASSERT_EMPTY_CONTAINER(d.raw_texts());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_minus_1_after_step_2) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);
  sh.line_available("step 2", d);

  d.clear();
  sh.line_available("step -1", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<10>"), data::instantiation_kind::template_instantiation)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_minus_1_after_step_2_in_full_mode) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);
  sh.line_available("step 2", d);

  d.clear();
  sh.line_available("step -1", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<10>"), data::instantiation_kind::template_instantiation)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_over_fib_from_root) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("step over", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int_<55>")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_over_fib_from_after_step) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("step", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<10>"), data::instantiation_kind::template_instantiation)},
    d.frames()
  );

  d.clear();
  sh.line_available("step over", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<10>"), data::instantiation_kind::memoization)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_over_minus_1_fib_from_after_step) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("step", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<10>"), data::instantiation_kind::template_instantiation)},
    d.frames()
  );

  d.clear();
  sh.line_available("step over", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<10>"), data::instantiation_kind::memoization)},
    d.frames()
  );

  d.clear();
  sh.line_available("step over -1", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<10>"), data::instantiation_kind::template_instantiation)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_over_minus_1_multi_fib_from_after_step) {
  in_memory_displayer d;
  mdb_test_shell sh(multi_fibonacci_mp);

  sh.line_available("evaluate int_<multi_fib<10>::value>", d);

  d.clear();
  sh.line_available("step 4", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      frame(type("multi_fib<4>"),
      data::instantiation_kind::template_instantiation)
    },
    d.frames()
  );

  d.clear();
  sh.line_available("step over", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      frame(
        type("multi_fib<5>"),
        data::instantiation_kind::template_instantiation
      )
    },
    d.frames()
  );

  d.clear();
  sh.line_available("step over", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      frame(
        type("multi_fib<8>"),
        data::instantiation_kind::template_instantiation
      )
    },
    d.frames()
  );

  d.clear();
  sh.line_available("step over -1", d);

  // step over -1 is not always the inverse of step over
  JUST_ASSERT_EQUAL_CONTAINER(
    {
      frame(
        type("multi_fib<6>"),
        data::instantiation_kind::template_instantiation
      )
    },
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_out_fib_from_root) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);

  d.clear();
  sh.line_available("step out", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int_<5>")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_out_fib_after_one_step) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);
  sh.line_available("step", d);

  d.clear();
  sh.line_available("step out", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int_<5>")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_out_fib_after_two_steps) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);
  sh.line_available("step 2", d);

  d.clear();
  sh.line_available("step out", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<5>"), data::instantiation_kind::memoization)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_out_fib_after_three_steps) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);
  sh.line_available("step 3", d);

  d.clear();
  sh.line_available("step out", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<4>"), data::instantiation_kind::template_instantiation)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_out_fib_twice_after_five_steps) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);
  sh.line_available("step 5", d);

  d.clear();
  sh.line_available("step out 2", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<5>"), data::instantiation_kind::memoization)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_out_fib_three_after_five_steps) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);
  sh.line_available("step 5", d);

  d.clear();
  sh.line_available("step out 3", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int_<5>")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_out_fib_four_after_five_steps) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);
  sh.line_available("step 5", d);

  d.clear();
  sh.line_available("step out 4", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int_<5>")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_out_minus_1_at_root_of_fib) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);

  d.clear();
  sh.line_available("step out -1", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {"Metaprogram reached the beginning"},
    d.raw_texts()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_out_minus_1_after_step_4_in_fib) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);
  sh.line_available("step 4", d);

  d.clear();
  sh.line_available("step out -1", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {frame(type("fib<3>"), data::instantiation_kind::template_instantiation)},
    d.frames()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_over_template_spec_no_deduced_event) {
  in_memory_displayer d;
  mdb_test_shell sh(template_specialization_mp);

  sh.line_available("evaluate int_<foo<3, 1>::value>", d);
  d.clear();

  for (unsigned i = 0; i < 4; ++i) {
    sh.line_available("step", d);
  }

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      frame(
        type("foo<3, 1>"),
        data::instantiation_kind::template_instantiation
      ),
      frame(type("foo<3, 1>"), data::instantiation_kind::memoization),
      frame(type("int_<45>"), data::instantiation_kind::template_instantiation)
    },
    d.frames()
  );

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({type("int_<45>")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_step_garbage_argument) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("step asd", d);

  JUST_ASSERT_EQUAL_CONTAINER(d.errors(), {"Argument parsing failed"});
}
#endif

