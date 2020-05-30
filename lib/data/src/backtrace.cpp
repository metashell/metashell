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

#include <metashell/data/backtrace.hpp>

#include <boost/range/algorithm/equal.hpp>

#include <algorithm>
#include <cassert>

namespace metashell
{
  namespace data
  {
    backtrace::backtrace(bool buffered_)
      : _frames(),
        _buffered_pop_count(buffered_ ? std::make_optional(0) : std::nullopt)
    {
    }

    backtrace::backtrace(std::initializer_list<frame> frames_)
      : _frames(std::move(frames_)), _buffered_pop_count(std::nullopt)
    {
      std::reverse(_frames.begin(), _frames.end());
    }

    void backtrace::push_front(const frame& f_)
    {
      flush();
      _frames.push_back(f_);
    }

    void backtrace::pop_front()
    {
      if (_buffered_pop_count)
      {
        ++*_buffered_pop_count;
      }
      else
      {
        _frames.pop_back();
      }
    }

    bool backtrace::empty() const { return _frames.empty(); }

    backtrace::size_type backtrace::size() const { return _frames.size(); }

    const frame& backtrace::operator[](size_type i) const
    {
      return _frames[_frames.size() - 1 - i];
    }

    backtrace::iterator backtrace::begin() const { return _frames.rbegin(); }

    backtrace::iterator backtrace::end() const { return _frames.rend(); }

    void backtrace::flush()
    {
      if (_buffered_pop_count)
      {
        assert(*_buffered_pop_count >= 0);
        assert(static_cast<unsigned int>(*_buffered_pop_count) <=
               _frames.size());
        const auto e = _frames.end();
        _frames.erase(e - *_buffered_pop_count, e);
        _buffered_pop_count = 0;
      }
    }

    int backtrace::buffered_pop_count() const
    {
      return _buffered_pop_count.value_or(0);
    }

    bool backtrace::flushed_empty() const
    {
      return (_frames.size() - buffered_pop_count()) <= 0;
    }

    const frame& backtrace::flushed_front() const
    {
      return _frames[_frames.size() - 1 - buffered_pop_count()];
    }

    void backtrace::pop_flat()
    {
      if (!flushed_empty() && flushed_front().flat())
      {
        pop_front();
      }
      assert(flushed_empty() || !flushed_front().flat());
    }

    void backtrace::update(const frame& event_)
    {
      pop_flat();
      push_front(event_);
    }

    void backtrace::update(const pop_frame&)
    {
      assert(!flushed_empty());

      pop_flat();
      pop_front();
    }

    std::ostream& operator<<(std::ostream& o_, const backtrace& t_)
    {
      o_ << "backtrace{";
      bool first = true;
      for (const frame& f : t_)
      {
        if (first)
        {
          first = false;
        }
        else
        {
          o_ << ", ";
        }
        o_ << f;
      }
      return o_ << "}";
    }

    bool operator==(const backtrace& a_, const backtrace& b_)
    {
      return boost::equal(a_, b_);
    }

    void update(backtrace& bt_, const debugger_event& event_)
    {
      mpark::visit([&bt_](const auto& e) { bt_.update(e); }, event_);
    }
  }
}
