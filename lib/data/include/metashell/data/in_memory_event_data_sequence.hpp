#ifndef METASHELL_DATA_IN_MEMORY_EVENT_DATA_SEQUENCE_HPP
#define METASHELL_DATA_IN_MEMORY_EVENT_DATA_SEQUENCE_HPP

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
#include <vector>

namespace metashell
{
  namespace data
  {
    class in_memory_event_data_sequence
    {
    public:
      in_memory_event_data_sequence(cpp_code root_name_,
                                    metaprogram_mode mode_,
                                    std::vector<event_data> events_);

      std::optional<event_data> next();

      const cpp_code& root_name() const;

      metaprogram_mode mode() const;

    private:
      std::vector<event_data> _events;
      std::vector<event_data>::size_type _next;
      cpp_code _root_name;
      metaprogram_mode _mode;
    };
  }
}

#endif
