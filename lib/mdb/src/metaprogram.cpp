
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

#include <metashell/mdb/caching_disabled.hpp>
#include <metashell/mdb/metaprogram.hpp>

#include <algorithm>
#include <cassert>

namespace metashell
{
  namespace mdb
  {
    metaprogram::metaprogram(std::unique_ptr<iface::event_data_sequence> trace,
                             bool caching_enabled)
      : event_source(std::move(trace)),
        next_unread_event(event_source->next()),
        current_frame(data::frame(event_source->root_name())),
        current_bt(data::backtrace()),
        mode(event_source->mode()),
        history(caching_enabled ? std::make_optional(
                                      debugger_history(mode, **current_frame)) :
                                  std::nullopt),
        result("Internal Metashell error: metaprogram not finished yet")
    {
      assert(event_source);

      ++tree_depth;
      update(final_bt, **current_frame);
      update(*current_bt, **current_frame);

      if (mode == data::metaprogram_mode::profile)
      {
        read_remaining_events();
      }
    }

    bool metaprogram::is_empty()
    {
      if (read_open_or_flat)
      {
        return false;
      }
      else if (next_unread_event)
      {
        switch (relative_depth_of(*next_unread_event))
        {
        case data::relative_depth::open:
        /* [[fallthrough]] */ case data::relative_depth::flat:
          return false;
        case data::relative_depth::close:
        /* [[fallthrough]] */ case data::relative_depth::end:
          return true;
        }
        assert(!"Invalid realtive depth");
      }
      return true;
    }

    const data::type_or_code_or_error& metaprogram::get_evaluation_result()
    {
      if (is_empty() && next_unread_event)
      {
        if (auto r = result_of(*next_unread_event))
        {
          result = std::move(*r);
          return result;
        }
      }
      read_remaining_events();
      return result;
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

    bool metaprogram::cached_ahead_of(size_type loc) const
    {
      return loc < read_event_count - 1;
    }

    void metaprogram::step()
    {
      assert(!is_finished());

      if (cached_ahead_of(next_event))
      {
        assert(bool(history));

        do
        {
          ++next_event;

          if (current_bt && next_event < read_event_count)
          {
            update(*current_bt, (*history)[next_event]);
          }
        } while (next_event < read_event_count &&
                 mpark::get_if<data::pop_frame>(&(*history)[next_event]));

        if (has_unread_event && next_event >= read_event_count)
        {
          --next_event;
        }
        else
        {
          cache_current_frame();
          return;
        }
      }

      if (!cached_ahead_of(next_event))
      {
        std::optional<data::debugger_event> last = std::nullopt;
        do
        {
          ++next_event;

          try_reading_until(next_event, &last);

          if (last)
          {
            if (current_bt)
            {
              update(*current_bt, *last);
            }
            if (auto* f = mpark::get_if<data::frame>(&*last))
            {
              current_frame = *f;
            }
          }
        } while ((has_unread_event || next_event < read_event_count) && last &&
                 mpark::get_if<data::pop_frame>(&*last));
      }
    }

    void metaprogram::step_back()
    {
      assert(!is_at_start());

      if (history)
      {
        do
        {
          --next_event;
        } while (next_event != 0 &&
                 mpark::get_if<data::pop_frame>(&(*history)[next_event]));
        current_bt = std::nullopt;

        cache_current_frame();
      }
      else
      {
        throw caching_disabled("stepping backwards");
      }
    }

    const data::frame& metaprogram::get_current_frame() const
    {
      assert(!is_at_start());
      assert(!is_finished());
      assert(bool(current_frame));

      return **current_frame;
    }

    const data::backtrace& metaprogram::get_backtrace()
    {
      if (is_finished() && get_evaluation_result().is_error())
      {
        return final_bt;
      }
      else
      {
        if (!current_bt)
        {
          assert(bool(history));
          current_bt = history->backtrace_at(next_event);
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
        assert(bool(history));

        current_frame = data::frame_only_event(
            mpark::get<data::frame>((*history)[next_event]));
      }
    }

    std::optional<data::debugger_event> metaprogram::read_next_event()
    {
      assert(bool(history) || read_event_count <= next_event);

      if (next_unread_event)
      {
        update(tree_depth, *next_unread_event);
        const auto at = timestamp(*next_unread_event);
        const data::relative_depth rdepth =
            relative_depth_of(*next_unread_event);
        if (rdepth == data::relative_depth::open ||
            rdepth == data::relative_depth::flat)
        {
          read_open_or_flat = true;
        }
        if (auto r = result_of(*next_unread_event))
        {
          result = std::move(*r);
        }
        const data::debugger_event de =
            to_debugger_event(std::move(*next_unread_event), mode);
        if (history)
        {
          history->add_event(de, rdepth, at);
          if (next_event <= read_event_count &&
              (!current_frame ||
               current_frame->event() != (*history)[next_event]))
          {
            if (const auto f =
                    mpark::get_if<data::frame>(&(*history)[next_event]))
            {
              current_frame = data::frame_only_event(*f);
            }
          }
        }
        update(final_bt, de);
        ++read_event_count;
        next_unread_event = event_source->next();
        return de;
      }
      else
      {
        has_unread_event = false;
        return std::nullopt;
      }
    }

    bool metaprogram::try_reading_until(
        size_type pos, std::optional<data::debugger_event>* last_event_read_)
    {
      if (last_event_read_)
      {
        *last_event_read_ = std::nullopt;
      }
      while (pos >= read_event_count && has_unread_event)
      {
        if (auto n = read_next_event())
        {
          if (last_event_read_)
          {
            *last_event_read_ = n;
          }
        }
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

    bool metaprogram::caching_enabled() const { return bool(history); }

    metaprogram::iterator::iterator(metaprogram& mp_)
      : mp(&mp_), at(std::nullopt)
    {
    }

    metaprogram::iterator::iterator(metaprogram& mp_,
                                    metaprogram::size_type at_)
      : mp(&mp_), at(at_)
    {
      mp->try_reading_until(at_, nullptr);
    }

    metaprogram::iterator::reference metaprogram::iterator::
    operator[](difference_type n) const
    {
      assert(mp);

      if (at)
      {
        return *iterator(*mp, *at + n);
      }
      else
      {
        mp->read_remaining_events();
        return *iterator(*mp, mp->read_event_count + n);
      }
    }

    metaprogram::iterator::reference metaprogram::iterator::operator*() const
    {
      assert(mp);
      assert(bool(at));

      if (*at == mp->next_event)
      {
        assert(bool(mp->current_frame));
        return mp->current_frame->event();
      }
      else
      {
        assert(bool(mp->history));
        return (*mp->history)[*at];
      }
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

      if (!mp->try_reading_until(*at, nullptr))
      {
        at = std::nullopt;
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
}
