// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell_system_test/filename_set.hpp>

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>

#include <iostream>

using namespace metashell_system_test;

filename_set::filename_set() : _paths() {}

filename_set::filename_set(
    std::initializer_list<boost::filesystem::path> paths_)
  : _paths(paths_)
{
}

filename_set::const_iterator filename_set::begin() const
{
  return _paths.begin();
}

filename_set::const_iterator filename_set::end() const { return _paths.end(); }

std::ostream& metashell_system_test::operator<<(std::ostream& out_,
                                                const filename_set& filenames_)
{
  return out_ << to_json_string(filenames_);
}

json_string
metashell_system_test::to_json_string(const filename_set& filenames_)
{
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> w(buff);

  w.StartObject();

  w.Key("type");
  w.String("filename_set");

  w.Key("filenames");
  w.StartArray();
  for (const boost::filesystem::path& filename : filenames_)
  {
    w.String(filename.string().c_str());
  }
  w.EndArray();

  w.EndObject();

  return json_string(buff.GetString());
}

bool metashell_system_test::operator==(const filename_set& filenames_,
                                       const json_string& s_)
{
  return to_json_string(filenames_) == s_;
}
