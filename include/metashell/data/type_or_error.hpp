#ifndef METASHELL_DATA_TYPE_OR_ERROR_HPP
#define METASHELL_DATA_TYPE_OR_ERROR_HPP

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

#include <boost/variant.hpp>

#include <metashell/data/type.hpp>

namespace metashell { namespace data {

class type_or_error {
public:
  typedef type type_type;
  typedef std::string error_type;

  static type_or_error make_type(const type_type& t);
  static type_or_error make_error(const error_type& e);

  bool is_type() const;
  bool is_error() const;

  const type_type& get_type() const;
  const error_type& get_error() const;

private:
  type_or_error(const type_type& t);
  type_or_error(const error_type& e);

  typedef boost::variant<type_type, error_type> data_type;
  data_type data;
};

}}

#endif
