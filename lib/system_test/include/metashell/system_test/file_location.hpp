#ifndef METASHELL_SYSTEM_TEST_FILE_LOCATION_HPP
#define METASHELL_SYSTEM_TEST_FILE_LOCATION_HPP

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

#include <metashell/system_test/json_string.hpp>

#include <pattern/placeholder.hpp>
#include <pattern/to_optional.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/operators.hpp>

#include <iosfwd>
#include <optional>
#include <stdexcept>
#include <string>

namespace metashell
{
  namespace system_test
  {
    class file_location
        : boost::equality_comparable<file_location, json_string>,
          boost::equality_comparable<file_location>
    {
    public:
      explicit file_location(const json_string& s_);

      template <class Filename, class Row, class Column>
      file_location(Filename filename_, Row row_, Column column_)
        : _filename(pattern::to_optional(filename_)),
          _row(pattern::to_optional(row_)),
          _column(pattern::to_optional(column_))
      {
      }

      template <class JsonDocument>
      file_location(const JsonDocument& d_)
        : _filename(std::nullopt), _row(std::nullopt), _column(std::nullopt)
      {
        init(d_);
      }

      bool filename_specified() const;
      const boost::filesystem::path& filename() const;

      bool row_specified() const;
      int row() const;

      bool column_specified() const;
      int column() const;

      bool operator==(const file_location& l_) const;

    private:
      std::optional<boost::filesystem::path> _filename;
      std::optional<int> _row;
      std::optional<int> _column;

      void init(const std::string& s_);

      template <class JsonDocument>
      void init(const JsonDocument& d_)
      {
        if (d_.IsString())
        {
          init(std::string(d_.GetString(), d_.GetStringLength()));
        }
        else
        {
          throw std::runtime_error("Invalid file_location");
        }
      }
    };

    std::ostream& operator<<(std::ostream& out_, const file_location& l_);

    std::string to_string(const file_location& l_);
    json_string to_json_string(const file_location& l_);

    bool operator==(const file_location& l_, const json_string& s_);
  }
}

#endif
