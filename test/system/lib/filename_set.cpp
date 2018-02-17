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

#include <metashell/system_test/filename_set.hpp>
#include <metashell/system_test/query_json.hpp>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <boost/filesystem.hpp>

#include <iostream>

using namespace metashell::system_test;

filename_set::filename_set() : _paths() {}

filename_set::filename_set(const json_string& s_)
{
  rapidjson::Document d;
  d.Parse(s_.get().c_str());

  if (members_are({"type", "filenames"}, d) &&
      is_string("filename_set", d["type"]) && d["filenames"].IsArray())
  {
    const auto e = d["filenames"].End();
    for (auto i = d["filenames"].Begin(); i != e; ++i)
    {
      if (i->IsString())
      {
        _paths.insert(boost::filesystem::canonical(i->GetString()));
      }
      else
      {
        throw std::runtime_error("Invalid filename in filename_set:" +
                                 s_.get());
      }
    }
  }
  else
  {
    throw std::runtime_error("Invalid filename_set: " + s_.get());
  }
}

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

bool filename_set::operator==(const filename_set& fs_) const
{
  return _paths == fs_._paths;
}

std::ostream& metashell::system_test::operator<<(std::ostream& out_,
                                                 const filename_set& filenames_)
{
  return out_ << to_json_string(filenames_);
}

json_string
metashell::system_test::to_json_string(const filename_set& filenames_)
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

bool metashell::system_test::operator==(const filename_set& filenames_,
                                        const json_string& s_)
{
  return filenames_ == filename_set(s_);
}
