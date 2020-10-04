#ifndef METASHELL_DATA_STANDARD_HEADERS_ALLOWED_HPP
#define METASHELL_DATA_STANDARD_HEADERS_ALLOWED_HPP

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

#include <array>
#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    enum class standard_headers_allowed
    {
      none,
      c,
      cpp,
      all
    };

    constexpr std::array<standard_headers_allowed, 4>
        all_standard_headers_alloweds{
            {standard_headers_allowed::none, standard_headers_allowed::c,
             standard_headers_allowed::cpp, standard_headers_allowed::all}};

    standard_headers_allowed disable_c(standard_headers_allowed);
    standard_headers_allowed disable_cpp(standard_headers_allowed);

    std::string to_string(standard_headers_allowed);
    std::ostream& operator<<(std::ostream&, standard_headers_allowed);
  } // namespace data
} // namespace metashell

#endif
