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

#include <metashell/core/source_position.hpp>

#include <gtest/gtest.h>

#include <boost/algorithm/string/join.hpp>

#include <sstream>
#include <stdexcept>
#include <vector>

using namespace metashell;

namespace
{
  core::source_position
  position_of(const std::initializer_list<std::string>& lines_,
              const std::string& line_separator_)
  {
    return core::source_position_of(
        boost::algorithm::join(lines_, line_separator_));
  }

  core::source_position
  position_of(const std::initializer_list<std::string>& lines_)
  {
    const std::vector<core::source_position> source_positions{
        position_of(lines_, "\n"), position_of(lines_, "\r"),
        position_of(lines_, "\r\n")};
    for (const core::source_position& p : source_positions)
    {
      if (p != source_positions[0])
      {
        std::ostringstream msg;
        msg << "position of: " << p << " != " << source_positions[0];
        throw std::runtime_error(msg.str());
      }
    }
    return source_positions[0];
  }
} // namespace

TEST(source_position, tests)
{
  const core::line_number l1(1);
  const core::line_number l2(2);

  const core::column c1(1);
  const core::column c2(2);
  const core::column c3(3);

  ASSERT_TRUE(core::source_position(l1, c2) == core::source_position(l1, c2));
  ASSERT_TRUE(core::source_position(l2, c1) != core::source_position(l1, c2));

  ASSERT_EQ("1:2", to_string(core::source_position(l1, c2)));

  ASSERT_EQ(core::source_position(l1, c1), position_of({}));
  ASSERT_EQ(core::source_position(l1, c2), position_of({"x"}));
  ASSERT_EQ(core::source_position(l1, c3), position_of({"ab"}));
  ASSERT_EQ(core::source_position(l2, c2), position_of({"ab", "c"}));
}
