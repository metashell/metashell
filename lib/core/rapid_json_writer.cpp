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

#include <metashell/rapid_json_writer.hpp>

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <iostream>
#include <cassert>

using namespace metashell;

rapid_json_writer::rapid_json_writer(std::ostream& out_) :
  _os(out_),
  _writer(_os)
{}

void rapid_json_writer::string(const std::string& value_)
{
  _writer.String(value_.c_str(), value_.size());
}

void rapid_json_writer::int_(int value_)
{
  _writer.Int(value_);
}

void rapid_json_writer::start_object()
{
  _writer.StartObject();
}

void rapid_json_writer::key(const std::string& key_)
{
  _writer.Key(key_.c_str(), key_.size());
}

void rapid_json_writer::end_object()
{
  _writer.EndObject();
}

void rapid_json_writer::start_array()
{
  _writer.StartArray();
}

void rapid_json_writer::end_array()
{
  _writer.EndArray();
}

void rapid_json_writer::end_document()
{
  _os.new_line();
  _writer.Reset(_os);
}

