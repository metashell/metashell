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

#include <metashell/token_iterator.hpp>

#include <just/test.hpp>

using namespace metashell;

JUST_TEST_CASE(test_formatting_empty_token_list)
{
  JUST_ASSERT_EQUAL("", tokens_to_string(token_iterator(), token_iterator()));
}

JUST_TEST_CASE(test_formatting_one_token)
{
  const std::string src = "int hello";
  const token_iterator b = begin_tokens(src, "<test>");

  JUST_ASSERT_EQUAL("int", tokens_to_string(b, skip(b)));
}

JUST_TEST_CASE(test_formatting_more_tokens)
{
  const std::string src = "int hello";
  const token_iterator b = begin_tokens(src, "<test>");

  JUST_ASSERT_EQUAL("int hello", tokens_to_string(b, token_iterator()));
}

