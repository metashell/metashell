#ifndef METASHELL_SUPPORTED_HPP
#define METASHELL_SUPPORTED_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/core/not_supported.hpp>

#include <type_traits>

namespace metashell
{
  template <class T>
  struct supported : std::true_type
  {
  };

  template <>
  struct supported<not_supported> : std::false_type
  {
  };
}

#endif
