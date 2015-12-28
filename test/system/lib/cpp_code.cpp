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

#include <metashell_system_test/cpp_code.hpp>
#include <metashell_system_test/query_json.hpp>

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>


#include <iostream>
#include <cassert>

using namespace metashell_system_test;

cpp_code::cpp_code(const std::string& code_) :
  _code(code_)
{}

cpp_code::cpp_code(placeholder) :
  _code(boost::none)
{}

bool cpp_code::code_specified() const
{
  return _code != boost::none;
}

const std::string& cpp_code::code() const
{
  assert(code_specified());
  return *_code;
}

std::ostream& metashell_system_test::operator<<(
  std::ostream& out_,
  const cpp_code& cpp_code_
)
{
  return out_ << to_json_string(cpp_code_);
}

json_string metashell_system_test::to_json_string(const cpp_code& t_)
{
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> w(buff);

  w.StartObject();

  w.Key("type");
  w.String("cpp_code");

  w.Key("code");
  if (t_.code_specified())
  {
    w.String(t_.code().c_str());
  }
  else
  {
    w.Null();
  }

  w.EndObject();

  return json_string(buff.GetString());
}

bool metashell_system_test::operator==(
  const cpp_code& cpp_code_,
  const json_string& s_
)
{
  rapidjson::Document d;
  d.Parse(s_.get().c_str());

  return
    members_are({"type", "code"}, d)
    && is_string("cpp_code", d["type"])
    && (!cpp_code_.code_specified() || is_string(cpp_code_.code(), d["code"]));
}

