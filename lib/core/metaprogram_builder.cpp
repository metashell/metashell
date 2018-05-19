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
  metaprogram_builder::metaprogram_builder(
      std::vector<data::debugger_event>& events_,
      data::backtrace& final_bt_,
      data::metaprogram_mode mode_,
      data::cpp_code root_name_)
    : _events(&events_),
      _final_bt(&final_bt_),
      _frame_stack(),
      _final_bt_pop(final_bt_),
      _mode(mode_),
      _result("Internal Metashell error: metaprogram not finished yet")
  {
    {
      data::frame f{std::move(root_name_)};
      _events->push_back(f);
      _final_bt->push_front(std::move(f));
    }

    _frame_stack.push_back(back_of(*_events));
  }

  void metaprogram_builder::push_back(data::event_data event_)
  {
    assert(!_frame_stack.empty());

    running_at(_frame_stack, timestamp(event_), _mode);

    switch (relative_depth_of(event_))
    {
    case data::relative_depth::open:
      push_event(std::move(event_));
      _frame_stack.push_back(back_of(*_events));
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

      if (_frame_stack.empty())
      {
        const auto r = result_of(event_);
        assert(bool(r));
        _result = *r;

        if (const auto full_time = time_taken(_events->front()))
        {
          for (data::debugger_event& event : *_events)
          {
            full_time_taken(event, *full_time);
          }
        }
      }
      else
      {
        throw exception("Unclosed opening event: " +
                        to_string(_frame_stack.back()));
      }
      break;
    }
  }

  void metaprogram_builder::pop_event()
  {
    _events->emplace_back(_frame_stack.pop_back());
    _final_bt_pop.buffer_pop_front();
  }

  void metaprogram_builder::push_event(data::event_data event_)
  {
    _frame_stack.back().add_child();
    _final_bt_pop.flush();

    data::frame f(std::move(event_), _mode);
    _final_bt->push_front(f);
    _events->emplace_back(std::move(f));
  }

  const data::type_or_code_or_error& metaprogram_builder::result() const
  {
    return _result;
  }
}
