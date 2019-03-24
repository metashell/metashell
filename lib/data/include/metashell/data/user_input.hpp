#ifndef METASHELL_DATA_USER_INPUT_HPP
#define METASHELL_DATA_USER_INPUT_HPP

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

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/mdb_command.hpp>

#include <boost/operators.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    class user_input : boost::totally_ordered<user_input>,
                       boost::addable<user_input>
    {
    public:
      using size_type = std::string::size_type;
      using iterator = std::string::const_iterator;
      using const_iterator = std::string::const_iterator;

      user_input() = default;
      explicit user_input(std::string);
      explicit user_input(const cpp_code&);

      template <class InputIterator>
      user_input(InputIterator begin_, InputIterator end_)
        : _value(begin_, end_)
      {
      }

      const std::string& value() const;
      const char* c_str() const;

      size_type size() const;
      bool empty() const;

      void clear();

      char back() const;

      user_input substr(size_t pos_, size_t len_) const;

      user_input& operator+=(const user_input&);

      const_iterator begin() const;
      const_iterator end() const;

      explicit operator cpp_code() const;
      explicit operator mdb_command() const;

    private:
      std::string _value;
    };

    std::ostream& operator<<(std::ostream&, const user_input&);
    std::string to_string(const user_input&);

    bool operator==(const user_input&, const user_input&);
    bool operator<(const user_input&, const user_input&);

    bool has_non_whitespace(const user_input&);
  }
}

#endif
