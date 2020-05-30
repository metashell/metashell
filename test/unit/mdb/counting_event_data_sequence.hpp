#ifndef METASHELL_TEST_COUNTING_EVENT_DATA_SEQUENCE_HPP
#define METASHELL_TEST_COUNTING_EVENT_DATA_SEQUENCE_HPP

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

#include <metashell/iface/event_data_sequence.hpp>

#include <vector>

class counting_event_data_sequence
    : public metashell::iface::event_data_sequence
{
public:
  typedef std::vector<metashell::data::event_data>::size_type size_type;

  explicit counting_event_data_sequence(
      std::vector<metashell::data::event_data> events_,
      metashell::data::metaprogram_mode mode_ =
          metashell::data::metaprogram_mode::normal);

  virtual std::optional<metashell::data::event_data> next() override;

  virtual metashell::data::cpp_code root_name() const override;

  virtual metashell::data::metaprogram_mode mode() const override;

  size_type next_called_times() const;

private:
  metashell::data::metaprogram_mode _mode;
  std::vector<metashell::data::event_data> _events;
  size_type _next_called_times;
};

#endif
