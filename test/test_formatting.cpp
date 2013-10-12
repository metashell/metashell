// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include "util.hpp"
#include "test_shell.hpp"

#include <boost/test/unit_test.hpp>

using namespace metashell;

BOOST_AUTO_TEST_CASE(test_basic_formatting)
{
  BOOST_CHECK_EQUAL(
    "double",
    get_output(
      "int",
      "namespace metashell"
      "{"
        "template <>"
        "struct format<int>"
        "{"
          "typedef double type;"
        "};"
      "}"
    )
  );
}

BOOST_AUTO_TEST_CASE(test_tag_dispatched_formatting)
{
  BOOST_CHECK_EQUAL(
    "char",
    get_output(
      "foo",
      "struct foo_tag {};"

      "struct foo"
      "{"
        "typedef foo_tag tag;"
      "};"

      "namespace metashell"
      "{"
        "template <>"
        "struct format_impl<foo_tag>"
        "{"
          "typedef format_impl type;"

          "template <class T>"
          "struct apply"
          "{"
            "typedef char type;"
          "};"
        "};"
      "}"
    )
  );
}

BOOST_AUTO_TEST_CASE(test_formatting_disabled)
{
  config cfg = config::empty;
  cfg.indent = false;

  test_shell sh(cfg, 10);
  sh.line_available(
    "template <class... Ts> struct template_with_a_long_name {};"
  );
  sh.line_available("template_with_a_long_name<int, double, char>");

  BOOST_CHECK_EQUAL(
    "template_with_a_long_name<int, double, char>",
    sh.output()
  );
}


