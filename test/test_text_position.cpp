// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <metashell/text_position.hpp>

#include <boost/test/unit_test.hpp>

using namespace metashell;

BOOST_AUTO_TEST_CASE(test_text_position)
{
  const text_position s;

  BOOST_CHECK_EQUAL(text_position(1, 1), s);
  BOOST_CHECK_EQUAL(text_position(1, 2), s + "x");
  BOOST_CHECK_EQUAL(text_position(2, 1), s + "x\n");
  BOOST_CHECK_EQUAL(text_position(2, 1), s + "x\r");
  BOOST_CHECK_EQUAL(text_position(2, 1), s + "x\r\n");
  BOOST_CHECK_EQUAL(text_position(3, 1), s + "x\n\r");
  BOOST_CHECK_EQUAL(text_position(3, 1), s + "x\r\r");
  BOOST_CHECK_EQUAL(text_position(3, 1), s + "x\n\n");
  BOOST_CHECK_EQUAL(text_position(3, 1), s + "x\r\n\n");
  BOOST_CHECK_EQUAL(text_position(3, 1), s + "x\r\n\r");
}


