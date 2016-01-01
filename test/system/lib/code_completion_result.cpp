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

#include <metashell_system_test/code_completion_result.hpp>
#include <metashell_system_test/query_json.hpp>

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>

#include <cassert>
#include <iostream>

using namespace metashell_system_test;

code_completion_result::code_completion_result(
    const std::initializer_list<std::string>& results_)
  : _results(results_)
{
}

code_completion_result::iterator code_completion_result::begin() const
{
  return _results.begin();
}

code_completion_result::iterator code_completion_result::end() const
{
  return _results.end();
}

int code_completion_result::size() const { return _results.size(); }

bool code_completion_result::contains(const std::string& result_) const
{
  return _results.find(result_) != _results.end();
}

code_completion_result code_completion_result::with(
    const std::initializer_list<std::string>& members_) const
{
  code_completion_result r(*this);
  r._results.insert(members_.begin(), members_.end());
  return std::move(r);
}

std::ostream& metashell_system_test::
operator<<(std::ostream& out_, const code_completion_result& r_)
{
  return out_ << to_json_string(r_);
}

json_string
metashell_system_test::to_json_string(const code_completion_result& r_)
{
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> w(buff);

  w.StartObject();

  w.Key("type");
  w.String("code_completion_result");

  w.Key("completions");
  w.StartArray();
  for (const std::string& r : r_)
  {
    w.String(r.c_str());
  }
  w.EndArray();

  w.EndObject();

  return json_string(buff.GetString());
}

bool metashell_system_test::operator==(const code_completion_result& r_,
                                       const json_string& s_)
{
  rapidjson::Document d;
  d.Parse(s_.get().c_str());

  if (members_are({"type", "completions"}, d) &&
      is_string("code_completion_result", d["type"]) &&
      d["completions"].IsArray() && int(d["completions"].Size()) == r_.size())
  {
    auto& completions = d["completions"];
    for (auto i = completions.Begin(), e = completions.End(); i != e; ++i)
    {
      if (!(i->IsString() &&
            r_.contains(std::string(i->GetString(), i->GetStringLength()))))
      {
        return false;
      }
    }
    return true;
  }
  else
  {
    return false;
  }
}
