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

#include "mock_json_writer.hpp"

void mock_json_writer::string(const std::string& value_)
{
  _calls.push_back("string " + value_);
}

void mock_json_writer::int_(int value_)
{
  _calls.push_back("int " + std::to_string(value_));
}

void mock_json_writer::start_object()
{
  _calls.push_back("start_object");
}

void mock_json_writer::key(const std::string& key_)
{
  _calls.push_back("key " + key_);
}

void mock_json_writer::end_object()
{
  _calls.push_back("end_object");
}

const std::vector<std::string>& mock_json_writer::calls() const
{
  return _calls;
}

void mock_json_writer::start_array()
{
  _calls.push_back("start_array");
}

void mock_json_writer::end_array()
{
  _calls.push_back("end_array");
}

void mock_json_writer::end_document()
{
  _calls.push_back("end_document");
}

