// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2022, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/engine/templight/entry.hpp>

#include <gtest/gtest.h>

using namespace metashell::engine::templight;

TEST(entry, smaller)
{
  ASSERT_FALSE(smaller("", ""));

  ASSERT_FALSE(smaller("1", "1"));
  ASSERT_FALSE(smaller("a", "a"));

  ASSERT_TRUE(smaller("1", "2"));
  ASSERT_FALSE(smaller("2", "1"));

  ASSERT_TRUE(smaller("2", "10"));
  ASSERT_FALSE(smaller("10", "2"));

  ASSERT_TRUE(smaller("1", "a"));
  ASSERT_FALSE(smaller("a", "1"));

  ASSERT_TRUE(smaller("a", "b"));
  ASSERT_FALSE(smaller("b", "a"));
}
