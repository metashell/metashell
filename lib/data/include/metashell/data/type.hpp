#ifndef METASHELL_DATA_TYPE_HPP
#define METASHELL_DATA_TYPE_HPP

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

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/string.hpp>

#include <optional>

namespace metashell
{
  namespace data
  {
    // Allowing empty values because the Templight trace migth contain entries
    // about types with empty name.
    class type : string<type, true, constraint::any, constraint::any, cpp_code>
    {
    public:
      using string<type, true, constraint::any, constraint::any, cpp_code>::
          string;

      using string<type, true, constraint::any, constraint::any, cpp_code>::
          value;

      using string<type, true, constraint::any, constraint::any, cpp_code>::
          size;

      using string<type, true, constraint::any, constraint::any, cpp_code>::
          begin;

      using string<type, true, constraint::any, constraint::any, cpp_code>::end;

      static constexpr const char* name_of_type() { return "type"; }

      const cpp_code& name() const;

      bool is_integral_constant(const type& type_,
                                const std::string& value_) const;

      operator cpp_code() const;
    };

    std::optional<type> trim_wrap_type(const type& type_);
    bool is_remove_ptr(const type& type_);
  }
}

#endif
