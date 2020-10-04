#ifndef METASHELL_ENGINE_NAME_HPP
#define METASHELL_ENGINE_NAME_HPP

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

#include <metashell/data/auto_engine_name.hpp>
#include <metashell/data/real_engine_name.hpp>

#include <variant.hpp>

#include <iosfwd>
#include <string>
#include <string_view>

namespace metashell
{
  namespace data
  {
    using engine_name = mpark::variant<auto_engine_name, real_engine_name>;

    std::string operator+(const std::string&, engine_name);
    std::string operator+(engine_name, const std::string&);

    engine_name parse_engine_name(const std::string_view&);

    std::string to_string(engine_name);
    std::ostream& operator<<(std::ostream&, engine_name);
  } // namespace data
} // namespace metashell

#endif
