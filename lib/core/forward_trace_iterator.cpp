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

#include <metashell/forward_trace_iterator.hpp>

#include <algorithm>

using namespace metashell;

forward_trace_iterator::forward_trace_iterator() : _finished(true) {}

forward_trace_iterator::forward_trace_iterator(
    metaprogram::iterator begin_,
    metaprogram::iterator end_,
    const boost::optional<int>& max_depth_)
  : _at(begin_), _end(end_), _finished(_at == _end), _max_depth(max_depth_)
{
  cache_current();
}

forward_trace_iterator& forward_trace_iterator::operator++()
{
  assert(!_finished);
  do
  {
    step_from(*_at);
    ++_at;
  } while (_at != _end && step_to(*_at));

  if (_at == _end || _depth <= 0)
  {
    _finished = true;
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
    _at = _end;
  }
  return _depth > 0;
}

void forward_trace_iterator::cache_current()
{
  const auto p = mpark::get_if<data::frame>(&*_at);
  assert(p);
  _current = data::call_graph_node(
      *p, _depth,
      (_max_depth && _depth == _max_depth) ? 0 : p->number_of_children());
}

const data::call_graph_node& forward_trace_iterator::operator*() const
{
  assert(!_finished);
  assert(mpark::get_if<data::frame>(&*_at) != nullptr);

  return _current;
}

bool forward_trace_iterator::operator==(const forward_trace_iterator& i_) const
{
  return _finished == i_._finished;
}
