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
using namespace metashell::data;

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_backtrace_without_evaluation) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("backtrace", d);

  JUST_ASSERT_EQUAL_CONTAINER(d.errors(), {"Metaprogram not evaluated yet"});
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_backtrace_unstepped_fibonacci) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("backtrace", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    { data::backtrace{data::frame(data::type("int_<fib<10>::value>"))} },
    d.backtraces()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_backtrace_when_metaprogram_finished) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("evaluate int", d);
  sh.line_available("continue", d);

  d.clear();
  sh.line_available("backtrace", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({data::type("int")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_backtrace_when_metaprogram_finished_in_full_mode) {
  in_memory_displayer d;
  mdb_test_shell sh;

  sh.line_available("evaluate -full int", d);
  sh.line_available("continue", d);

  d.clear();
  sh.line_available("backtrace", d);

  JUST_ASSERT_EQUAL_CONTAINER({"Metaprogram finished"}, d.raw_texts());
  JUST_ASSERT_EQUAL_CONTAINER({data::type("int")}, d.types());
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_backtrace_1_stepped_fibonacci) {
  using data::instantiation_kind;
  using data::type;
  using data::frame;
  using data::backtrace;

  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);
  sh.line_available("step", d);

  d.clear();
  sh.line_available("backtrace", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      backtrace{
        frame(type("fib<10>"), file_location(), instantiation_kind::template_instantiation),
        frame(type("int_<fib<10>::value>"))
      }
    },
    d.backtraces()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_backtrace_2_stepped_fibonacci) {
  using data::instantiation_kind;
  using data::type;
  using data::frame;
  using data::backtrace;

  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);
  sh.line_available("step 2", d);

  d.clear();
  sh.line_available("backtrace", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      backtrace{
        frame(type("fib<8>"), file_location(), instantiation_kind::template_instantiation),
        frame(type("fib<10>"), file_location(), instantiation_kind::template_instantiation),
        frame(type("int_<fib<10>::value>"))
      }
    },
    d.backtraces()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_backtrace_3_stepped_fibonacci) {
  using data::instantiation_kind;
  using data::type;
  using data::frame;
  using data::backtrace;

  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);
  sh.line_available("step 3", d);

  d.clear();
  sh.line_available("backtrace", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      backtrace{
        frame(type("fib<6>"), file_location(), instantiation_kind::template_instantiation),
        frame(type("fib<8>"), file_location(), instantiation_kind::template_instantiation),
        frame(type("fib<10>"), file_location(), instantiation_kind::template_instantiation),
        frame(type("int_<fib<10>::value>"))
      }
    },
    d.backtraces()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_backtrace_garbage_argument) {
  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);

  d.clear();
  sh.line_available("backtrace asd", d);

  JUST_ASSERT_EQUAL_CONTAINER(d.errors(),
      {"This command doesn't accept arguments"});
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
JUST_TEST_CASE(test_mdb_backtrace_bt_alias) {
  using data::instantiation_kind;
  using data::type;
  using data::frame;
  using data::backtrace;

  in_memory_displayer d;
  mdb_test_shell sh(fibonacci_mp);

  sh.line_available("evaluate int_<fib<10>::value>", d);
  sh.line_available("step", d);

  d.clear();
  sh.line_available("backtrace", d);
  JUST_ASSERT_EQUAL_CONTAINER(
    {
      backtrace{
        frame(type("fib<10>"), file_location(), instantiation_kind::template_instantiation),
        frame(type("int_<fib<10>::value>"))
      }
    },
    d.backtraces()
  );

  d.clear();
  sh.line_available("bt", d);
  JUST_ASSERT_EQUAL_CONTAINER(
    {
      backtrace{
        frame(type("fib<10>"), file_location(), instantiation_kind::template_instantiation),
        frame(type("int_<fib<10>::value>"))
      }
    },
    d.backtraces()
  );
}
#endif

#ifndef METASHELL_DISABLE_TEMPLIGHT_TESTS
  JUST_TEST_CASE(test_mdb_backtrace_on_error) {
  using data::instantiation_kind;
  using data::type;
  using data::frame;
  using data::backtrace;

  in_memory_displayer d;
  mdb_test_shell sh(missing_value_fibonacci_mp);

  sh.line_available("evaluate int_<fib<5>::value>", d);
  sh.line_available("continue", d);

  d.clear();
  sh.line_available("backtrace", d);

  JUST_ASSERT_EQUAL_CONTAINER(
    {
      backtrace{
        frame(type("fib<0>"), file_location(), instantiation_kind::memoization),
        frame(type("fib<2>"), file_location(), instantiation_kind::template_instantiation),
        frame(type("fib<3>"), file_location(), instantiation_kind::template_instantiation),
        frame(type("fib<5>"), file_location(), instantiation_kind::template_instantiation),
        frame(type("int_<fib<5>::value>"))
      }
    },
    d.backtraces()
  );
}
#endif
