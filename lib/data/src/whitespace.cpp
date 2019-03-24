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

#include <metashell/data/exception.hpp>
#include <metashell/data/whitespace.hpp>

#include <algorithm>
#include <cctype>
#include <iostream>

namespace metashell
{
  namespace data
  {
    whitespace::whitespace(std::string value_) : _value(std::move(value_))
    {
      if (std::find_if(_value.begin(), _value.end(), [](char c_) {
            return !std::isspace(c_);
          }) != _value.end())
      {
        throw exception("Invalid whitespace: " + _value);
      }
    }

    const std::string& whitespace::value() const { return _value; }

    bool whitespace::empty() const { return _value.empty(); }

    std::ostream& operator<<(std::ostream& out_, const whitespace& w_)
    {
      return out_ << w_.value();
    }

    std::string to_string(const whitespace& w_) { return w_.value(); }

    bool operator==(const whitespace& lhs_, const whitespace& rhs_)
    {
      return lhs_.value() == rhs_.value();
    }

    std::string operator+(const whitespace& lhs_, const std::string& rhs_)
    {
      return lhs_.value() + rhs_;
    }

    std::string operator+(const std::string& lhs_, const whitespace& rhs_)
    {
      return lhs_ + rhs_.value();
    }
  }
}
