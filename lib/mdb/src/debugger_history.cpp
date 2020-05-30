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

#include <metashell/mdb/debugger_history.hpp>

namespace metashell
{
  namespace mdb
  {
    debugger_history::debugger_history(data::metaprogram_mode mode_,
                                       data::frame root_frame_)
      : _events{std::move(root_frame_)}, _mode(mode_)
    {
      _frame_stack.push_back(0);
    }

    void debugger_history::add_event(data::debugger_event event_,
                                     data::relative_depth rdepth_,
                                     const std::optional<double>& timestamp_)
    {
      assert(!_frame_stack.empty());

      running_at(_frame_stack, _events, timestamp_, _mode);

      _events.emplace_back(std::move(event_));

      switch (rdepth_)
      {
      case data::relative_depth::open:
        _frame_stack.back(_events).add_child();
        _frame_stack.push_back(_events.size() - 1);
        break;
      case data::relative_depth::flat:
        _frame_stack.back(_events).add_child();
        break;
      case data::relative_depth::close:
        pop_event();
        break;
      case data::relative_depth::end:
        pop_event();
        while (!_frame_stack.empty())
        {
          pop_event();
        }
        if (const auto full_time = time_taken(_events.front()))
        {
          for (data::debugger_event& event : _events)
          {
            full_time_taken(event, *full_time);
          }
        }
        break;
      }
    }

    void debugger_history::pop_event()
    {
      _frame_stack.back(_events).finished();
      _frame_stack.pop_back();
    }

    const data::debugger_event& debugger_history::
    operator[](debugger_history::size_type n_) const
    {
      assert(n_ < _events.size());
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
}
