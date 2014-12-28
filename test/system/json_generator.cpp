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

#include "json_generator.hpp"

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <map>

namespace
{
  std::string json_object(
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

    return buff.GetString();
  }
}

std::string command(const std::string& cmd_)
{
  return json_object({{"type", "cmd"}, {"cmd", cmd_}});
}

std::string prompt(const std::string& prompt_)
{
  return json_object({{"type", "prompt"}, {"prompt", prompt_}});
}

std::string type(const std::string& name_)
{
  return json_object({{"type", "type"}, {"name", name_}});
}

std::string raw_text(const std::string& value_)
{
  return json_object({{"type", "raw_text"}, {"value", value_}});
}

