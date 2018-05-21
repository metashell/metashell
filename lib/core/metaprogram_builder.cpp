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

#include <metashell/exception.hpp>
#include <metashell/metaprogram_builder.hpp>

namespace metashell
{
  metaprogram_builder::metaprogram_builder(data::metaprogram_mode mode_,
                                           data::cpp_code root_name_)
    : _events{data::frame(std::move(root_name_))}, _mode(mode_)
  {
    _frame_stack.push_back(0);
  }

  void metaprogram_builder::push_back(data::event_data event_)
  {
    assert(!_frame_stack.empty());

    running_at(_frame_stack, _events, timestamp(event_), _mode);

    switch (relative_depth_of(event_))
    {
    case data::relative_depth::open:
      push_event(std::move(event_));
      _frame_stack.push_back(_events.size() - 1);
      break;
    case data::relative_depth::flat:
      push_event(std::move(event_));
      break;
    case data::relative_depth::close:
      pop_event();
      if (_frame_stack.empty())
      {
        throw exception("Unpaired closing event: " + to_string(event_));
      }
      break;
    case data::relative_depth::end:
      pop_event();

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
      break;
    }
  }

  void metaprogram_builder::pop_event()
  {
    _frame_stack.back(_events).finished();
    _events.emplace_back(_frame_stack.pop_back());
  }

  void metaprogram_builder::push_event(data::event_data event_)
  {
    _frame_stack.back(_events).add_child();

    _events.emplace_back(data::frame(std::move(event_), _mode));
  }

  const data::debugger_event& metaprogram_builder::
  operator[](metaprogram_builder::size_type n_) const
  {
    return _events[n_];
  }
}
