#ifndef METASHELL_FILTER_ENABLE_REACHABLE_HPP
#define METASHELL_FILTER_ENABLE_REACHABLE_HPP

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

#include <optional>
#include <vector>

namespace metashell
{
  namespace core
  {
    template <class Events>
    class filter_enable_reachable_t
    {
    public:
      explicit filter_enable_reachable_t(
          Events&& events_, std::optional<data::file_location> from_)
        : _events(std::move(events_)), _from(std::move(from_))
      {
      }

      std::optional<data::event_data> next()
      {
        while (std::optional<data::event_data> event = _events.next())
        {
          const data::event_kind kind = kind_of(*event);

          switch (relative_depth_of(kind))
          {
          case data::relative_depth::open:
            _depth_enabled.push_back(
                enabled(kind) &&
                (_depth_enabled.back() ||
                 (from_here(*event) && !is_remove_ptr(*event) &&
                  (kind != data::event_kind::memoization ||
                   !trim_wrap_type(mpark::get<data::event_details<
                                       data::event_kind::memoization>>(*event)
                                       .what.full_name)))));
            if (_depth_enabled.back())
            {
              return event;
            }
            break;
          case data::relative_depth::flat:
            if (enabled(kind) && (_depth_enabled.back() || from_here(*event)))
            {
              return event;
            }
            break;
          case data::relative_depth::close:
          {
            const bool keep = _depth_enabled.back();
            _depth_enabled.pop_back();
            if (keep)
            {
              return event;
            }
            break;
          }
          case data::relative_depth::end:
            return event;
          }
        }

        return std::nullopt;
      }

      data::cpp_code root_name() const { return _events.root_name(); }

      data::metaprogram_mode mode() const { return _events.mode(); }

    private:
      Events _events;
      std::optional<data::file_location> _from;
      std::vector<bool> _depth_enabled{false};

      bool from_here(const data::event_data& event_) const
      {
        return !_from || from_line(event_, *_from);
      }
    };

    template <class Events>
    filter_enable_reachable_t<Events>
    filter_enable_reachable(Events&& events_,
                            std::optional<data::file_location> from_)
    {
      return filter_enable_reachable_t<Events>(
          std::move(events_), std::move(from_));
    }
  }
}

#endif
