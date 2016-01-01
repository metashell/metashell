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

#include <metashell_system_test/prompt.hpp>

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <iostream>

using namespace metashell_system_test;

prompt::prompt(const std::string& prompt_) : _prompt(prompt_) {}

const std::string& prompt::value() const { return _prompt; }

std::ostream& metashell_system_test::operator<<(std::ostream& out_,
                                                const prompt& prompt_)
{
  return out_ << to_json_string(prompt_);
}

json_string metashell_system_test::to_json_string(const prompt& p_)
{
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> w(buff);

  w.StartObject();

  w.Key("type");
  w.String("prompt");

  w.Key("prompt");
  w.String(p_.value().c_str());

  w.EndObject();

  return json_string(buff.GetString());
}

bool metashell_system_test::operator==(const prompt& prompt_,
                                       const json_string& s_)
{
  return to_json_string(prompt_) == s_;
}
