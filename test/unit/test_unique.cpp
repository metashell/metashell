// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/unique.hpp>

#include <gtest/gtest.h>

#include <sstream>

using namespace metashell;

namespace
{
  template <class T>
  void sort_by_id(unique<T>& a_, unique<T>& b_)
  {
    if (a_.id() > b_.id())
    {
      std::swap(a_, b_);
    }
  }

  template <class T>
  std::string to_string_issues(T value_)
  {
    using std::to_string;

    const auto un = unique_value(value_);

    return to_string(value_) == to_string(un) ? "" : to_string(value_) +
                                                         " != " + to_string(un);
  }

  template <class T>
  std::string streaming_issues(T value_)
  {
    const auto un = unique_value(value_);

    std::ostringstream s_unique;
    s_unique << un;

    std::ostringstream s_value;
    s_value << value_;

    return s_unique.str() == s_value.str() ? "" : s_unique.str() + " != " +
                                                      s_value.str();
  }
}

TEST(unique, getting_value_back)
{
  ASSERT_EQ(13, unique<int>(13).value());
  ASSERT_EQ(21, unique<int>(21).value());
  ASSERT_EQ(31, unique_value(31).value());
  ASSERT_EQ("hello", unique<std::string>("hello").value());
}

TEST(unique, id_management)
{
  unique<int> a(13);
  unique<int> b(13);

  unique<int> copy_of_a = a;
  unique<int> assigned_a(0);
  assigned_a = a;

  ASSERT_NE(a.id(), b.id());
  ASSERT_EQ(a.id(), copy_of_a.id());
  ASSERT_EQ(a.id(), assigned_a.id());

  const auto a_id = a.id();
  unique<int> move_initialised_a = std::move(a);

  ASSERT_EQ(a_id, move_initialised_a.id());

  unique<int> moved_a(0);
  moved_a = std::move(move_initialised_a);
  ASSERT_EQ(a_id, moved_a.id());
}

TEST(unique, equality)
{
  const unique<int> a13(13);
  const unique<int> b13(13);

  const unique<int> a21(21);
  const unique<int> b21(21);

  ASSERT_EQ(a13, a13);
  ASSERT_EQ(b13, b13);
  ASSERT_EQ(a21, a21);
  ASSERT_EQ(b21, b21);

  ASSERT_NE(a13, b13);
  ASSERT_NE(a13, b21);
  ASSERT_NE(a21, b21);
}

TEST(unique, less)
{
  unique<int> a13(13);
  unique<int> b13(13);
  sort_by_id(a13, b13);

  ASSERT_LT(a13, b13);
  ASSERT_FALSE(b13 < a13);
}

TEST(unique, to_string)
{
  ASSERT_EQ("", to_string_issues(13));
  ASSERT_EQ("", to_string_issues('x'));
  ASSERT_EQ("", to_string_issues(4.21));
}

TEST(unique, streaming)
{
  ASSERT_EQ("", streaming_issues(13));
  ASSERT_EQ("", streaming_issues('x'));
  ASSERT_EQ("", streaming_issues(4.21));
}
