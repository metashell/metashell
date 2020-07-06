#ifndef METASHELL_DEBUGGER_EVENT_HPP
#define METASHELL_DEBUGGER_EVENT_HPP

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
#include <metashell/data/frame.hpp>
#include <metashell/data/metaprogram_mode.hpp>
#include <metashell/data/pop_frame.hpp>

#include <variant.hpp>

#include <iosfwd>
#include <optional>
#include <string>

namespace metashell
{
  namespace data
  {
    typedef mpark::variant<pop_frame, frame> debugger_event;

    debugger_event to_debugger_event(event_data event_, metaprogram_mode mode_);

    std::string to_string(const debugger_event& event_);
    std::ostream& operator<<(std::ostream& out_, const debugger_event& event_);

    std::optional<double> time_taken(const debugger_event& event_);
    void full_time_taken(debugger_event& event_, double full_time_);
  }
}

#endif
