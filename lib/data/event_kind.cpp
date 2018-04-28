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

#include <metashell/data/event_kind.hpp>

namespace metashell
{
  namespace data
  {
    std::ostream& operator<<(std::ostream& os, event_kind kind)
    {
      return os << to_string(kind);
    }

    std::string to_string(event_kind kind)
    {
      switch (kind)
      {
#ifdef PREPROCESSOR_EVENT_KIND
#error PREPROCESSOR_EVENT_KIND defined
#endif
#define PREPROCESSOR_EVENT_KIND(name, str, rdepth)                             \
  case event_kind::name:                                                       \
    return str;

#ifdef TEMPLATE_EVENT_KIND
#error TEMPLATE_EVENT_KIND defined
#endif
#define TEMPLATE_EVENT_KIND(name, str, rdepth)                                 \
  case event_kind::name:                                                       \
    return str;

#ifdef MISC_EVENT_KIND
#error MISC_EVENT_KIND defined
#endif
#define MISC_EVENT_KIND(name, str, rdepth)                                     \
  case event_kind::name:                                                       \
    return str;

#include <metashell/data/impl/event_kind_list.hpp>

#undef MISC_EVENT_KIND
#undef TEMPLATE_EVENT_KIND
#undef PREPROCESSOR_EVENT_KIND
      }
      return "UnknownKind";
    }

    bool enabled(event_kind kind)
    {
      switch (kind)
      {
      case event_kind::memoization:
      case event_kind::template_instantiation:
      case event_kind::deduced_template_argument_substitution:
      case event_kind::explicit_template_argument_substitution:
      case event_kind::declaring_special_member:
      case event_kind::defining_synthesized_function:

      case event_kind::macro_expansion:
      case event_kind::macro_definition:
      case event_kind::macro_deletion:
      case event_kind::rescanning:
      case event_kind::expanded_code:
      case event_kind::generated_token:
      case event_kind::skipped_token:
      case event_kind::quote_include:
      case event_kind::sys_include:
      case event_kind::preprocessing_condition:
      case event_kind::preprocessing_condition_result:
      case event_kind::preprocessing_else:
      case event_kind::preprocessing_endif:
      case event_kind::error_directive:
      case event_kind::line_directive:
        return true;
      default:
        return false;
      }
    }
  }
}
