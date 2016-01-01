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

#include <metashell/path_builder.hpp>

#include <just/test.hpp>

using namespace metashell;

namespace
{
  std::string join_path(const std::string& a_, const std::string& b_)
  {
#ifdef _WIN32
    return a_ + "\\" + b_;
#else
    return a_ + "/" + b_;
#endif
  }
}

JUST_TEST_CASE(test_empty_path)
{
  const std::string p = path_builder();
  JUST_ASSERT_EQUAL("", p);
}

JUST_TEST_CASE(test_one_element_path)
{
  const std::string p = path_builder("foo");
  JUST_ASSERT_EQUAL("foo", p);
}

JUST_TEST_CASE(test_appending_to_empty_path)
{
  const std::string p = path_builder() / "foo";
  JUST_ASSERT_EQUAL("foo", p);
}

JUST_TEST_CASE(test_appending_to_non_empty_path)
{
  const std::string p = path_builder("foo") / "bar";
  JUST_ASSERT_EQUAL(join_path("foo", "bar"), p);
}
