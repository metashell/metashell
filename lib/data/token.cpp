// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/token.hpp>

#include <cassert>

using namespace metashell::data;

token::token() : _type(token_type::unknown), _value() {}

token::token(cpp_code value_, token_type type_)
  : _type(type_), _value(std::move(value_))
{
}

token_type token::type() const { return _type; }

token_category token::category() const { return category_of_token(_type); }

const cpp_code& token::value() const { return _value; }

std::string metashell::data::string_literal_value(const token& token_)
{
  assert(token_.type() == token_type::string_literal);

  std::string value = token_.value().value();
  if (!value.empty() && value[0] == '"')
  {
    value.erase(0, 1);
  }
  if (!value.empty() && value[value.size() - 1] == '"')
  {
    value.erase(value.size() - 1);
  }
  return value;
}
