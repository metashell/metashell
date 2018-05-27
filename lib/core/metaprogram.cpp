
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
#include <metashell/metaprogram.hpp>

#include <algorithm>
#include <cassert>

namespace metashell
{
  metaprogram::metaprogram(std::unique_ptr<iface::event_data_sequence> trace)
    : event_source(std::move(trace)),
      current_frame(event_source->root_name()),
      mode(event_source->mode()),
      history(mode, current_frame),
      result("Internal Metashell error: metaprogram not finished yet")
  {
    assert(event_source);

    ++tree_depth;
    update(final_bt, current_frame);

    if (mode == data::metaprogram_mode::profile)
    {
      read_remaining_events();
    }
  }

  bool metaprogram::is_empty()
  {
    while (!read_open_or_flat && has_unread_event)
    {
      read_next_event();
    }
    return !read_open_or_flat;
  }

  const data::type_or_code_or_error& metaprogram::get_evaluation_result()
  {
    read_remaining_events();
    return result;
  }

  void metaprogram::reset_state()
  {
    next_event = 0;
    current_bt = history.backtrace_at(0);
    cache_current_frame();
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

  bool metaprogram::is_finished() const
  {
    return !has_unread_event && next_event >= read_event_count;
  }

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

      if (try_reading_until(next_event) && current_bt)
      {
        update(*current_bt, history[next_event]);
      }
    } while ((has_unread_event || next_event < read_event_count) &&
             mpark::get_if<data::pop_frame>(&history[next_event]));

    cache_current_frame();
  }

  void metaprogram::step_back()
  {
    assert(!is_at_start());
    do
    {
      --next_event;
    } while (next_event != 0 &&
             mpark::get_if<data::pop_frame>(&history[next_event]));
    current_bt = boost::none;

    cache_current_frame();
  }

  const data::frame& metaprogram::get_current_frame() const
  {
    assert(!is_at_start());
    assert(!is_finished());

    return current_frame;
  }

  const data::backtrace& metaprogram::get_backtrace()
  {
    if (!has_unread_event && next_event == read_event_count &&
        get_evaluation_result().is_error())
    {
      return final_bt;
    }
    else
    {
      if (!current_bt)
      {
        current_bt = history.backtrace_at(next_event);
      }
      return *current_bt;
    }
  }

  metaprogram::size_type metaprogram::size()
  {
    read_remaining_events();

    return read_event_count;
  }

  metaprogram::iterator metaprogram::begin(bool include_original_expression)
  {
    return iterator(*this, include_original_expression ? 0 : 1);
  }

  metaprogram::iterator metaprogram::current_position()
  {
    return iterator(*this, next_event);
  }

  metaprogram::iterator metaprogram::end() { return iterator(*this); }

  void metaprogram::cache_current_frame()
  {
    if (next_event < read_event_count)
    {
      current_frame = mpark::get<data::frame>(history[next_event]);
    }
  }

  void metaprogram::read_next_event()
  {
    if (boost::optional<data::event_data> event = event_source->next())
    {
      update(tree_depth, *event);
      const auto at = timestamp(*event);
      const data::relative_depth rdepth = relative_depth_of(*event);
      if (rdepth == data::relative_depth::open ||
          rdepth == data::relative_depth::flat)
      {
        read_open_or_flat = true;
      }
      if (auto r = result_of(*event))
      {
        result = std::move(*r);
      }
      const data::debugger_event de =
          to_debugger_event(std::move(*event), mode);
      history.add_event(de, rdepth, at);
      update(final_bt, de);
      ++read_event_count;
    }
    else
    {
      has_unread_event = false;
    }
  }

  bool metaprogram::try_reading_until(size_type pos)
  {
    while (pos >= read_event_count && has_unread_event)
    {
      read_next_event();
    }
    return pos < read_event_count;
  }

  void metaprogram::read_remaining_events()
  {
    while (has_unread_event)
    {
      read_next_event();
    }
  }

  metaprogram::iterator::iterator(metaprogram& mp_) : mp(&mp_), at(boost::none)
  {
  }

  metaprogram::iterator::iterator(metaprogram& mp_, metaprogram::size_type at_)
    : mp(&mp_), at(at_)
  {
    mp->try_reading_until(at_);
  }

  metaprogram::iterator::reference metaprogram::iterator::
  operator[](difference_type n) const
  {
    assert(mp);

    if (!at)
    {
      mp->read_remaining_events();
    }

    return mp->history[at ? *at : mp->read_event_count + n];
  }

  const data::debugger_event& metaprogram::iterator::operator*() const
  {
    assert(mp);
    assert(bool(at));

    return mp->history[*at];
  }

  metaprogram::iterator& metaprogram::iterator::operator--()
  {
    return *this -= 1;
  }

  metaprogram::iterator& metaprogram::iterator::operator++()
  {
    return *this += 1;
  }

  metaprogram::iterator& metaprogram::iterator::operator-=(difference_type n)
  {
    return *this += -n;
  }

  metaprogram::iterator& metaprogram::iterator::operator+=(difference_type n)
  {
    assert(mp);

    if (at)
    {
      *at += n;
    }
    else
    {
      mp->read_remaining_events();
      at = mp->read_event_count + n;
    }

    if (!mp->try_reading_until(*at))
    {
      at = boost::none;
    }

    return *this;
  }

  bool metaprogram::iterator::operator==(const iterator& mi) const
  {
    assert(mp == mi.mp);
    return at == mi.at;
  }

  bool metaprogram::iterator::operator<(const iterator& mi) const
  {
    assert(mp == mi.mp);
    return at < mi.at;
  }
}
