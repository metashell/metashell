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

#include <metashell/system_test/filename_list.hpp>
#include <metashell/system_test/query_json.hpp>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <boost/range/algorithm/equal.hpp>

#include <iostream>
#include <stdexcept>

namespace metashell
{
  namespace system_test
  {
    filename_list::filename_list() : _paths() {}

    filename_list::filename_list(std::initializer_list<value_type> paths_)
      : _paths(paths_)
    {
    }

    filename_list::filename_list(const json_string& s_)
    {
      rapidjson::Document d;
      d.Parse(s_.get().c_str());

      if (members_are({"type", "filenames"}, d) &&
          is_string("filename_list", d["type"]))
      {
        if (d["filenames"].IsArray())
        {
          for (auto i = d["filenames"].Begin(), e = d["filenames"].End();
               i != e; ++i)
          {
            if (i->IsString())
            {
              _paths.push_back(i->GetString());
            }
            else
            {
              throw std::runtime_error("Invalid filename in filename_list: " +
                                       s_.get());
            }
          }
        }
      }
      else
      {
        throw std::runtime_error("Invalid filename_list: " + s_.get());
      }
    }

    filename_list::const_iterator filename_list::begin() const
    {
      return _paths.begin();
    }

    filename_list::const_iterator filename_list::end() const
    {
      return _paths.end();
    }

    bool filename_list::empty() const { return _paths.empty(); }

    std::ostream& operator<<(std::ostream& out_,
                             const filename_list& filenames_)
    {
      return out_ << to_json_string(filenames_);
    }

    json_string to_json_string(const filename_list& filenames_)
    {
      rapidjson::StringBuffer buff;
      rapidjson::Writer<rapidjson::StringBuffer> w(buff);

      w.StartObject();

      w.Key("type");
      w.String("filename_list");

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

    bool operator==(const filename_list& filenames_, const json_string& s_)
    {
      return to_json_string(filenames_) == s_;
    }

    bool operator==(const filename_list& a_, const filename_list& b_)
    {
      return boost::range::equal(a_, b_);
    }
  } // namespace system_test
} // namespace metashell
