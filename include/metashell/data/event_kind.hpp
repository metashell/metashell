#ifndef METASHELL_EVENT_KIND_HPP
#define METASHELL_EVENT_KIND_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/data/event_category.hpp>
#include <metashell/data/relative_depth.hpp>

#include <ostream>
#include <string>

#ifdef PREPROCESSOR_EVENT_KIND
#error PREPROCESSOR_EVENT_KIND defined
#endif

#ifdef TEMPLATE_EVENT_KIND
#error TEMPLATE_EVENT_KIND defined
#endif

#ifdef MISC_EVENT_KIND
#error MISC_EVENT_KIND defined
#endif

namespace metashell
{
  namespace data
  {
    enum class event_kind
    {
#define PREPROCESSOR_EVENT_KIND(name, str, rdepth) name,
#define TEMPLATE_EVENT_KIND(name, str, rdepth) name,
#define MISC_EVENT_KIND(name, str, rdepth) name,
#include <metashell/data/impl/event_kind_list.hpp>
#undef MISC_EVENT_KIND
#undef TEMPLATE_EVENT_KIND
#undef PREPROCESSOR_EVENT_KIND
    };

    constexpr event_category category(event_kind kind)
    {
      switch (kind)
      {
#define PREPROCESSOR_EVENT_KIND(name, str, rdepth) case event_kind::name:
#define TEMPLATE_EVENT_KIND(name, str, rdepth)
#define MISC_EVENT_KIND(name, str, rdepth)
#include <metashell/data/impl/event_kind_list.hpp>
#undef MISC_EVENT_KIND
#undef TEMPLATE_EVENT_KIND
#undef PREPROCESSOR_EVENT_KIND
        return event_category::preprocessor;
#define PREPROCESSOR_EVENT_KIND(name, str, rdepth)
#define TEMPLATE_EVENT_KIND(name, str, rdepth) case event_kind::name:
#define MISC_EVENT_KIND(name, str, rdepth)
#include <metashell/data/impl/event_kind_list.hpp>
#undef MISC_EVENT_KIND
#undef TEMPLATE_EVENT_KIND
#undef PREPROCESSOR_EVENT_KIND
        return event_category::template_;
#define PREPROCESSOR_EVENT_KIND(name, str, rdepth)
#define TEMPLATE_EVENT_KIND(name, str, rdepth)
#define MISC_EVENT_KIND(name, str, rdepth) case event_kind::name:
#include <metashell/data/impl/event_kind_list.hpp>
#undef MISC_EVENT_KIND
#undef TEMPLATE_EVENT_KIND
#undef PREPROCESSOR_EVENT_KIND
        return event_category::misc;
      }
    }

    constexpr relative_depth relative_depth_of(event_kind kind)
    {
      switch (kind)
      {
#define PREPROCESSOR_EVENT_KIND(name, str, rdepth)                             \
  case event_kind::name:                                                       \
    return relative_depth::rdepth;
#define TEMPLATE_EVENT_KIND PREPROCESSOR_EVENT_KIND
#define MISC_EVENT_KIND PREPROCESSOR_EVENT_KIND
#include <metashell/data/impl/event_kind_list.hpp>
#undef MISC_EVENT_KIND
#undef TEMPLATE_EVENT_KIND
#undef PREPROCESSOR_EVENT_KIND
      }
    }

    std::ostream& operator<<(std::ostream& os, event_kind kind);

    std::string to_string(event_kind kind);
  }
}

#ifdef PREPROCESSOR_EVENT_KIND
#error PREPROCESSOR_EVENT_KIND defined
#endif

#ifdef TEMPLATE_EVENT_KIND
#error TEMPLATE_EVENT_KIND defined
#endif

#ifdef MISC_EVENT_KIND
#error MISC_EVENT_KIND defined
#endif

#endif
