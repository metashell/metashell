#ifndef METASHELL_IFACE_EVENT_DATA_SEQUENCE_HPP
#define METASHELL_IFACE_EVENT_DATA_SEQUENCE_HPP

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

#include <optional>

namespace metashell
{
  namespace iface
  {
    class event_data_sequence
    {
    public:
      virtual ~event_data_sequence() {}

      virtual std::optional<data::event_data> next() = 0;

      virtual data::cpp_code root_name() const = 0;

      virtual data::metaprogram_mode mode() const = 0;
    };
  }
}

#endif
