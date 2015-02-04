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

#include <metashell/in_memory_displayer.hpp>

#include "mdb_test_shell.hpp"

#include "test_metaprograms.hpp"

#include <just/test.hpp>

using namespace metashell;

//Few tests taken directly from test_mdb_step.cpp
#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_next_fib_from_root) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("next", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({data::type("int_<55>")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_next_minus_1_multi_fib_from_after_step) {
  in_memory_displayer d;
  mdb_test_shell sh(multi_fibonacci_mp);

  sh.line_available("evaluate int_<multi_fib<10>::value>", d);

  d.clear();
  sh.line_available("step 4", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      data::frame(data::type("multi_fib<4>"),
      data::instantiation_kind::template_instantiation)
    },
    d.frames()
  );

  d.clear();
  sh.line_available("next", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      data::frame(
        data::type("multi_fib<5>"),
        data::instantiation_kind::template_instantiation
      )
    },
    d.frames()
  );

  d.clear();
  sh.line_available("next", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      data::frame(
        data::type("multi_fib<8>"),
        data::instantiation_kind::template_instantiation
      )
    },
    d.frames()
  );

  d.clear();
  sh.line_available("next -1", d);

  // step over -1 is not always the inverse of step over
  JUST_ASSERT_EQUAL_CONTAINER(
    {
      data::frame(
        data::type("multi_fib<6>"),
        data::instantiation_kind::template_instantiation
      )
    },
    d.frames()
  );
}
#endif
