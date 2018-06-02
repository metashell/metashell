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

#include <metashell/data/frame_only_event.hpp>

#include <cassert>

namespace metashell
{
  namespace data
  {
    frame_only_event::frame_only_event(frame frame_) : _event(std::move(frame_))
    {
    }

    const frame& frame_only_event::operator*() const
    {
      const auto p = mpark::get_if<frame>(&_event);
      assert(p);
      return *p;
    }

    const debugger_event& frame_only_event::event() const { return _event; }

    void frame_only_event::full_time_taken(double t_)
    {
      data::full_time_taken(_event, t_);
    }
  }
}
