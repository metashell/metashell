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

#include <metashell/rapid_object_handler.hpp>
#include <metashell/null_displayer.hpp>
#include <metashell/in_memory_displayer.hpp>

#include <just/test.hpp>

using namespace metashell;

JUST_TEST_CASE(test_new_rapid_object_handler_is_empty)
{
  null_displayer d;
  JUST_ASSERT(rapid_object_handler(d).empty());
}

JUST_TEST_CASE(test_new_rapid_object_handler_is_not_failed)
{
  null_displayer d;
  JUST_ASSERT(!rapid_object_handler(d).failed());
}

JUST_TEST_CASE(test_new_rapid_object_handler_is_failed_after_null)
{
  in_memory_displayer d;
  rapid_object_handler r(d);
  const bool b = r.Null();

  JUST_ASSERT(!b);
  JUST_ASSERT(r.failed());
  JUST_ASSERT_EQUAL_CONTAINER({"Unexpected null element"}, d.errors());
}

JUST_TEST_CASE(test_new_rapid_object_handler_is_failed_after_bool)
{
  in_memory_displayer d;
  rapid_object_handler r(d);
  const bool b = r.Bool(true);

  JUST_ASSERT(!b);
  JUST_ASSERT(r.failed());
  JUST_ASSERT_EQUAL_CONTAINER({"Unexpected bool element: true"}, d.errors());
}

JUST_TEST_CASE(test_new_rapid_object_handler_is_failed_after_int)
{
  in_memory_displayer d;
  rapid_object_handler r(d);
  const bool b = r.Int(1);

  JUST_ASSERT(!b);
  JUST_ASSERT(r.failed());
  JUST_ASSERT_EQUAL_CONTAINER({"Unexpected integer element: 1"}, d.errors());
}

JUST_TEST_CASE(test_new_rapid_object_handler_is_failed_after_uint)
{
  in_memory_displayer d;
  rapid_object_handler r(d);
  const bool b = r.Uint(1);

  JUST_ASSERT(!b);
  JUST_ASSERT(r.failed());
  JUST_ASSERT_EQUAL_CONTAINER({"Unexpected integer element: 1"}, d.errors());
}

JUST_TEST_CASE(test_new_rapid_object_handler_is_failed_after_int64)
{
  in_memory_displayer d;
  rapid_object_handler r(d);
  const bool b = r.Int64(1);

  JUST_ASSERT(!b);
  JUST_ASSERT(r.failed());
  JUST_ASSERT_EQUAL_CONTAINER({"Unexpected integer element: 1"}, d.errors());
}

JUST_TEST_CASE(test_new_rapid_object_handler_is_failed_after_uint64)
{
  in_memory_displayer d;
  rapid_object_handler r(d);
  const bool b = r.Uint64(1);

  JUST_ASSERT(!b);
  JUST_ASSERT(r.failed());
  JUST_ASSERT_EQUAL_CONTAINER({"Unexpected integer element: 1"}, d.errors());
}

JUST_TEST_CASE(test_new_rapid_object_handler_is_failed_after_double)
{
  in_memory_displayer d;
  rapid_object_handler r(d);
  const bool b = r.Double(11.13);

  JUST_ASSERT(!b);
  JUST_ASSERT(r.failed());
  JUST_ASSERT_EQUAL_CONTAINER({"Unexpected double element: 11.13"}, d.errors());
}

JUST_TEST_CASE(test_new_rapid_object_handler_is_failed_after_standalone_string)
{
  in_memory_displayer d;
  rapid_object_handler r(d);
  const bool b = r.String("foo", sizeof("foo") - 1, true);

  JUST_ASSERT(!b);
  JUST_ASSERT(r.failed());
  JUST_ASSERT_EQUAL_CONTAINER({"Unexpected string: foo"}, d.errors());
}

JUST_TEST_CASE(test_new_rapid_object_handler_is_failed_after_start_array)
{
  in_memory_displayer d;
  rapid_object_handler r(d);
  const bool b = r.StartArray();

  JUST_ASSERT(!b);
  JUST_ASSERT(r.failed());
  JUST_ASSERT_EQUAL_CONTAINER({"Unexpected array"}, d.errors());
}

JUST_TEST_CASE(test_rapid_object_handler_is_not_failed_after_start_object)
{
  null_displayer d;
  rapid_object_handler r(d);
  const bool b = r.StartObject();

  JUST_ASSERT(b);
  JUST_ASSERT(!r.failed());
}

JUST_TEST_CASE(test_rapid_object_handler_returns_none_for_non_existing_fields)
{
  null_displayer d;
  rapid_object_handler r(d);

  const boost::optional<std::string> f = r.field("foo");

  JUST_ASSERT(!f);
}

JUST_TEST_CASE(
  test_rapid_object_handler_is_failed_after_parsing_non_whitelisted_key
)
{
  in_memory_displayer d;
  rapid_object_handler r(d);

  r.StartObject();
  const bool b = r.Key("foo", sizeof("foo") - 1, true);

  JUST_ASSERT(!b);
  JUST_ASSERT(r.failed());
  JUST_ASSERT_EQUAL_CONTAINER({"Unknown field: foo"}, d.errors());
}

namespace
{
  void test_whitelisted_field(const std::string& field_)
  {
    null_displayer d;
    rapid_object_handler r(d);

    r.StartObject();
    const bool b_key = r.Key(field_.c_str(), field_.size(), true);
    JUST_ASSERT(b_key);

    const bool b_value = r.String("foo", sizeof("foo") - 1, true);
    JUST_ASSERT(b_value);

    JUST_ASSERT(!r.failed());

    const boost::optional<std::string> f = r.field(field_);

    JUST_ASSERT(f != boost::none);
    JUST_ASSERT_EQUAL("foo", *f);
  }
}

JUST_TEST_CASE(test_rapid_object_handler_whitelisted_fields)
{
  test_whitelisted_field("type");
  test_whitelisted_field("cmd");
}

JUST_TEST_CASE(test_new_rapid_object_handler_is_failed_after_nested_object)
{
  in_memory_displayer d;
  rapid_object_handler r(d);

  const bool b1 = r.StartObject();
  JUST_ASSERT(b1);

  const bool b2 = r.Key("type", sizeof("type") - 1, true);
  JUST_ASSERT(b2);

  const bool b3 = r.StartObject();

  JUST_ASSERT(!b3);
  JUST_ASSERT(r.failed());
  JUST_ASSERT_EQUAL_CONTAINER({"Unexpected nested object"}, d.errors());
}

JUST_TEST_CASE(test_rapid_object_handler_end_object_after_start_object_succeeds)
{
  null_displayer d;
  rapid_object_handler r(d);

  const bool b1 = r.StartObject();
  JUST_ASSERT(b1);

  const bool b2 = r.EndObject(0);
  JUST_ASSERT(b2);

  JUST_ASSERT(!r.failed());
}

