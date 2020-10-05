#ifndef METASHELL_DATA_TYPE_OR_CODE_OR_ERROR_HPP
#define METASHELL_DATA_TYPE_OR_CODE_OR_ERROR_HPP

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

#include <ostream>

#include <variant.hpp>

#include <metashell/data/blank.hpp>
#include <metashell/data/type.hpp>

namespace metashell
{
  namespace data
  {
    class type_or_code_or_error
    {
    public:
      typedef type type_type;
      typedef cpp_code code_type;
      typedef std::string error_type;

      type_or_code_or_error();
      type_or_code_or_error(const type_type& t);
      type_or_code_or_error(const code_type& t);
      type_or_code_or_error(const error_type& e);

      static type_or_code_or_error make_none();
      static type_or_code_or_error make_type(const type_type& t);
      static type_or_code_or_error make_error(const error_type& e);

      bool is_none() const;
      bool is_type() const;
      bool is_code() const;
      bool is_error() const;

      const type_type& get_type() const;
      const code_type& get_code() const;
      const error_type& get_error() const;

      bool operator==(const type_or_code_or_error& other) const;

    private:
      typedef mpark::variant<blank, type_type, code_type, error_type> data_type;
      data_type data;
    };

    std::ostream& operator<<(std::ostream& os, const type_or_code_or_error& te);
  } // namespace data
} // namespace metashell

#endif
