#ifndef METASHELL_MERGE_REPEATED_EVENTS_HPP
#define METASHELL_MERGE_REPEATED_EVENTS_HPP

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
#include <metashell/data/metaprogram_mode.hpp>

#include <metashell/core/filter_with_queue.hpp>

#include <optional>
#include <vector>

namespace metashell
{
  namespace core
  {
    template <class Events>
    class filter_merge_repeated_events_t
    {
    public:
      explicit filter_merge_repeated_events_t(Events&& events_)
        : _events(std::move(events_))
      {
      }

      std::optional<data::event_data> next()
      {
        while (std::optional<data::event_data> event = _events.next())
        {
          switch (relative_depth_of(*event))
          {
          case data::relative_depth::open:
            _last_open.back() = event;
            _last_open.emplace_back(std::nullopt);
            return event;
          case data::relative_depth::flat:
            return event;
          case data::relative_depth::close:
          case data::relative_depth::end:
            _last_open.pop_back();
            if (std::optional<data::event_data> ahead = _events.next())
            {
              if (what(*ahead) == what(*_last_open.back()))
              {
                _last_open.emplace_back(std::nullopt);
              }
              else
              {
                _events.queue(std::move(*ahead));
                return event;
              }
            }
            else
            {
              return event;
            }
            break;
          }
        }
        return std::nullopt;
      }

      data::cpp_code root_name() const { return _events.root_name(); }

      data::metaprogram_mode mode() const { return _events.mode(); }

    private:
      filter_with_queue<Events> _events;

      std::vector<std::optional<data::event_data>> _last_open{std::nullopt};
    };

    template <class Events>
    filter_merge_repeated_events_t<Events>
    filter_merge_repeated_events(Events&& events_)
    {
      return filter_merge_repeated_events_t<Events>(std::move(events_));
    }
  }
}

#endif
