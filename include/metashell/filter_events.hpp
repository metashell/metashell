#ifndef METASHELL_FILTER_EVENTS_HPP
#define METASHELL_FILTER_EVENTS_HPP

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

#include <metashell/filter_enable_reachable.hpp>
#include <metashell/filter_expand_memoizations.hpp>
#include <metashell/filter_merge_repeated_events.hpp>
#include <metashell/filter_repeated_memoization.hpp>
#include <metashell/filter_replay_instantiations.hpp>
#include <metashell/filter_unwrap_vertices.hpp>

namespace metashell
{
  template <class Events>
  auto filter_events(Events&& events_,
                     boost::optional<data::file_location> from_,
                     bool keep_memoization_after_instantiation_)
  {
    return filter_expand_memoizations(
        filter_repeated_memoization(
            keep_memoization_after_instantiation_,
            filter_unwrap_vertices(filter_enable_reachable(
                filter_replay_instantiations(
                    filter_merge_repeated_events(std::move(events_)), from_),
                from_))),
        !keep_memoization_after_instantiation_);
  }
}

#endif
