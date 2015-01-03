// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/indenter.hpp>

#include <just/test.hpp>

using namespace metashell;

JUST_TEST_CASE(test_empty_indenter_returns_empty_string)
{
  indenter ind(10);
  JUST_ASSERT_EQUAL("", ind.str());
}

JUST_TEST_CASE(test_displaying_raw_text)
{
  indenter ind(10);
  ind.raw("foo");
  JUST_ASSERT_EQUAL("foo\n", ind.str());
}

JUST_TEST_CASE(test_displaying_raw_text_that_is_longer_than_width)
{
  indenter ind(2);
  ind.raw("foo");
  JUST_ASSERT_EQUAL("foo\n", ind.str());
}

JUST_TEST_CASE(test_left_aligning_one_liner)
{
  indenter ind(10);
  ind.left_align("foo", "-", ".");
  JUST_ASSERT_EQUAL(".foo\n", ind.str());
}

JUST_TEST_CASE(test_left_aligning_two_liner)
{
  indenter ind(10);
  ind.left_align("foo foo foo", "-", ".");
  JUST_ASSERT_EQUAL(".foo foo\n-foo\n", ind.str());
}

JUST_TEST_CASE(test_removing_extra_whitespace_at_line_break)
{
  indenter ind(10);
  ind.left_align("foo foo                  foo", "-", ".");
  JUST_ASSERT_EQUAL(".foo foo\n-foo\n", ind.str());
}

JUST_TEST_CASE(test_left_aligning_two_liner_word)
{
  indenter ind(10);
  ind.left_align("foofoofoofoo", "-", ".");
  JUST_ASSERT_EQUAL(".foofoofoo\n-foo\n", ind.str());
}

JUST_TEST_CASE(test_left_aligning_three_liner_word)
{
  indenter ind(10);
  ind.left_align("foofoofoobarbarbarxyz", "-", ".");
  JUST_ASSERT_EQUAL(".foofoofoo\n-barbarbar\n-xyz\n", ind.str());
}

JUST_TEST_CASE(test_left_aligning_with_new_line)
{
  indenter ind(10);
  ind.left_align("foo\nbar xyz", "-", ".");
  JUST_ASSERT_EQUAL(".foo\n-bar xyz\n", ind.str());
}

JUST_TEST_CASE(test_left_aligning_text_shorter_than_the_width_with_new_line)
{
  indenter ind(10);
  ind.left_align("foo\nbar", "-", ".");
  JUST_ASSERT_EQUAL(".foo\n-bar\n", ind.str());
}

JUST_TEST_CASE(test_left_aligning_text_starting_with_whitespace)
{
  indenter ind(10);
  ind.left_align(" foo", "-", ".");
  JUST_ASSERT_EQUAL(". foo\n", ind.str());
}

JUST_TEST_CASE(test_left_aligning_text_with_trailing_whitespace)
{
  indenter ind(10);
  ind.left_align("foo ", "-", ".");
  JUST_ASSERT_EQUAL(".foo\n", ind.str());
}

