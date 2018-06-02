#ifndef METASHELL_DATA_FRAME_ONLY_EVENT_HPP
#define METASHELL_DATA_FRAME_ONLY_EVENT_HPP

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

namespace metashell
{
  namespace data
  {
    class frame_only_event
    {
    public:
      frame_only_event(frame frame_);

      const frame& operator*() const;

      const debugger_event& event() const;

      void full_time_taken(double t_);

    private:
      debugger_event _event;
    };
  }
}

#endif
