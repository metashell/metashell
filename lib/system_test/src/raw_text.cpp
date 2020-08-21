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

#include <metashell/system_test/query_json.hpp>
#include <metashell/system_test/raw_text.hpp>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <iostream>

namespace metashell
{
  namespace system_test
  {
    raw_text::raw_text(const std::string& text_) : _text(text_) {}

    raw_text::raw_text(pattern::placeholder) : _text(std::nullopt) {}

    raw_text::raw_text(const json_string& s_) : _text(std::nullopt)
    {
      rapidjson::Document d;
      d.Parse(s_.get().c_str());

      if (members_are({"type", "value"}, d) &&
          is_string("raw_text", d["type"]) && d["value"].IsString())
      {
        _text = d["value"].GetString();
      }
      else
      {
        throw std::runtime_error("Invalid raw_text: " + s_.get());
      }
    }

    bool raw_text::text_specified() const { return bool(_text); }

    const std::string& raw_text::text() const { return *_text; }

    std::ostream& operator<<(std::ostream& out_, const raw_text& raw_text_)
    {
      return out_ << to_json_string(raw_text_);
    }

    json_string to_json_string(const raw_text& t_)
    {
      rapidjson::StringBuffer buff;
      rapidjson::Writer<rapidjson::StringBuffer> w(buff);

      w.StartObject();

      w.Key("type");
      w.String("raw_text");

      w.Key("value");
      if (t_.text_specified())
      {
        w.String(t_.text().c_str());
      }
      else
      {
        w.Null();
      }

      w.EndObject();

      return json_string(buff.GetString());
    }

    bool operator==(const raw_text& text_, const json_string& s_)
    {
      rapidjson::Document d;
      d.Parse(s_.get().c_str());

      return members_are({"type", "value"}, d) &&
             is_string("raw_text", d["type"]) &&
             (!text_.text_specified() || is_string(text_.text(), d["value"]));
    }
  }
}
