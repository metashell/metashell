#ifndef METASHELL_METAPROGRAM_HPP
#define METASHELL_METAPROGRAM_HPP

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

#include <metashell/metaprogram_builder.hpp>

#include <metashell/data/backtrace.hpp>
#include <metashell/data/debugger_event.hpp>
#include <metashell/data/direction_t.hpp>
#include <metashell/data/event_data.hpp>
#include <metashell/data/frame.hpp>
#include <metashell/data/metaprogram_mode.hpp>
#include <metashell/data/pop_frame.hpp>
#include <metashell/data/type_or_code_or_error.hpp>

#include <boost/optional.hpp>

#include <memory>
#include <vector>

namespace metashell
{
  class metaprogram
  {
  public:
    typedef std::vector<data::debugger_event>::const_iterator iterator;
    typedef iterator const_iterator;

    template <class Container>
    explicit metaprogram(std::unique_ptr<Container> trace)
      : mode(trace->mode()), builder(events, final_bt, mode, trace->root_name())
    {
      while (boost::optional<data::event_data> event = trace->next())
      {
        builder.push_back(std::move(*event));
      }
    }

    bool is_empty() const;

    const data::type_or_code_or_error& get_evaluation_result() const;

    void reset_state();

    data::metaprogram_mode get_mode() const;

    bool is_at_endpoint(data::direction_t direction) const;
    bool is_finished() const;
    bool is_at_start() const;

    void step(data::direction_t direction);
    void step();
    void step_back();

    data::frame get_current_frame() const;
    data::frame get_root_frame() const;
    const data::backtrace& get_backtrace();

    std::size_t size() const;

    iterator begin(bool include_original_expression = true) const;
    iterator current_position() const;
    iterator end() const;

  private:
    std::vector<data::debugger_event> events;
    // using indices to avoid invalidation during copy/move
    std::size_t next_event = 0;
    boost::optional<data::backtrace> current_bt;
    data::backtrace final_bt;

    data::metaprogram_mode mode;

    metaprogram_builder builder;

    void rebuild_backtrace();
    void update_backtrace(const data::debugger_event& event);
    void update_backtrace(const data::frame& event);
    void update_backtrace(const data::pop_frame& event);
  };
}

#endif
