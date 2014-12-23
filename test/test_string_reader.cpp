// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include "string_reader.hpp"

#include <just/test.hpp>

JUST_TEST_CASE(test_empty_string_reader)
{
  string_reader r{};

  const auto s = r(">");

  JUST_ASSERT(boost::none == s);
}

JUST_TEST_CASE(test_string_reader_with_one_element)
{
  string_reader r{"foo"};

  const auto s1 = r(">");
  const auto s2 = r(">");

  JUST_ASSERT(boost::none != s1);
  JUST_ASSERT_EQUAL("foo", *s1);
  JUST_ASSERT(boost::none == s2);
}

JUST_TEST_CASE(test_string_reader_with_two_elements)
{
  string_reader r{"foo", "bar"};

  const auto s1 = r(">");
  const auto s2 = r(">");
  const auto s3 = r(">");

  JUST_ASSERT(boost::none != s1);
  JUST_ASSERT_EQUAL("foo", *s1);
  JUST_ASSERT(boost::none != s2);
  JUST_ASSERT_EQUAL("bar", *s2);
  JUST_ASSERT(boost::none == s3);
}

