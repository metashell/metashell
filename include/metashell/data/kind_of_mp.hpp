#ifndef METASHELL_DATA_KIND_OF_MP_HPP
#define METASHELL_DATA_KIND_OF_MP_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/event_kind.hpp>

namespace metashell
{
  namespace data
  {
    template <class T>
    struct kind_of_mp;

    template <class T>
    struct kind_of_mp<const T> : kind_of_mp<T>
    {
    };

    template <class T>
    struct kind_of_mp<T&> : kind_of_mp<T>
    {
    };

    template <event_kind Kind>
    struct timeless_event_details;

    template <event_kind Kind>
    struct kind_of_mp<timeless_event_details<Kind>>
    {
      typedef kind_of_mp type;
      static constexpr event_kind value = Kind;
    };
  }
}

#endif
