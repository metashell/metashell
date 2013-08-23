// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "test_shell.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_non_existing_class)
{
  test_shell sh;
  sh.line_available("hello");
  BOOST_CHECK_EQUAL("", sh.info());
  BOOST_CHECK_EQUAL("", sh.output());
  BOOST_CHECK(!sh.error().empty());
}

