#ifndef METASHELL_MDB_DEBUGGER_HISTORY_HPP
#define METASHELL_MDB_DEBUGGER_HISTORY_HPP

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

#include <metashell/data/backtrace.hpp>
#include <metashell/data/debugger_event.hpp>
#include <metashell/data/frame_stack.hpp>
#include <metashell/data/metaprogram_mode.hpp>

#include <optional>

namespace metashell
{
  namespace mdb
  {
    class debugger_history
    {
    public:
      typedef std::vector<data::debugger_event>::size_type size_type;

      debugger_history(data::metaprogram_mode mode_, data::frame root_frame_);

      void add_event(data::debugger_event event_,
                     data::relative_depth rdepth_,
                     const std::optional<double>& timestamp_);

      const data::debugger_event& operator[](size_type n_) const;

      data::backtrace backtrace_at(size_type n_) const;

    private:
      std::vector<data::debugger_event> _events;
      data::frame_stack _frame_stack;
      data::metaprogram_mode _mode;

      void pop_event();
    };
  }
}

#endif
