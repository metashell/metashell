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

#include "counting_event_data_sequence.hpp"

counting_event_data_sequence::counting_event_data_sequence(
    std::vector<metashell::data::event_data> events_,
    metashell::data::metaprogram_mode mode_)
  : _mode(mode_), _events(std::move(events_)), _next_called_times(0)
{
}

std::optional<metashell::data::event_data> counting_event_data_sequence::next()
{
  if (_next_called_times < _events.size())
  {
    return _events[_next_called_times++];
  }
  else
  {
    return std::nullopt;
  }
}

metashell::data::cpp_code counting_event_data_sequence::root_name() const
{
  return metashell::data::cpp_code("root");
}

metashell::data::metaprogram_mode counting_event_data_sequence::mode() const
{
  return _mode;
}

counting_event_data_sequence::size_type
counting_event_data_sequence::next_called_times() const
{
  return _next_called_times;
}
