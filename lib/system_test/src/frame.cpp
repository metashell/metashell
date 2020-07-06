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

#include <metashell/system_test/frame.hpp>
#include <metashell/system_test/util.hpp>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <cassert>
#include <iostream>

namespace metashell
{
  namespace system_test
  {
    frame::frame(const type& name_) : _name(name_) {}

    frame::frame(const json_string& s_)
      : _name(std::nullopt),
        _source_location(std::nullopt),
        _point_of_event(std::nullopt),
        _kind(std::nullopt)
    {
      rapidjson::Document d;
      d.Parse(s_.get().c_str());
      init(d, true);
    }

    bool frame::has_name() const { return _name != std::nullopt; }

    const type& frame::name() const { return *_name; }

    bool frame::has_source_location() const
    {
      return _source_location != std::nullopt;
    }

    const file_location& frame::source_location() const
    {
      return *_source_location;
    }

    bool frame::has_point_of_event() const
    {
      return _point_of_event != std::nullopt;
    }

    const file_location& frame::point_of_event() const
    {
      return *_point_of_event;
    }

    bool frame::has_kind() const { return _kind != std::nullopt; }

    event_kind frame::kind() const
    {
      assert(has_kind());
      return *_kind;
    }

    bool frame::operator==(const frame& f_) const
    {
      return matches(_name, f_._name) &&
             matches(_source_location, f_._source_location) &&
             matches(_point_of_event, f_._point_of_event) &&
             matches(_kind, f_._kind);
    }

    std::ostream& operator<<(std::ostream& o_, const frame& f_)
    {
      return o_ << to_json_string(f_);
    }

    json_string to_json_string(const frame& f_)
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

    bool operator==(const frame& frame_, const json_string& s_)
    {
      rapidjson::Document d;
      d.Parse(s_.get().c_str());
      return d.IsObject() && d.HasMember("type") &&
             is_string("frame", d["type"]) && matches(frame_, d);
    }
  }
}
