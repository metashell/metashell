#ifndef METASHELL_FILTER_REPEATED_MEMOIZATION_HPP
#define METASHELL_FILTER_REPEATED_MEMOIZATION_HPP

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

#include <metashell/exception.hpp>

#include <boost/optional.hpp>

#include <set>
#include <vector>

namespace metashell
{
  template <class Events>
  class filter_repeated_memoization_t
      : public data::event_data_sequence<filter_repeated_memoization_t<Events>>
  {
  public:
    filter_repeated_memoization_t(bool keep_memoization_after_instantiation_,
                                  Events&& events_)
      : _events(std::move(events_)),
        _keep_memoization_after_instantiation(
            keep_memoization_after_instantiation_)
    {
    }

    boost::optional<data::event_data> next()
    {
      while (boost::optional<data::event_data> event = _events.next())
      {
        const auto inst = _instantiations.size() - 1;

        switch (relative_depth_of(*event))
        {
        case data::relative_depth::open:
          _instantiations.push_back({});
          break;
        case data::relative_depth::flat:
          break;
        case data::relative_depth::close:
        case data::relative_depth::end:
          _instantiations.pop_back();
          break;
        }

        switch (kind_of(*event))
        {
        case data::event_kind::template_instantiation:
          if (!_keep_memoization_after_instantiation)
          {
            _instantiations[inst].insert(
                mpark::get<data::event_details<
                    data::event_kind::template_instantiation>>(*event)
                    .what);
          }
          return event;
        case data::event_kind::memoization:
        {
          auto mem_as_inst = data::timeless_event_details<
              data::event_kind::template_instantiation>(
              mpark::get<data::event_details<data::event_kind::memoization>>(
                  *event)
                  .what);

          if (_instantiations[inst].find(mem_as_inst) ==
              _instantiations[inst].end())
          {
            _instantiations[inst].insert(std::move(mem_as_inst));
            return event;
          }
          else
          {
            event = _events.next();
            if (event && kind_of(*event) == data::event_kind::template_end)
            {
              _instantiations.pop_back();
            }
            else
            {
              throw exception(
                  "Missing template end event after memoization event.");
            }
          }
        }
        break;
        default:
          return event;
        }
      }

      return boost::none;
    }

  private:
    Events _events;
    bool _keep_memoization_after_instantiation;
    std::vector<std::set<
        data::timeless_event_details<data::event_kind::template_instantiation>>>
        _instantiations{{}};
  };

  template <class Events>
  filter_repeated_memoization_t<Events>
  filter_repeated_memoization(bool keep_memoization_after_instantiation_,
                              Events&& events_)
  {
    return filter_repeated_memoization_t<Events>(
        keep_memoization_after_instantiation_, std::move(events_));
  }
}

#endif
