// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/debugger_history.hpp>
#include <metashell/exception.hpp>

namespace metashell
{
  debugger_history::debugger_history(data::metaprogram_mode mode_,
                                     data::frame root_frame_)
    : _events{std::move(root_frame_)}, _mode(mode_)
  {
    _frame_stack.push_back(0);
  }

  void debugger_history::open_event(data::debugger_event event_,
                                    const boost::optional<double>& timestamp_)
  {
    assert(!_frame_stack.empty());

    flat_event(std::move(event_), timestamp_);
    _frame_stack.push_back(_events.size() - 1);
  }

  void debugger_history::flat_event(data::debugger_event event_,
                                    const boost::optional<double>& timestamp_)
  {
    assert(!_frame_stack.empty());

    running_at(_frame_stack, _events, timestamp_, _mode);

    _frame_stack.back(_events).add_child();
    _events.emplace_back(std::move(event_));
  }

  void debugger_history::close_event(const boost::optional<double>& timestamp_)
  {
    assert(!_frame_stack.empty());

    running_at(_frame_stack, _events, timestamp_, _mode);

    _frame_stack.back(_events).finished();
    _events.emplace_back(_frame_stack.pop_back());
  }

  void debugger_history::end_event(const boost::optional<double>& timestamp_)
  {
    assert(!_frame_stack.empty());

    close_event(timestamp_);

    if (!_frame_stack.empty())
    {
      throw exception("Unclosed opening event: " +
                      to_string(_frame_stack.back(_events)));
    }
    else if (const auto full_time = time_taken(_events.front()))
    {
      for (data::debugger_event& event : _events)
      {
        full_time_taken(event, *full_time);
      }
    }
  }

  const data::debugger_event& debugger_history::
  operator[](debugger_history::size_type n_) const
  {
    return _events[n_];
  }

  data::backtrace debugger_history::backtrace_at(size_type n_) const
  {
    data::backtrace result;
    const auto b = _events.begin();
    for (auto i = b, e = b + std::min(n_ + 1, _events.size()); i != e; ++i)
    {
      update(result, *i);
    }
    return result;
  }
}
