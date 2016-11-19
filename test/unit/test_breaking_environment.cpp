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

#include "breaking_environment.hpp"

#include <metashell/data/config.hpp>

#include <gtest/gtest.h>

TEST(breaking_environment, does_not_throw)
{
  breaking_environment e;

  // should not throw

  e.append("foo");
  e.get();
  e.get_appended("bar");
  e.get_headers();
}

TEST(breaking_environment, append_throws)
{
  breaking_environment e;
  e.append_throw_from_now();

  ASSERT_ANY_THROW(e.append("foo"));
  e.get();
  e.get_appended("bar");
  e.get_headers();
}

TEST(breaking_environment, get_appended_throws)
{
  breaking_environment e;
  e.get_appended_throw_from_now();

  e.append("foo");
  e.get();
  ASSERT_ANY_THROW(e.get_appended("bar"));
  e.get_headers();
}
