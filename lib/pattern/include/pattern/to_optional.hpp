#ifndef PATTERN_TO_OPTIONAL_HPP
#define PATTERN_TO_OPTIONAL_HPP

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

#include <pattern/placeholder.hpp>

#include <optional>

namespace pattern
{
  inline std::nullopt_t to_optional(placeholder) { return std::nullopt; }

  template <class T>
  std::optional<T> to_optional(T value_)
  {
    return std::optional<T>(std::move(value_));
  }
}

#endif
