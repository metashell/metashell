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

#include <metashell_system_test/backtrace.hpp>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace metashell_system_test;

backtrace::backtrace(std::vector<frame> frames_) : _frames(frames_) {}

backtrace::iterator backtrace::begin() const { return _frames.begin(); }

backtrace::iterator backtrace::end() const { return _frames.end(); }

std::ostream& metashell_system_test::operator<<(std::ostream& o_,
                                                const backtrace& c_)
{
  return o_ << to_json_string(c_);
}

json_string metashell_system_test::to_json_string(const backtrace& c_)
{
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> w(buff);

  w.StartObject();

  w.Key("type");
  w.String("backtrace");

  w.Key("frames");
  w.StartArray();
  for (const frame& f : c_)
  {
    w.StartObject();

    w.Key("name");
    const std::string name = f.name().name();
    w.String(name.c_str());

    if (f.has_kind())
    {
      w.Key("kind");
      const std::string kind = to_string(f.kind());
      w.String(kind.c_str());
    }

    w.EndObject();
  }
  w.EndArray();

  w.EndObject();

  return json_string(buff.GetString());
}

bool metashell_system_test::operator==(const backtrace& c_,
                                       const json_string& s_)
{
  return to_json_string(c_) == s_;
}
