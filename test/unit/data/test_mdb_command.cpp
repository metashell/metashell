// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/exception.hpp>
#include <metashell/data/mdb_command.hpp>

#include <gtest/gtest.h>

#include <functional>

#include <limits>
#include <string>

using namespace metashell::data;

TEST(mdb_command, test_int_argument_parsing)
{
  auto parse = [](const std::string& s_) {
    return int(mdb_command::argument_type(s_, whitespace()));
  };

  const int max_int = std::numeric_limits<int>::max();
  const int min_int = std::numeric_limits<int>::min();

  ASSERT_THROW(parse(""), exception);

  ASSERT_EQ(0, parse("0"));
  ASSERT_EQ(1, parse("1"));
  ASSERT_EQ(9, parse("9"));
  ASSERT_EQ(10, parse("10"));
  ASSERT_EQ(-10, parse("-10"));
  ASSERT_EQ(max_int, parse(std::to_string(max_int)));
  ASSERT_EQ(min_int, parse(std::to_string(min_int)));

  ASSERT_THROW(parse("a"), exception);
  ASSERT_THROW(parse("--10"), exception);
  ASSERT_THROW(parse("-"), exception);
  ASSERT_THROW(parse("-0"), exception);
  ASSERT_THROW(parse(std::to_string(max_int) + "0"), exception);
  ASSERT_THROW(
      parse(std::to_string(static_cast<unsigned int>(max_int) + 1)), exception);
  ASSERT_THROW(parse("-" + std::to_string(
                               static_cast<unsigned int>(-(min_int + 1)) + 2)),
               exception);
}
