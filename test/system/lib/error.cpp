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

#include <metashell/system_test/error.hpp>
#include <metashell/system_test/query_json.hpp>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <cassert>
#include <iostream>

using namespace metashell::system_test;

error::error(const std::string& msg_) : _msg(msg_) {}

error::error(pattern::placeholder) : _msg(boost::none) {}

bool error::message_specified() const { return _msg != boost::none; }

const std::string& error::message() const
{
  assert(message_specified());
  return *_msg;
}

std::ostream& metashell::system_test::operator<<(std::ostream& out_,
                                                 const error& error_)
{
  return out_ << to_json_string(error_);
}

json_string metashell::system_test::to_json_string(const error& e_)
{
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> w(buff);

  w.StartObject();

  w.Key("type");
  w.String("error");

  w.Key("msg");
  if (e_.message_specified())
  {
    w.String(e_.message().c_str());
  }
  else
  {
    w.Null();
  }

  w.EndObject();

  return json_string(buff.GetString());
}

bool metashell::system_test::operator==(const error& error_,
                                        const json_string& s_)
{
  rapidjson::Document d;
  d.Parse(s_.get().c_str());

  return members_are({"type", "msg"}, d) && is_string("error", d["type"]) &&
         (!error_.message_specified() || is_string(error_.message(), d["msg"]));
}
