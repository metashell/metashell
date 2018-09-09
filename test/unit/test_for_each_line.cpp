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

#include <metashell/core/for_each_line.hpp>

#include <gtest/gtest.h>

#include <boost/algorithm/string/join.hpp>

#include <vector>

using namespace metashell;

namespace
{
  void test_with_lines(const std::vector<std::string>& lines_,
                       const std::string& line_separator_)
  {
    std::vector<std::string> result;
    for_each_line(boost::algorithm::join(lines_, line_separator_),
                  [&result](const std::string& s_) { result.push_back(s_); });
    ASSERT_EQ(lines_, result);
  }

  void test_with_lines(const std::initializer_list<std::string>& lines_)
  {
    test_with_lines(lines_, "\n");
    test_with_lines(lines_, "\r");
    test_with_lines(lines_, "\r\n");
  }
}

TEST(for_each_line, inputs)
{
  test_with_lines({});
  test_with_lines({"hello"});
  test_with_lines({"hello", "world"});
  test_with_lines({"foo", "bar", "baz"});
  test_with_lines({"foo", ""});
  test_with_lines({"foo", "", "bar"});
  test_with_lines({"ab", "c"});
}
