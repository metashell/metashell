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

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/event_data.hpp>
#include <metashell/data/exception.hpp>
#include <metashell/data/metaprogram_mode.hpp>

#include <optional>
#include <set>
#include <vector>

namespace metashell
{
  namespace core
  {
    template <class Events>
    class filter_repeated_memoization_t
    {
    public:
      explicit filter_repeated_memoization_t(Events&& events_)
        : _events(std::move(events_))
      {
      }

      std::optional<data::event_data> next()
      {
        while (std::optional<data::event_data> event = _events.next())
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
            if (_events.mode() == data::metaprogram_mode::full)
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
                throw data::exception(
                    "Missing template end event after memoization event.");
              }
            }
          }
          break;
          default:
            return event;
          }
        }

        return std::nullopt;
      }

      data::cpp_code root_name() const { return _events.root_name(); }

      data::metaprogram_mode mode() const { return _events.mode(); }

    private:
      Events _events;
      std::vector<std::set<data::timeless_event_details<
          data::event_kind::template_instantiation>>>
          _instantiations{{}};
    };

    template <class Events>
    filter_repeated_memoization_t<Events>
    filter_repeated_memoization(Events&& events_)
    {
      return filter_repeated_memoization_t<Events>(std::move(events_));
    }
  }
}

#endif
