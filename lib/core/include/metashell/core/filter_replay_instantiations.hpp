#ifndef METASHELL_FILTER_REPLAY_INSTANTIATIONS_HPP
#define METASHELL_FILTER_REPLAY_INSTANTIATIONS_HPP

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

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/event_data.hpp>
#include <metashell/data/file_location.hpp>
#include <metashell/data/metaprogram_mode.hpp>

#include <metashell/core/event_cache.hpp>
#include <metashell/core/filter_with_queue.hpp>

#include <optional>

namespace metashell
{
  namespace core
  {
    template <class Events>
    class filter_replay_instantiations_t
    {
    public:
      explicit filter_replay_instantiations_t(
          Events&& events_, std::optional<data::file_location> from_)
        : _events(std::move(events_)),
          _from(std::move(from_)),
          _replaying(false)
      {
      }

      std::optional<data::event_data> next()
      {
        std::optional<data::event_data> event = _events.next();

        if (_from && event)
        {
          if (!_replaying && from_here(*event))
          {
            _replaying = true;
          }
          if (_replaying)
          {
            data::list<data::event_data> r = _cache.replay(*event);
            event = std::move(r.head);
            _events.queue(r.tail);
            _cache.erase_related(*event);
          }
          else
          {
            _cache.record(*event);
          }
        }

        return event;
      }

      data::cpp_code root_name() const { return _events.root_name(); }

      data::metaprogram_mode mode() const { return _events.mode(); }

    private:
      filter_with_queue<Events> _events;
      std::optional<data::file_location> _from;
      bool _replaying;
      event_cache _cache;

      bool from_here(const data::event_data& event_) const
      {
        return !_from || from_line(event_, *_from);
      }
    };

    template <class Events>
    filter_replay_instantiations_t<Events>
    filter_replay_instantiations(Events&& events_,
                                 std::optional<data::file_location> from_)
    {
      return filter_replay_instantiations_t<Events>(
          std::move(events_), std::move(from_));
    }
  }
}

#endif
