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

#include <metashell/data/user_input.hpp>

#include <cctype>
#include <iostream>

namespace metashell
{
  namespace data
  {
    user_input::user_input(std::string value_) : _value(std::move(value_)) {}

    user_input::user_input(const cpp_code& c_) : _value(c_.value()) {}

    const std::string& user_input::value() const { return _value; }

    const char* user_input::c_str() const { return _value.c_str(); }

    user_input::size_type user_input::size() const { return _value.size(); }

    bool user_input::empty() const { return _value.empty(); }

    void user_input::clear() { _value.clear(); }

    user_input user_input::substr(size_t pos_, size_t len_) const
    {
      return user_input(_value.substr(pos_, len_));
    }

    char user_input::back() const { return _value.back(); }

    user_input& user_input::operator+=(const user_input& rhs_)
    {
      _value += rhs_._value;
      return *this;
    }

    user_input::const_iterator user_input::begin() const
    {
      return _value.begin();
    }

    user_input::const_iterator user_input::end() const { return _value.end(); }

    user_input::operator cpp_code() const { return cpp_code(_value); }

    std::ostream& operator<<(std::ostream& o_, const user_input& i_)
    {
      return o_ << i_.value();
    }

    std::string to_string(const user_input& i_) { return i_.value(); }

    bool operator==(const user_input& lhs_, const user_input& rhs_)
    {
      return lhs_.value() == rhs_.value();
    }

    bool operator<(const user_input& lhs_, const user_input& rhs_)
    {
      return lhs_.value() < rhs_.value();
    }

    bool has_non_whitespace(const user_input& s_)
    {
      for (char c : s_)
      {
        if (!std::isspace(c))
        {
          return true;
        }
      }
      return false;
    }
  }
}
