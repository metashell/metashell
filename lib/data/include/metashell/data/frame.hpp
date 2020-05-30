#ifndef METASHELL_FRAME_HPP
#define METASHELL_FRAME_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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
#include <metashell/data/event_kind.hpp>
#include <metashell/data/file_location.hpp>
#include <metashell/data/finalisable_counter.hpp>
#include <metashell/data/metaprogram_mode.hpp>
#include <metashell/data/metaprogram_node.hpp>

#include <boost/operators.hpp>

#include <optional>

namespace metashell
{
  namespace data
  {
    class frame : boost::equality_comparable<frame>
    {
    public:
      frame() = default;

      explicit frame(metaprogram_node node_);

      frame(bool flat_,
            std::optional<double> running_at_,
            const metaprogram_node& node_,
            const file_location& source_location_);

      frame(bool flat_,
            std::optional<double> running_at_,
            const metaprogram_node& node_,
            const file_location& source_location_,
            const file_location& point_of_event_,
            event_kind kind_);

      frame(const event_data& event_, metaprogram_mode mode_);

      const data::metaprogram_node& node() const;
      const file_location& source_location() const;

      bool is_full() const;

      // precondition: is_full()
      event_kind kind() const;
      const file_location& point_of_event() const;

      std::optional<double> time_taken() const;
      std::optional<double> time_taken_ratio() const;

      void running_at(double t);
      void full_time_taken(double t);

      bool flat() const;

      void add_child();

      std::optional<int> number_of_children() const;

      void finished();

    private:
      metaprogram_node _node;
      file_location _source_location;
      std::optional<file_location> _point_of_event;
      std::optional<data::event_kind> _kind;
      std::optional<double> _started_at;
      std::optional<double> _finished_at;
      std::optional<double> _time_taken_ratio;
      bool _flat;
      finalisable_counter _number_of_children;

      bool finalised() const;
    };

    std::string to_string(const frame& f_);

    std::ostream& operator<<(std::ostream& o_, const frame& f_);
    bool operator==(const frame& a_, const frame& b_);
  }
}

#endif
