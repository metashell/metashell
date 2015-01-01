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

#include "frame.hpp"

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <cassert>
#include <iostream>

using namespace metashell_system_test;

frame::frame(const type& name_) :
  _name(name_)
{}

frame::frame(const type& name_, instantiation_kind kind_) :
  _name(name_),
  _kind(kind_)
{}

const type& frame::name() const
{
  return _name;
}

bool frame::has_kind() const
{
  return _kind != boost::none;
}

instantiation_kind frame::kind() const
{
  assert(has_kind());
  return *_kind;
}

std::ostream& metashell_system_test::operator<<(
  std::ostream& o_,
  const frame& f_
)
{
  return o_ << to_json_string(f_);
}

json_string metashell_system_test::to_json_string(const frame& f_)
{
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> w(buff);

  w.StartObject();

  w.Key("type");
  w.String("frame");

  w.Key("name");
  w.String(f_.name().name().c_str());

  if (f_.has_kind())
  {
    w.Key("kind");
    const std::string kind = to_string(f_.kind());
    w.String(kind.c_str());
  }

  w.EndObject();

  return json_string(buff.GetString());
}

bool metashell_system_test::operator==(
  const frame& frame_,
  const json_string& s_
)
{
  return to_json_string(frame_) == s_;
}

