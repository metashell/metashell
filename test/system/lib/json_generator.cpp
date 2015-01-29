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

#include <metashell_system_test/json_generator.hpp>

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <map>

using namespace metashell_system_test;

namespace
{
  json_string json_object(
    std::initializer_list<std::pair<std::string, std::string>> values_
  )
  {
    rapidjson::StringBuffer buff;
    rapidjson::Writer<rapidjson::StringBuffer> w(buff);

    w.StartObject();

    for (const auto& p : values_)
    {
      w.Key(p.first.c_str());
      w.String(p.second.c_str());
    }

    w.EndObject();

    return json_string(buff.GetString());
  }
}

json_string metashell_system_test::command(const std::string& cmd_)
{
  return json_object({{"type", "cmd"}, {"cmd", cmd_}});
}

json_string metashell_system_test::code_completion(const std::string& code_)
{
  return json_object({{"type", "code_completion"}, {"code", code_}});
}

