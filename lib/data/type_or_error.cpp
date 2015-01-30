
// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/data/type_or_error.hpp>

#include <cassert>

namespace metashell { namespace data {

type_or_error::type_or_error(const type_type& t) : data(t) {}

type_or_error::type_or_error(const error_type& e) : data(e) {}

type_or_error type_or_error::make_type(const type_type& t) {
  return type_or_error(t);
}

type_or_error type_or_error::make_error(const error_type& e) {
  return type_or_error(e);
}

bool type_or_error::is_type() const {
  return boost::get<type_type>(&data) != nullptr;
}

bool type_or_error::is_error() const {
  return boost::get<error_type>(&data) != nullptr;
}

const type_or_error::type_type& type_or_error::get_type() const {
  assert(is_type());
  return boost::get<type_type>(data);
}

const type_or_error::error_type& type_or_error::get_error() const {
  assert(is_error());
  return boost::get<error_type>(data);
}

std::ostream& operator<<(std::ostream& os, const type_or_error& te) {
  if (te.is_type()) {
    os << "type[" << te.get_type() << "]";
  } else {
    os << "error[" << te.get_error() << "]";
  }
  return os;
}

}}
