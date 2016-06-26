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

#include <metashell/system_test/type.hpp>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <cassert>
#include <iostream>

using namespace metashell::system_test;

type::type(const std::string& name_) : _name(name_) {}

type::type(pattern::placeholder) : _name(boost::none) {}

bool type::name_specified() const { return _name != boost::none; }

const std::string& type::name() const
{
  assert(name_specified());
  return *_name;
}

std::ostream& metashell::system_test::operator<<(std::ostream& out_,
                                                 const type& type_)
{
  return out_ << to_json_string(type_);
}

json_string metashell::system_test::to_json_string(const type& t_)
{
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> w(buff);

  w.StartObject();

  w.Key("type");
  w.String("type");

  w.Key("name");
  if (t_.name_specified())
  {
    w.String(t_.name().c_str());
  }
  else
  {
    w.Null();
  }

  w.EndObject();

  return json_string(buff.GetString());
}

bool metashell::system_test::operator==(const type& type_,
                                        const json_string& s_)
{
  rapidjson::Document d;
  d.Parse(s_.get().c_str());
  return members_are({"type", "name"}, d) && matches(type_, d["name"]);
}
