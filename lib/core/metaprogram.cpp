
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

#include <metashell/metaprogram.hpp>

#include <algorithm>
#include <cassert>

namespace metashell
{
  bool metaprogram::is_empty() const { return events.size() <= 2; }

  const data::type_or_code_or_error& metaprogram::get_evaluation_result() const
  {
    return builder.result();
  }

  void metaprogram::reset_state()
  {
    next_event = 0;
    rebuild_backtrace();
  }

  data::metaprogram_mode metaprogram::get_mode() const { return mode; }

  bool metaprogram::is_at_endpoint(data::direction_t direction) const
  {
    switch (direction)
    {
    case data::direction_t::forward:
      return is_finished();
    case data::direction_t::backwards:
      return is_at_start();
    default:
      assert(false);
      return true;
    }
  }

  bool metaprogram::is_finished() const { return next_event >= events.size(); }

  bool metaprogram::is_at_start() const { return next_event == 0; }

  void metaprogram::step(data::direction_t direction)
  {
    switch (direction)
    {
    case data::direction_t::forward:
      step();
      break;
    case data::direction_t::backwards:
      step_back();
      break;
    default:
      assert(false);
    }
  }

  void metaprogram::step()
  {
    assert(!is_finished());
    do
    {
      ++next_event;
      if (next_event != events.size())
      {
        update_backtrace(events[next_event]);
      }
    } while (next_event != events.size() &&
             mpark::get_if<data::pop_frame>(&events[next_event]));
  }

  void metaprogram::step_back()
  {
    assert(!is_at_start());
    do
    {
      --next_event;
    } while (next_event != 0 &&
             mpark::get_if<data::pop_frame>(&events[next_event]));
    current_bt = boost::none;
  }

  data::frame metaprogram::get_current_frame() const
  {
    assert(!is_at_start());
    assert(!is_finished());
    assert(bool(mpark::get_if<data::frame>(&events[next_event])));

    return mpark::get<data::frame>(events[next_event]);
  }

  data::frame metaprogram::get_root_frame() const
  {
    assert(bool(mpark::get_if<data::frame>(&events.front())));
    return mpark::get<data::frame>(events.front());
  }

  void metaprogram::rebuild_backtrace()
  {
    current_bt = data::backtrace();
    const auto b = begin();
    for (auto i = b, e = b + std::min(next_event + 1, size()); i != e; ++i)
    {
      update_backtrace(*i);
    }
  }

  void metaprogram::update_backtrace(const data::debugger_event& event)
  {
    if (current_bt)
    {
      if (!current_bt->empty() && current_bt->back().flat())
      {
        current_bt->pop_front();
      }
      assert(current_bt->empty() || !current_bt->back().flat());

      mpark::visit([this](const auto& e) { this->update_backtrace(e); }, event);
    }
  }

  void metaprogram::update_backtrace(const data::frame& event)
  {
    current_bt->push_front(event);
  }

  void metaprogram::update_backtrace(const data::pop_frame& event)
  {
    assert(!current_bt->empty());
    assert(event.target < events.size());

    current_bt->pop_front();
  }

  const data::backtrace& metaprogram::get_backtrace()
  {
    if (next_event == events.size() && get_evaluation_result().is_error())
    {
      return final_bt;
    }
    else
    {
      if (!current_bt)
      {
        rebuild_backtrace();
      }
      assert(bool(current_bt));
      return *current_bt;
    }
  }

  std::size_t metaprogram::size() const { return events.size(); }

  metaprogram::iterator
  metaprogram::begin(bool include_original_expression) const
  {
    metaprogram::iterator result = events.begin();
    if (!include_original_expression && result != events.end())
    {
      ++result;
    }
    return result;
  }

  metaprogram::iterator metaprogram::current_position() const
  {
    return events.begin() + next_event;
  }

  metaprogram::iterator metaprogram::end() const { return events.end(); }
}
