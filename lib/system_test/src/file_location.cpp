// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/file_location.hpp>
#include <metashell/system_test/query_json.hpp>
#include <metashell/system_test/util.hpp>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>

namespace metashell
{
  namespace system_test
  {
    namespace
    {
      bool is_int(const std::string& s_)
      {
        return !s_.empty() && std::all_of(s_.begin(), s_.end(), [](char c_) {
          return c_ >= '0' && c_ <= '9';
        });
      }
    }

    file_location::file_location(const json_string& s_)
      : _filename(std::nullopt), _row(std::nullopt), _column(std::nullopt)
    {
      rapidjson::Document d;
      d.Parse(s_.get().c_str());
      init(d);
    }

    void file_location::init(const std::string& s_)
    {
      std::vector<std::string> parts;
      boost::algorithm::split(parts, s_, [](char c_) { return c_ == ':'; });

      auto filename_end = parts.end();

      if (parts.size() > 1 && is_int(parts.back()))
      {
        if (parts.size() > 2 && is_int(parts[parts.size() - 2]))
        {
          --filename_end;
          _column = stoi(*filename_end);
        }
        --filename_end;
        _row = stoi(*filename_end);
      }
      _filename = boost::algorithm::join(
          boost::make_iterator_range(parts.begin(), filename_end), ":");
    }

    bool file_location::filename_specified() const
    {
      return _filename != std::nullopt;
    }

    const boost::filesystem::path& file_location::filename() const
    {
      assert(filename_specified());
      return *_filename;
    }

    bool file_location::row_specified() const { return _row != std::nullopt; }

    int file_location::row() const
    {
      assert(row_specified());
      return *_row;
    }

    bool file_location::column_specified() const
    {
      return _column != std::nullopt;
    }

    int file_location::column() const
    {
      assert(column_specified());
      return *_column;
    }

    bool file_location::operator==(const file_location& l_) const
    {
      return matches(_filename, l_._filename) && matches(_row, l_._row) &&
             matches(_column, l_._column);
    }

    std::ostream& operator<<(std::ostream& out_, const file_location& l_)
    {
      return out_ << to_json_string(l_);
    }

    std::string to_string(const file_location& l_)
    {
      return (l_.filename_specified() ? l_.filename().string() : "*") + ':' +
             (l_.row_specified() ? std::to_string(l_.row()) : "*") + ':' +
             (l_.column_specified() ? std::to_string(l_.column()) : "*");
    }

    json_string to_json_string(const file_location& l_)
    {
      rapidjson::StringBuffer buff;
      rapidjson::Writer<rapidjson::StringBuffer> w(buff);

      w.String(to_string(l_).c_str());

      return json_string(buff.GetString());
    }

    bool operator==(const file_location& l_, const json_string& s_)
    {
      return l_ == file_location(s_);
    }
  }
}
