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

#include <metashell/system_test/cpp_code.hpp>
#include <metashell/system_test/query_json.hpp>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <iostream>
#include <stdexcept>

using namespace metashell::system_test;

cpp_code::cpp_code(pattern::string code_) : _code(code_) {}

cpp_code::cpp_code(const json_string& s_) : _code(pattern::_)
{
  rapidjson::Document d;
  d.Parse(s_.get().c_str());

  if (members_are({"type", "code"}, d) && is_string("cpp_code", d["type"]) &&
      d["code"].IsString())
  {
    _code = d["code"].GetString();
  }
  else
  {
    throw std::runtime_error("Invalid cpp_code: " + s_.get());
  }
}
const pattern::string& cpp_code::code() const { return _code; }

std::ostream& metashell::system_test::operator<<(std::ostream& out_,
                                                 const cpp_code& cpp_code_)
{
  return out_ << to_json_string(cpp_code_);
}

json_string metashell::system_test::to_json_string(const cpp_code& t_)
{
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> w(buff);

  w.StartObject();

  w.Key("type");
  w.String("cpp_code");

  w.Key("code");
  write(t_.code(), w);

  w.EndObject();

  return json_string(buff.GetString());
}

bool metashell::system_test::operator==(const cpp_code& cpp_code_,
                                        const json_string& s_)
{
  rapidjson::Document d;
  d.Parse(s_.get().c_str());

  if (members_are({"type", "code"}, d) && is_string("cpp_code", d["type"]))
  {
    const auto& code = d["code"];
    return cpp_code_.code().match(
        std::string(code.GetString(), code.GetStringLength()));
  }
  else
  {
    return false;
  }
}
