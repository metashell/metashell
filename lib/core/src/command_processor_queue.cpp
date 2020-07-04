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

#include <metashell/core/command_processor_queue.hpp>

#include <cassert>

namespace metashell
{
  namespace core
  {
    bool command_processor_queue::empty() const { return _items.empty(); }

    data::code_completion
    command_processor_queue::code_complete(const data::user_input& s_) const
    {
      return empty() ? data::code_completion{} :
                       _items.back().first->code_complete(s_, true);
    }

    void command_processor_queue::pop(iface::displayer& displayer_)
    {
      assert(!empty());

      if (_items.back().second)
      {
        _items.back().second(displayer_);
      }
      _items.pop_back();
      displayer_.show_raw_text("");
    }

    void command_processor_queue::cancel_operation()
    {
      if (!empty())
      {
        _items.back().first->cancel_operation();
      }
    }

    void command_processor_queue::line_available(const data::user_input& cmd_,
                                                 iface::displayer& displayer_)
    {
      assert(_history != nullptr);

      if (!empty())
      {
        _items.back().first->line_available(cmd_, displayer_, *_history);
      }
    }

    std::string command_processor_queue::prompt() const
    {
      assert(!empty());

      return _items.back().first->prompt();
    }

    void command_processor_queue::pop_stopped_processors(
        iface::displayer& displayer_)
    {
      while (!empty() && _items.back().first->stopped())
      {
        pop(displayer_);
      }
    }

    void command_processor_queue::push(
        std::unique_ptr<iface::command_processor> item_,
        cleanup_function cleanup_)
    {
      _items.push_back({move(item_), move(cleanup_)});
    }

    void command_processor_queue::history(iface::history& history_)
    {
      _history = &history_;
    }
  }
}
