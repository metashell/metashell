// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "util.hpp"

#include <boost/test/unit_test.hpp>

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


