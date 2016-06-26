#ifndef METASHELL_SYSTEM_TEST_JSON_STRING_HPP
#define METASHELL_SYSTEM_TEST_JSON_STRING_HPP

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

#include <boost/operators.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace system_test
  {
    class json_string : boost::equality_comparable<json_string>
    {
    public:
      explicit json_string(const std::string& json_);

      const std::string& get() const;

    private:
      std::string _json;
    };

    bool operator==(const json_string& a_, const json_string& b_);
    std::ostream& operator<<(std::ostream& out_, const json_string& s_);
  }
}

#endif
