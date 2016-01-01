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

#include <metashell_system_test/comment.hpp>
#include <metashell_system_test/query_json.hpp>

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>

#include <iostream>
#include <cassert>

using namespace metashell_system_test;

comment::comment(std::vector<paragraph> paragraphs_)
  : _paragraphs_specified(true), _paragraphs(move(paragraphs_))
{
}

comment::comment(placeholder) : _paragraphs_specified(false), _paragraphs() {}

bool comment::paragraphs_specified() const { return _paragraphs_specified; }

const std::vector<paragraph>& comment::paragraphs() const
{
  assert(paragraphs_specified());
  return _paragraphs;
}

std::ostream& metashell_system_test::operator<<(std::ostream& out_,
                                                const comment& comment_)
{
  return out_ << to_json_string(comment_);
}

json_string metashell_system_test::to_json_string(const comment& c_)
{
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> w(buff);

  w.StartObject();

  w.Key("type");
  w.String("comment");

  w.Key("paragraphs");
  if (c_.paragraphs_specified())
  {
    w.StartArray();
    for (const paragraph& p : c_.paragraphs())
    {
      write(p, w);
    }
    w.EndArray();
  }
  else
  {
    w.Null();
  }

  w.EndObject();

  return json_string(buff.GetString());
}

bool metashell_system_test::operator==(const comment& c_, const json_string& s_)
{
  rapidjson::Document d;
  d.Parse(s_.get().c_str());

  if (members_are({"type", "paragraphs"}, d) && is_string("comment", d["type"]))
  {
    if (c_.paragraphs_specified())
    {
      if (d["paragraphs"].IsArray() &&
          d["paragraphs"].Size() == c_.paragraphs().size())
      {
        auto i = d["paragraphs"].Begin();
        for (const paragraph& p : c_.paragraphs())
        {
          if (metashell_system_test::operator!=(p, *i))
          {
            return false;
          }
          ++i;
        }
        return true;
      }
    }
    else
    {
      return true;
    }
  }
  return false;
}
