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

#include <metashell/mdb/forward_trace_iterator.hpp>

#include <algorithm>

namespace metashell
{
  namespace mdb
  {
    forward_trace_iterator::forward_trace_iterator() : _at_end(std::nullopt) {}

    forward_trace_iterator::forward_trace_iterator(
        metaprogram::iterator begin_,
        metaprogram::iterator end_,
        const std::optional<int>& max_depth_)
      : _at_end(begin_ == end_ ?
                    std::nullopt :
                    std::make_optional(std::make_pair(begin_, end_))),
        _max_depth(max_depth_)
    {
      cache_current();
    }

    forward_trace_iterator& forward_trace_iterator::operator++()
    {
      assert(!finished());
      do
      {
        step_from(*at());
        ++at();
      } while (at() != end() && step_to(*at()));

      if (at() == end() || _depth <= 0)
      {
        _at_end = std::nullopt;
      }
      else
      {
        cache_current();
      }
      return *this;
    }

    void forward_trace_iterator::step_from(const data::debugger_event& event_)
    {
      mpark::visit([this](const auto& f) { this->step_from(f); }, event_);
    }

    void forward_trace_iterator::step_from(const data::frame& frame_)
    {
      if (!frame_.flat())
      {
        ++_depth;
      }
    }

    void forward_trace_iterator::step_from(const data::pop_frame&) {}

    bool forward_trace_iterator::step_to(const data::debugger_event& event_)
    {
      return mpark::visit(
          [this](const auto& f) -> bool { return this->step_to(f); }, event_);
    }

    bool forward_trace_iterator::step_to(const data::frame&)
    {
      return _max_depth && _depth > *_max_depth;
    }

    bool forward_trace_iterator::step_to(const data::pop_frame&)
    {
      --_depth;
      if (_depth == -1)
      {
        _at_end = std::nullopt;
      }
      return _depth > 0;
    }

    void forward_trace_iterator::cache_current()
    {
      auto p = mpark::get_if<data::frame>(&*at());
      assert(p);

      if (!p->number_of_children() && _at_end)
      {
        for (metaprogram::iterator i = at(), e = end();
             i != e && !p->number_of_children();)
        {
          ++i;
          p = mpark::get_if<data::frame>(&*at());
          assert(p);
        }
      }

      const auto number_of_children = p->number_of_children();

      _current = data::call_graph_node(
          *p, _depth,
          ((_max_depth && _depth == _max_depth) || !number_of_children) ?
              0 :
              *number_of_children);
    }

    const data::call_graph_node& forward_trace_iterator::operator*() const
    {
      assert(!finished());
      assert(mpark::get_if<data::frame>(&*at()) != nullptr);

      return _current;
    }

    bool forward_trace_iterator::
    operator==(const forward_trace_iterator& i_) const
    {
      return finished() == i_.finished();
    }

    bool forward_trace_iterator::finished() const { return !_at_end; }

    const metaprogram::iterator& forward_trace_iterator::at() const
    {
      assert(_at_end);
      return _at_end->first;
    }

    metaprogram::iterator& forward_trace_iterator::at()
    {
      assert(_at_end);
      return _at_end->first;
    }

    const metaprogram::iterator& forward_trace_iterator::end() const
    {
      assert(_at_end);
      return _at_end->second;
    }
  }
}
