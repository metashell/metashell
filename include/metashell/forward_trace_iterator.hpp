#ifndef METASHELL_FORWARD_TRACE_ITERATOR_HPP
#define METASHELL_FORWARD_TRACE_ITERATOR_HPP

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

#include <metashell/data/call_graph_node.hpp>
#include <metashell/data/frame.hpp>
#include <metashell/data/pop_frame.hpp>
#include <metashell/metaprogram.hpp>

#include <boost/operators.hpp>
#include <boost/optional.hpp>

namespace metashell
{
  class forward_trace_iterator
      : public boost::forward_iterator_helper<forward_trace_iterator,
                                              const data::call_graph_node>
  {
  public:
    forward_trace_iterator();

    forward_trace_iterator(metaprogram::iterator begin_,
                           metaprogram::iterator end_,
                           const boost::optional<int>& max_depth_);

    forward_trace_iterator& operator++();

    bool operator==(const forward_trace_iterator& i_) const;

    const data::call_graph_node& operator*() const;

  private:
    metaprogram::iterator _at;
    metaprogram::iterator _end;
    data::call_graph_node _current;
    bool _finished;

    boost::optional<int> _max_depth;
    int _depth = 0;

    void cache_current();

    void step_from(const data::debugger_event&);
    void step_from(const data::frame&);
    void step_from(const data::pop_frame&);

    bool step_to(const data::debugger_event&);
    bool step_to(const data::frame& frame_);
    bool step_to(const data::pop_frame& frame_);
  };
}

#endif
