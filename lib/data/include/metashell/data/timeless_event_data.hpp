#ifndef METASHELL_DATA_TIMELESS_EVENT_DATA_HPP
#define METASHELL_DATA_TIMELESS_EVENT_DATA_HPP

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
#include <metashell/data/timeless_event_details.hpp>

#include <variant>

namespace metashell
{
  namespace data
  {
    typedef std::variant<
#ifdef PREPROCESSOR_EVENT_KIND
#error PREPROCESSOR_EVENT_KIND defined
#endif
#define PREPROCESSOR_EVENT_KIND(name, str, rdepth)                             \
  timeless_event_details<event_kind::name> EVENT_KIND_SEP

#ifdef TEMPLATE_EVENT_KIND
#error TEMPLATE_EVENT_KIND defined
#endif
#define TEMPLATE_EVENT_KIND PREPROCESSOR_EVENT_KIND

#ifdef MISC_EVENT_KIND
#error MISC_EVENT_KIND defined
#endif
#define MISC_EVENT_KIND PREPROCESSOR_EVENT_KIND

#include <metashell/data/impl/event_kind_list.hpp>

#undef MISC_EVENT_KIND
#undef TEMPLATE_EVENT_KIND
#undef PREPROCESSOR_EVENT_KIND
        >
        timeless_event_data;

    event_kind kind_of(const timeless_event_data& data);

    bool operator==(const timeless_event_data& a, const timeless_event_data& b);

    inline bool operator!=(const timeless_event_data& a,
                           const timeless_event_data& b)
    {
      return !(a == b);
    }
  } // namespace data
} // namespace metashell

#endif
