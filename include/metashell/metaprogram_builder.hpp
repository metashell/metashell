#ifndef METASHELL_METAPROGRAM_BUILDER_HPP
#define METASHELL_METAPROGRAM_BUILDER_HPP

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
#include <metashell/data/cpp_code.hpp>
#include <metashell/data/debugger_event.hpp>
#include <metashell/data/event_data.hpp>
#include <metashell/data/frame_stack.hpp>
#include <metashell/data/metaprogram_mode.hpp>

namespace metashell
{
  class metaprogram_builder
  {
  public:
    typedef std::vector<data::debugger_event>::size_type size_type;

    metaprogram_builder(data::metaprogram_mode mode_,
                        data::cpp_code root_name_);

    void push_back(data::event_data event_);

    const data::debugger_event& operator[](size_type n_) const;

    data::backtrace backtrace_at(size_type n_) const;

  private:
    std::vector<data::debugger_event> _events;
    data::frame_stack _frame_stack;
    data::metaprogram_mode _mode;

    void pop_event();
    void push_event(data::event_data event_);
  };
}

#endif
