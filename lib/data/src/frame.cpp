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

#include <metashell/data/frame.hpp>

#include <cassert>
#include <ostream>
#include <sstream>
#include <stdexcept>

namespace metashell
{
  namespace data
  {
    namespace
    {
      file_location source_location_of(const event_data& event_)
      {
        if (std::optional<file_location> l = source_location(event_))
        {
          return *l;
        }
        else
        {
          throw std::runtime_error("Event " + to_string(event_) +
                                   " has no source location information.");
        }
      }

      file_location point_of_event_of(const event_data& event_)
      {
        if (std::optional<file_location> p = point_of_event(event_))
        {
          return *p;
        }
        else
        {
          throw std::runtime_error("Event " + to_string(event_) +
                                   " has no point of event information.");
        }
      }
    }

    frame::frame(metaprogram_node node_) : _node(std::move(node_)), _flat(false)
    {
    }

    frame::frame(bool flat_,
                 std::optional<double> running_at_,
                 const metaprogram_node& node_,
                 const file_location& source_location_)
      : _node(node_),
        _source_location(source_location_),
        _started_at(std::move(running_at_)),
        _finished_at(_started_at),
        _flat(flat_)
    {
    }

    frame::frame(bool flat_,
                 std::optional<double> running_at_,
                 const metaprogram_node& node_,
                 const file_location& source_location_,
                 const file_location& point_of_event_,
                 event_kind kind_)
      : _node(node_),
        _source_location(source_location_),
        _point_of_event(point_of_event_),
        _kind(kind_),
        _started_at(std::move(running_at_)),
        _finished_at(_started_at),
        _flat(flat_)
    {
    }

    frame::frame(const event_data& event_, metaprogram_mode mode_)
      : _node(to_metaprogram_node(name(event_))),
        _source_location(source_location_of(event_)),
        _point_of_event((mode_ == metaprogram_mode::normal ||
                         mode_ == metaprogram_mode::profile) ?
                            std::make_optional(point_of_event_of(event_)) :
                            std::nullopt),
        _kind((mode_ == metaprogram_mode::normal ||
               mode_ == metaprogram_mode::profile) ?
                  std::make_optional(kind_of(event_)) :
                  std::nullopt),
        _started_at(mode_ == metaprogram_mode::profile ? timestamp(event_) :
                                                         std::nullopt),
        _finished_at(_started_at),
        _flat(relative_depth_of(event_) == relative_depth::flat)
    {
    }

    const metaprogram_node& frame::node() const { return _node; }

    const file_location& frame::source_location() const
    {
      return _source_location;
    }

    bool frame::is_full() const
    {
      assert(bool(_kind) == bool(_point_of_event));
      return bool(_kind);
    }

    event_kind frame::kind() const
    {
      assert(is_full());
      return *_kind;
    }

    const file_location& frame::point_of_event() const
    {
      assert(is_full());
      return *_point_of_event;
    }

    std::optional<double> frame::time_taken() const
    {
      return (_finished_at && _started_at) ?
                 std::make_optional(*_finished_at - *_started_at) :
                 std::nullopt;
    }

    std::optional<double> frame::time_taken_ratio() const
    {
      return _time_taken_ratio;
    }

    bool frame::flat() const { return _flat; }

    void frame::add_child() { ++_number_of_children; }

    std::optional<int> frame::number_of_children() const
    {
      return *_number_of_children;
    }

    void frame::running_at(double t)
    {
      assert(!finalised());
      _started_at = _started_at ? std::min(*_started_at, t) : t;
      _finished_at = _finished_at ? std::max(*_finished_at, t) : t;
    }

    void frame::full_time_taken(double t)
    {
      if (const auto taken = time_taken())
      {
        if (t <= 0.0)
        {
          _time_taken_ratio = 1.0;
        }
        else
        {
          _time_taken_ratio = *taken / t;
        }
      }
    }

    void frame::finished()
    {
      assert(!finalised());
      _number_of_children.value_is_final();
    }

    bool frame::finalised() const { return bool(*_number_of_children); }

    std::string to_string(const frame& f_)
    {
      std::ostringstream s;
      s << f_;
      return s.str();
    }

    std::ostream& operator<<(std::ostream& o_, const frame& f_)
    {
      o_ << "frame(" << (f_.flat() ? "flat" : "not flat") << ", \"" << f_.node()
         << "\"";
      if (f_.is_full())
      {
        o_ << ", " << f_.point_of_event() << ", " << f_.kind();
      }
      if (const auto t = f_.time_taken())
      {
        o_ << ", " << *t << "s";
      }
      o_ << ")";
      return o_;
    }

    bool operator==(const frame& a_, const frame& b_)
    {
      return a_.flat() == b_.flat() && a_.node() == b_.node() &&
             a_.is_full() == b_.is_full() &&
             (!a_.is_full() || (a_.kind() == b_.kind() &&
                                a_.point_of_event() == b_.point_of_event())) &&
             a_.time_taken() == b_.time_taken() &&
             a_.time_taken_ratio() == b_.time_taken_ratio() &&
             a_.number_of_children() == b_.number_of_children();
    }
  }
} // namespace metashell::data
