#ifndef METASHELL_FILTER_EXPAND_MEMOIZATIONS_HPP
#define METASHELL_FILTER_EXPAND_MEMOIZATIONS_HPP

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

#include <metashell/data/event_data.hpp>
#include <metashell/data/event_data_sequence.hpp>
#include <metashell/data/file_location.hpp>

#include <metashell/event_cache.hpp>
#include <metashell/filter_with_queue.hpp>

#include <boost/optional.hpp>

namespace metashell
{
  template <class Events>
  class filter_expand_memoizations_t
      : public data::event_data_sequence<filter_expand_memoizations_t<Events>>
  {
  public:
    explicit filter_expand_memoizations_t(Events&& events_, bool enabled_)
      : _events(std::move(events_)), _enabled(enabled_), _skip(0)
    {
    }

    boost::optional<data::event_data> next()
    {
      boost::optional<data::event_data> event = _events.next();

      if (_enabled && event)
      {
        auto tail = _cache.replay(*event).tail;

        if (_skip > 0)
        {
          --_skip;
        }
        else
        {
          _cache.record(*event);
        }

        _skip += tail.size();
        _events.queue(tail);
      }

      return event;
    }

  private:
    filter_with_queue<Events> _events;
    bool _enabled;
    int _skip;
    event_cache _cache;
  };

  template <class Events>
  filter_expand_memoizations_t<Events>
  filter_expand_memoizations(Events&& events_, bool enabled_)
  {
    return filter_expand_memoizations_t<Events>(std::move(events_), enabled_);
  }
}

#endif
