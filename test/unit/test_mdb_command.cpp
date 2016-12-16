// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/mdb_command.hpp>

#include <gtest/gtest.h>

#include "util.hpp"

using namespace metashell;

TEST(mdb_command, repeatable_constructor)
{
  mdb_command x(
      {"asdf"}, repeatable_t::repeatable, nullptr, "[asd]", "fdsa", "xxyy");

  ASSERT_EQ(std::vector<std::string>{"asdf"}, x.get_keys());
  ASSERT_TRUE(x.is_repeatable());
  ASSERT_EQ(x.get_usage(), "[asd]");
  ASSERT_EQ(x.get_short_description(), "fdsa");
  ASSERT_EQ(x.get_long_description(), "xxyy");
}

TEST(mdb_command, non_repeatable_constructor)
{
  mdb_command x(
      {"asdf"}, repeatable_t::non_repeatable, nullptr, "[asd]", "fdsa", "xxyy");

  ASSERT_EQ(std::vector<std::string>{"asdf"}, x.get_keys());
  ASSERT_FALSE(x.is_repeatable());
  ASSERT_EQ(x.get_short_description(), "fdsa");
  ASSERT_EQ(x.get_long_description(), "xxyy");
}

TEST(mdb_command, multiple_keys_constructor)
{
  mdb_command x({"asdf", "xxx"}, repeatable_t::non_repeatable, nullptr, "[asd]",
                "fd", "xx");

  ASSERT_EQ((std::vector<std::string>{"asdf", "xxx"}), x.get_keys());
  ASSERT_FALSE(x.is_repeatable());
  ASSERT_EQ(x.get_usage(), "[asd]");
  ASSERT_EQ(x.get_short_description(), "fd");
  ASSERT_EQ(x.get_long_description(), "xx");
}

TEST(mdb_command, full_description_empty_long_description)
{
  mdb_command x(std::vector<std::string>{"asdf"}, repeatable_t::non_repeatable,
                nullptr, "[asd]", "fd", "");

  ASSERT_EQ(x.get_usage(), "[asd]");
  ASSERT_EQ(x.get_short_description(), "fd");
  ASSERT_EQ(x.get_long_description(), "");
  ASSERT_EQ(x.get_full_description(), "fd");
}

TEST(mdb_command, full_description_non_empty_long_description)
{
  mdb_command x(std::vector<std::string>{"asdf"}, repeatable_t::non_repeatable,
                nullptr, "[asd]", "fd", "xx");

  ASSERT_EQ(x.get_usage(), "[asd]");
  ASSERT_EQ(x.get_short_description(), "fd");
  ASSERT_EQ(x.get_long_description(), "xx");
  ASSERT_EQ(x.get_full_description(), "fd\nxx");
}
