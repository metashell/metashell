#ifndef METASHELL_FILTER_UNWRAP_VERTICES_HPP
#define METASHELL_FILTER_UNWRAP_VERTICES_HPP

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

#include <metashell/core/type.hpp>

#include <optional>

namespace metashell
{
  namespace core
  {
    template <class Events>
    class filter_unwrap_vertices_t
    {
    public:
      explicit filter_unwrap_vertices_t(Events&& events_)
        : _events(std::move(events_))
      {
      }

      std::optional<data::event_data> next()
      {
        std::optional<data::event_data> event = _events.next();

        if (event)
        {
          if (const std::optional<data::type> type = type_of(*event))
          {
            if (const auto t = trim_wrap_type(*type))
            {
              if (is_template_type(*t))
              {
                set_type(*event, *t);
              }
              else
              {
                // All of the below optionals are expected to hold a value
                event =
                    data::event_details<data::event_kind::non_template_type>{
                        {*t, *point_of_event(*event), *source_location(*event)},
                        *timestamp(*event)};
              }
            }
          }
        }

        return event;
      }

      data::cpp_code root_name() const { return _events.root_name(); }

      data::metaprogram_mode mode() const { return _events.mode(); }

    private:
      Events _events;
    };

    template <class Events>
    filter_unwrap_vertices_t<Events> filter_unwrap_vertices(Events&& events_)
    {
      return filter_unwrap_vertices_t<Events>(std::move(events_));
    }
  }
}

#endif
