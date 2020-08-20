#ifndef METASHELL_CPP_CODE_HPP
#define METASHELL_CPP_CODE_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/constraint/code.hpp>

#include <metashell/data/string.hpp>

namespace metashell
{
  namespace data
  {
    class cpp_code : string<cpp_code, true, constraint::code>
    {
    public:
      using string<cpp_code, true, constraint::code>::string;
      using string<cpp_code, true, constraint::code>::value;
      using string<cpp_code, true, constraint::code>::size;
      using string<cpp_code, true, constraint::code>::begin;
      using string<cpp_code, true, constraint::code>::end;
      using string<cpp_code, true, constraint::code>::empty;
      using string<cpp_code, true, constraint::code>::substr;

      static constexpr const char* name_of_type() { return "C++ code"; }
    };

    cpp_code add_markers(const cpp_code& code_, bool process_directives_);
    cpp_code remove_markers(const cpp_code& code_, bool process_directives_);

    int lines_in(const cpp_code& code_);
  }
}

#endif
