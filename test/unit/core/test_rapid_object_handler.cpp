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

#include <metashell/core/in_memory_displayer.hpp>
#include <metashell/core/null_displayer.hpp>
#include <metashell/core/rapid_object_handler.hpp>

#include <gtest/gtest.h>

using namespace metashell;

TEST(rapid_object_handler, new_is_empty)
{
  core::null_displayer d;
  ASSERT_TRUE(core::rapid_object_handler(d).empty());
}

TEST(rapid_object_handler, new_is_not_failed)
{
  core::null_displayer d;
  ASSERT_FALSE(core::rapid_object_handler(d).failed());
}

TEST(rapid_object_handler, new_is_failed_after_null)
{
  core::in_memory_displayer d;
  core::rapid_object_handler r(d);
  const bool b = r.Null();

  ASSERT_FALSE(b);
  ASSERT_TRUE(r.failed());
  ASSERT_EQ(std::vector<std::string>{"Unexpected null element"}, d.errors());
}

TEST(rapid_object_handler, new_is_failed_after_bool)
{
  core::in_memory_displayer d;
  core::rapid_object_handler r(d);
  const bool b = r.Bool(true);

  ASSERT_FALSE(b);
  ASSERT_TRUE(r.failed());
  ASSERT_EQ(
      std::vector<std::string>{"Unexpected bool element: true"}, d.errors());
}

TEST(rapid_object_handler, new_is_failed_after_int)
{
  core::in_memory_displayer d;
  core::rapid_object_handler r(d);
  const bool b = r.Int(1);

  ASSERT_FALSE(b);
  ASSERT_TRUE(r.failed());
  ASSERT_EQ(
      std::vector<std::string>{"Unexpected integer element: 1"}, d.errors());
}

TEST(rapid_object_handler, new_is_failed_after_uint)
{
  core::in_memory_displayer d;
  core::rapid_object_handler r(d);
  const bool b = r.Uint(1);

  ASSERT_FALSE(b);
  ASSERT_TRUE(r.failed());
  ASSERT_EQ(
      std::vector<std::string>{"Unexpected integer element: 1"}, d.errors());
}

TEST(rapid_object_handler, new_is_failed_after_int64)
{
  core::in_memory_displayer d;
  core::rapid_object_handler r(d);
  const bool b = r.Int64(1);

  ASSERT_FALSE(b);
  ASSERT_TRUE(r.failed());
  ASSERT_EQ(
      std::vector<std::string>{"Unexpected integer element: 1"}, d.errors());
}

TEST(rapid_object_handler, new_is_failed_after_uint64)
{
  core::in_memory_displayer d;
  core::rapid_object_handler r(d);
  const bool b = r.Uint64(1);

  ASSERT_FALSE(b);
  ASSERT_TRUE(r.failed());
  ASSERT_EQ(
      std::vector<std::string>{"Unexpected integer element: 1"}, d.errors());
}

TEST(rapid_object_handler, new_is_failed_after_double)
{
  core::in_memory_displayer d;
  core::rapid_object_handler r(d);
  const bool b = r.Double(11.13);

  ASSERT_FALSE(b);
  ASSERT_TRUE(r.failed());
  ASSERT_EQ(
      std::vector<std::string>{"Unexpected double element: 11.13"}, d.errors());
}

TEST(rapid_object_handler, new_is_failed_after_zero_double)
{
  core::in_memory_displayer d;
  core::rapid_object_handler r(d);
  const bool b = r.Double(0);

  ASSERT_FALSE(b);
  ASSERT_TRUE(r.failed());
  ASSERT_EQ(
      std::vector<std::string>{"Unexpected double element: 0"}, d.errors());
}

TEST(rapid_object_handler, new_is_failed_after_one_double)
{
  core::in_memory_displayer d;
  core::rapid_object_handler r(d);
  const bool b = r.Double(1);

  ASSERT_FALSE(b);
  ASSERT_TRUE(r.failed());
  ASSERT_EQ(
      std::vector<std::string>{"Unexpected double element: 1"}, d.errors());
}

TEST(rapid_object_handler, new_is_failed_after_standalone_string)
{
  core::in_memory_displayer d;
  core::rapid_object_handler r(d);
  const bool b = r.String("foo", sizeof("foo") - 1, true);

  ASSERT_FALSE(b);
  ASSERT_TRUE(r.failed());
  ASSERT_EQ(std::vector<std::string>{"Unexpected string: foo"}, d.errors());
}

TEST(rapid_object_handler, new_is_failed_after_start_array)
{
  core::in_memory_displayer d;
  core::rapid_object_handler r(d);
  const bool b = r.StartArray();

  ASSERT_FALSE(b);
  ASSERT_TRUE(r.failed());
  ASSERT_EQ(std::vector<std::string>{"Unexpected array"}, d.errors());
}

TEST(rapid_object_handler, not_failed_after_start_object)
{
  core::null_displayer d;
  core::rapid_object_handler r(d);
  const bool b = r.StartObject();

  ASSERT_TRUE(b);
  ASSERT_FALSE(r.failed());
}

TEST(rapid_object_handler, returns_none_for_non_existing_fields)
{
  core::null_displayer d;
  core::rapid_object_handler r(d);

  const std::optional<std::string> f = r.field("foo");

  ASSERT_FALSE(f);
}

TEST(rapid_object_handler, failed_after_parsing_non_whitelisted_key)
{
  core::in_memory_displayer d;
  core::rapid_object_handler r(d);

  r.StartObject();
  const bool b = r.Key("foo", sizeof("foo") - 1, true);

  ASSERT_FALSE(b);
  ASSERT_TRUE(r.failed());
  ASSERT_EQ(std::vector<std::string>{"Unknown field: foo"}, d.errors());
}

namespace
{
  void test_whitelisted_field(const std::string& field_)
  {
    core::null_displayer d;
    core::rapid_object_handler r(d);

    r.StartObject();
    const bool b_key = r.Key(field_.c_str(), field_.size(), true);
    ASSERT_TRUE(b_key);

    const bool b_value = r.String("foo", sizeof("foo") - 1, true);
    ASSERT_TRUE(b_value);

    ASSERT_FALSE(r.failed());

    const std::optional<std::string> f = r.field(field_);

    ASSERT_TRUE(f != std::nullopt);
    ASSERT_EQ("foo", *f);
  }
}

TEST(rapid_object_handler, whitelisted_fields)
{
  test_whitelisted_field("type");
  test_whitelisted_field("cmd");
  test_whitelisted_field("code");
}

TEST(rapid_object_handler, new_is_failed_after_nested_object)
{
  core::in_memory_displayer d;
  core::rapid_object_handler r(d);

  const bool b1 = r.StartObject();
  ASSERT_TRUE(b1);

  const bool b2 = r.Key("type", sizeof("type") - 1, true);
  ASSERT_TRUE(b2);

  const bool b3 = r.StartObject();

  ASSERT_FALSE(b3);
  ASSERT_TRUE(r.failed());
  ASSERT_EQ(std::vector<std::string>{"Unexpected nested object"}, d.errors());
}

TEST(rapid_object_handler, end_object_after_start_object_succeeds)
{
  core::null_displayer d;
  core::rapid_object_handler r(d);

  const bool b1 = r.StartObject();
  ASSERT_TRUE(b1);

  const bool b2 = r.EndObject(0);
  ASSERT_TRUE(b2);

  ASSERT_FALSE(r.failed());
}
