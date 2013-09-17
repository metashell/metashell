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


