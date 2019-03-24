#ifndef METASHELL_DATA_WHITESPACE_HPP
#define METASHELL_DATA_WHITESPACE_HPP

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

#include <boost/operators.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    class whitespace : boost::equality_comparable<whitespace>
    {
    public:
      whitespace() = default;
      explicit whitespace(std::string);

      template <class InputIt>
      whitespace(InputIt begin_, InputIt end_)
        : whitespace(std::string(begin_, end_))
      {
      }

      const std::string& value() const;

      bool empty() const;

    private:
      std::string _value;
    };

    std::ostream& operator<<(std::ostream&, const whitespace&);
    std::string to_string(const whitespace&);

    bool operator==(const whitespace&, const whitespace&);

    std::string operator+(const whitespace&, const std::string&);
    std::string operator+(const std::string&, const whitespace&);
  }
}

#endif
