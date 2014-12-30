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

#include <sstream>
#include <iostream>

using namespace metashell;

namespace
{
  std::string to_string(bool b_)
  {
    return b_ ? "true" : "false";
  }

  template <class T>
  std::string unexpected_integer_element(T value_)
  {
    std::ostringstream s;
    s << "Unexpected integer element: " << value_;
    return s.str();
  }

  bool whitelisted(const std::string& field_)
  {
    return field_ == "type" || field_ == "cmd" || field_ == "code";
  }
}

rapid_object_handler::rapid_object_handler(iface::displayer& displayer_) :
  _empty(true),
  _failed(false),
  _in_object(false),
  _displayer(displayer_)
{}

bool rapid_object_handler::Null()
{
  _empty = false;
  fail("Unexpected null element");
  return false;
}

bool rapid_object_handler::Bool(bool b_)
{
  _empty = false;
  fail("Unexpected bool element: " + ::to_string(b_));
  return false;
}

bool rapid_object_handler::Int(int i_)
{
  _empty = false;
  fail(unexpected_integer_element(i_));
  return false;
}

bool rapid_object_handler::Uint(unsigned i_)
{
  _empty = false;
  fail(unexpected_integer_element(i_));
  return false;
}

bool rapid_object_handler::Int64(int64_t i_)
{
  _empty = false;
  fail(unexpected_integer_element(i_));
  return false;
}

bool rapid_object_handler::Uint64(uint64_t i_)
{
  _empty = false;
  fail(unexpected_integer_element(i_));
  return false;
}

bool rapid_object_handler::Double(double d_)
{
  _empty = false;

  std::ostringstream s;
  s << "Unexpected double element: " << d_;
  fail(s.str());

  return false;
}

bool rapid_object_handler::string(const std::string& str_)
{
  _empty = false;
  if (_in_object)
  {
    _fields.insert({_next_key, str_});
    return true;
  }
  else
  {
    fail("Unexpected string: " + str_);
    return false;
  }
}

bool rapid_object_handler::StartObject()
{
  _empty = false;
  if (_in_object)
  {
    fail("Unexpected nested object");
    return false;
  }
  else
  {
    _in_object = true;
    return true;
  }
}

bool rapid_object_handler::key(const std::string& str_)
{
  _empty = false;
  if (whitelisted(str_))
  {
    _next_key = str_;
    return true;
  }
  else
  {
    fail("Unknown field: " + str_);
    return false;
  }
}

bool rapid_object_handler::end_object()
{
  _empty = false;
  return true;
}

bool rapid_object_handler::StartArray()
{
  _empty = false;
  fail("Unexpected array");
  return false;
}

bool rapid_object_handler::end_array()
{
  _empty = false;
  fail("Unexpected array");
  return false;
}

bool rapid_object_handler::failed() const
{
  return _failed;
}

boost::optional<std::string> rapid_object_handler::field(
  const std::string& name_
) const
{
  const auto i = _fields.find(name_);
  if (i == _fields.end())
  {
    return boost::none;
  }
  else
  {
    return i->second;
  }
}

bool rapid_object_handler::empty() const
{
  return _empty;
}

void rapid_object_handler::fail(const std::string& msg_)
{
  if (!_failed)
  {
    _displayer.show_error(msg_);
    _failed = true;
  }
}

