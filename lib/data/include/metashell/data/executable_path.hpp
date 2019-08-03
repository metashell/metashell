#ifndef METASHELL_DATA_EXECUTABLE_PATH_HPP
#define METASHELL_DATA_EXECUTABLE_PATH_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/command_line_argument.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/operators.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    class executable_path : boost::equality_comparable<executable_path>
    {
    public:
      explicit executable_path(const boost::filesystem::path&);
      explicit executable_path(const command_line_argument&);
      explicit executable_path(std::string);
      explicit executable_path(const char*);

      boost::filesystem::path value() const;
      const char* c_str() const;

      const std::string& as_string() const;

    private:
      std::string _value;
    };

    std::string operator+(const executable_path&, const std::string&);
    std::string operator+(const std::string&, const executable_path&);

    bool operator==(const executable_path&, const executable_path&);

    std::ostream& operator<<(std::ostream&, const executable_path&);
    std::string to_string(const executable_path&);

    executable_path absolute(const executable_path&);
  }
}

#endif
