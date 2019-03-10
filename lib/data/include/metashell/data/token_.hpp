#ifndef METASHELL_DATA_TOKEN__HPP
#define METASHELL_DATA_TOKEN__HPP

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
#include <metashell/data/token_category.hpp>
#include <metashell/data/token_type.hpp>

#include <boost/operators.hpp>

namespace metashell
{
  namespace data
  {
    template <token_category Category>
    class token_ : boost::totally_ordered<token_<Category>>
    {
    public:
      token_() = default;

      template <token_type Type>
      static token_ make(cpp_code value_)
      {
        return token_(std::move(value_), Type);
      }

      const cpp_code& value() const { return _value; }
      cpp_code& value() { return _value; }
      token_type type() const { return _type; }

    private:
      token_type _type;
      cpp_code _value;

      token_(cpp_code value_, token_type type_)
        : _type(type_), _value(std::move(value_))
      {
      }
    };

    template <token_type Type>
    auto make_token(cpp_code value_)
    {
      return token_<category_of_token(Type)>::template make<Type>(
          std::move(value_));
    }

    template <token_category Category>
    bool operator==(const token_<Category>& lhs_, const token_<Category>& rhs_)
    {
      return lhs_.type() == rhs_.type() && lhs_.value() == rhs_.value();
    }

    template <token_category Category>
    bool operator<(const token_<Category>& lhs_, const token_<Category>& rhs_)
    {
      return lhs_.type() < rhs_.type() ||
             (lhs_.type() == rhs_.type() && lhs_.value() < rhs_.value());
    }
  }
}

#endif
