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

#include <metashell/data/in_memory_event_data_sequence.hpp>

namespace metashell
{
  namespace data
  {
    in_memory_event_data_sequence::in_memory_event_data_sequence(
        cpp_code root_name_,
        metaprogram_mode mode_,
        std::vector<event_data> events_)
      : _events(std::move(events_)),
        _next(0),
        _root_name(std::move(root_name_)),
        _mode(mode_)
    {
    }

    std::optional<event_data> in_memory_event_data_sequence::next()
    {
      return _next < _events.size() ? std::make_optional(_events[_next++]) :
                                      std::nullopt;
    }

    const cpp_code& in_memory_event_data_sequence::root_name() const
    {
      return _root_name;
    }

    metaprogram_mode in_memory_event_data_sequence::mode() const
    {
      return _mode;
    }
  }
}
