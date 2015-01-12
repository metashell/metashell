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

using namespace metashell::data;

token::token() :
  _type(token_type::unknown),
  _value()
{}

token::token(std::string value_, token_type type_) :
  _type(type_),
  _value(std::move(value_))
{}

token_type token::type() const
{
  return _type;
}

token_category token::category() const
{
  return category_of_token(_type);
}

const std::string& token::value() const
{
  return _value;
}

