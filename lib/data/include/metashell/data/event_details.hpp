#ifndef METASHELL_EVENT_DETAILS_HPP
#define METASHELL_EVENT_DETAILS_HPP

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

#include <iostream>
#include <optional>

namespace metashell
{
  namespace data
  {
    template <event_kind Kind>
    struct event_details
    {
      timeless_event_details<Kind> what;
      double timestamp;
    };

    template <event_kind Kind>
    std::optional<double> timestamp(const event_details<Kind>& details_)
    {
      return details_.timestamp;
    }

    template <event_kind Kind>
    std::ostream& operator<<(std::ostream& out_,
                             const event_details<Kind>& details_)
    {
      return out_ << "event_details<" << to_string(Kind) << ">{"
                  << details_.what << ", " << details_.timestamp << "}";
    }

    template <>
    struct event_details<event_kind::evaluation_end>
    {
      timeless_event_details<event_kind::evaluation_end> what;
    };

    std::optional<double>
    timestamp(const event_details<event_kind::evaluation_end>& details_);

    std::ostream&
    operator<<(std::ostream& out_,
               const event_details<event_kind::evaluation_end>& details_);
  }
}

#endif
