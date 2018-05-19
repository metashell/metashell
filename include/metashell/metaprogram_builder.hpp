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
#include <metashell/data/buffered_pop_front.hpp>
#include <metashell/data/cpp_code.hpp>
#include <metashell/data/debugger_event.hpp>
#include <metashell/data/event_data.hpp>
#include <metashell/data/frame_stack.hpp>
#include <metashell/data/metaprogram_mode.hpp>
#include <metashell/data/type_or_code_or_error.hpp>

namespace metashell
{
  class metaprogram_builder
  {
  public:
    typedef data::event_data value_type;

    metaprogram_builder(std::vector<data::debugger_event>& events_,
                        data::backtrace& final_bt_,
                        data::metaprogram_mode mode_,
                        data::cpp_code root_name_);

    void push_back(data::event_data event_);

    const data::type_or_code_or_error& result() const;

  private:
    std::vector<data::debugger_event>* _events;
    data::backtrace* _final_bt;
    data::frame_stack _frame_stack;
    data::buffered_pop_front<data::backtrace> _final_bt_pop;
    data::metaprogram_mode _mode;
    data::type_or_code_or_error _result;

    void pop_event();
    void push_event(data::event_data event_);
  };
}

#endif
