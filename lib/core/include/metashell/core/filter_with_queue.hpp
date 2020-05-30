#ifndef METASHELL_FILTER_WITH_QUEUE_HPP
#define METASHELL_FILTER_WITH_QUEUE_HPP

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

#include <deque>
#include <optional>

namespace metashell
{
  namespace core
  {
    template <class Events>
    class filter_with_queue
    {
    public:
      explicit filter_with_queue(Events&& events_) : _events(std::move(events_))
      {
      }

      std::optional<data::event_data> next()
      {
        std::optional<data::event_data> result =
            _queue.empty() ? _events.next() : std::move(_queue.front());
        if (!_queue.empty())
        {
          _queue.pop_front();
        }
        return result;
      }

      template <class InputRange>
      void queue(InputRange& events_)
      {
        _queue.insert(_queue.begin(), events_.begin(), events_.end());
      }

      void queue(data::event_data event_)
      {
        _queue.push_front(std::move(event_));
      }

      data::cpp_code root_name() const { return _events.root_name(); }

      data::metaprogram_mode mode() const { return _events.mode(); }

    private:
      Events _events;
      std::deque<data::event_data> _queue;
    };
  }
}

#endif
