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

#include <metashell/data/debugger_event.hpp>

#include <cassert>
#include <iostream>

namespace metashell
{
  namespace data
  {
    namespace impl
    {
      namespace
      {
        void full_time_taken(frame& f_, double full_time_)
        {
          f_.full_time_taken(full_time_);
        }

        void full_time_taken(pop_frame&, double) {}

        std::optional<double> time_taken(const frame& f_)
        {
          return f_.time_taken();
        }

        std::nullopt_t time_taken(const pop_frame&) { return std::nullopt; }
      }
    }

    debugger_event to_debugger_event(event_data event_, metaprogram_mode mode_)
    {
      switch (relative_depth_of(event_))
      {
      case relative_depth::open:
      /* [[fallthrough]] */ case relative_depth::flat:
        return frame(std::move(event_), mode_);
      case relative_depth::close:
      /* [[fallthrough]] */ case relative_depth::end:
        return pop_frame();
      }
      assert(!"Invalid relative depth");
      return debugger_event();
    }

    std::string to_string(const debugger_event& event_)
    {
      return mpark::visit([](const auto& e) { return to_string(e); }, event_);
    }

    std::ostream& operator<<(std::ostream& out_, const debugger_event& event_)
    {
      mpark::visit([&out_](const auto& e) { out_ << e; }, event_);
      return out_;
    }

    std::optional<double> time_taken(const debugger_event& event_)
    {
      return mpark::visit(
          [](const auto& f) -> std::optional<double> {
            return impl::time_taken(f);
          },
          event_);
    }

    void full_time_taken(debugger_event& event_, double full_time_)
    {
      mpark::visit(
          [full_time_](auto& f) { impl::full_time_taken(f, full_time_); },
          event_);
    }
  }
}
