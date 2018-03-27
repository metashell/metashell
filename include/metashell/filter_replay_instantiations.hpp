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

#include <metashell/data/event_data.hpp>
#include <metashell/data/event_data_sequence.hpp>
#include <metashell/data/file_location.hpp>
#include <metashell/data/type.hpp>

#include <boost/optional.hpp>

#include <deque>
#include <map>
#include <type_traits>
#include <vector>

namespace metashell
{
  template <class Events>
  class filter_replay_instantiations_t
      : public data::event_data_sequence<filter_replay_instantiations_t<Events>>
  {
  public:
    explicit filter_replay_instantiations_t(
        Events&& events_, boost::optional<data::file_location> from_)
      : _events(std::move(events_)), _from(std::move(from_)), _replaying(false)
    {
    }

    boost::optional<data::event_data> next()
    {
      boost::optional<data::event_data> event =
          _queue.empty() ? _events.next() : std::move(_queue.front());

      if (!_queue.empty())
      {
        _queue.pop_front();
      }

      if (_from && event)
      {
        if (!_replaying && from_here(*event))
        {
          _replaying = true;
        }
        mpark::visit(
            [this, &event](auto det) {
              if (this->_replaying)
              {
                event = this->replay_event(det);
              }
              else
              {
                this->record_event(det);
              }
            },
            *event);
      }

      return event;
    }

  private:
    Events _events;
    boost::optional<data::file_location> _from;
    std::deque<data::event_data> _queue;
    bool _replaying;
    std::map<data::type, std::vector<data::event_data>> _recorded;
    std::vector<std::pair<data::type, int>> _recording_to;

    bool from_here(const data::event_data& event_) const
    {
      return !_from || from_line(event_, *_from);
    }

    template <data::event_kind Kind>
    typename std::enable_if<Kind != data::event_kind::template_instantiation &&
                                Kind != data::event_kind::memoization,
                            boost::optional<data::event_data>>::type
    replay_event(data::event_details<Kind> event_)
    {
      return data::event_data(event_);
    }

    template <data::event_kind Kind>
    typename std::enable_if<Kind == data::event_kind::template_instantiation ||
                                Kind == data::event_kind::memoization,
                            boost::optional<data::event_data>>::type
    replay_event(data::event_details<Kind> event_)
    {
      const auto i = _recorded.find(event_.full_name);
      if (i != _recorded.end())
      {
        if (i->second.empty())
        {
          _recorded.erase(i);
        }
        else
        {
          _queue.insert(_queue.begin(), i->second.begin(), i->second.end());
          _recorded.erase(i);
          // The template_end event closing the memoization will become the
          // template_end event closing the simulated template_instantiation
          return data::event_data(
              data::event_details<data::event_kind::template_instantiation>{
                  event_.full_name, event_.point_of_event,
                  event_.source_location, event_.timestamp});
        }
      }
      return data::event_data(event_);
    }

    template <data::event_kind Kind>
    void record_event(const data::event_details<Kind>& event_)
    {
      if (!_recording_to.empty())
      {
        switch (relative_depth_of(event_))
        {
        case data::relative_depth::open:
          ++_recording_to.back().second;
          break;
        case data::relative_depth::flat:
          break;
        case data::relative_depth::close:
        case data::relative_depth::end:
          --_recording_to.back().second;
          break;
        }

        _recorded[_recording_to.back().first].emplace_back(event_);
      }
    }

    void record_event(const data::event_details<
                      data::event_kind::template_instantiation>& event_)
    {
      if (!_recording_to.empty())
      {
        _recorded[_recording_to.back().first].emplace_back(event_);
      }
      _recording_to.push_back(std::make_pair(event_.full_name, 0));
    }

    void record_event(
        const data::event_details<data::event_kind::template_end>& event_)
    {
      if (!_recording_to.empty())
      {
        if (_recording_to.back().second == 0)
        {
          _recording_to.pop_back();
          if (!_recording_to.empty())
          {
            _recorded[_recording_to.back().first].emplace_back(event_);
          }
        }
        else
        {
          _recorded[_recording_to.back().first].emplace_back(event_);
          --_recording_to.back().second;
        }
      }
    }
  };

  template <class Events>
  filter_replay_instantiations_t<Events>
  filter_replay_instantiations(Events&& events_,
                               boost::optional<data::file_location> from_)
  {
    return filter_replay_instantiations_t<Events>(
        std::move(events_), std::move(from_));
  }
}

#endif
