// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/frame_stack.hpp>

#include <cassert>

namespace metashell
{
  namespace data
  {
    bool frame_stack::empty() const { return _stack.empty(); }

    frame& frame_stack::back(std::vector<debugger_event>& events_) const
    {
      assert(!_stack.empty());
      assert(_stack.back() < events_.size());

      frame* f = mpark::get_if<frame>(&events_[_stack.back()]);
      assert(f);
      return *f;
    }

    void frame_stack::push_back(value_type item_)
    {
      _stack.emplace_back(item_);
    }

    void frame_stack::pop_back()
    {
      assert(!_stack.empty());
      _stack.pop_back();
    }

    void frame_stack::running_at(std::vector<debugger_event>& events_,
                                 double timestamp_)
    {
      for (auto i : _stack)
      {
        assert(i < events_.size());
        frame* f = mpark::get_if<frame>(&events_[i]);
        assert(f);
        f->running_at(timestamp_);
      }
    }

    void running_at(frame_stack& stack_,
                    std::vector<debugger_event>& events_,
                    const std::optional<double>& timestamp_,
                    metaprogram_mode mode_)
    {
      if (mode_ == metaprogram_mode::profile && timestamp_)
      {
        stack_.running_at(events_, *timestamp_);
      }
    }
  }
}
