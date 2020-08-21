#ifndef METASHELL_MDB_METAPROGRAM_HPP
#define METASHELL_MDB_METAPROGRAM_HPP

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

#include <metashell/mdb/debugger_history.hpp>

#include <metashell/iface/event_data_sequence.hpp>

#include <metashell/data/backtrace.hpp>
#include <metashell/data/debugger_event.hpp>
#include <metashell/data/direction_t.hpp>
#include <metashell/data/event_data.hpp>
#include <metashell/data/frame.hpp>
#include <metashell/data/frame_only_event.hpp>
#include <metashell/data/metaprogram_mode.hpp>
#include <metashell/data/pop_frame.hpp>
#include <metashell/data/tree_depth.hpp>
#include <metashell/data/type_or_code_or_error.hpp>

#include <boost/operators.hpp>

#include <iterator>
#include <memory>
#include <optional>
#include <vector>

namespace metashell
{
  namespace mdb
  {
    class metaprogram
    {
    public:
      typedef debugger_history::size_type size_type;

      class iterator
          : boost::random_access_iteratable<iterator,
                                            const data::debugger_event*,
                                            std::ptrdiff_t,
                                            data::debugger_event&>
      {
      public:
        typedef std::input_iterator_tag iterator_category;
        typedef const data::debugger_event value_type;
        typedef std::ptrdiff_t difference_type;
        typedef value_type* pointer;
        typedef value_type& reference;

        explicit iterator(metaprogram& mp_);
        iterator(metaprogram& mp_, metaprogram::size_type at_);

        reference operator*() const;

        iterator& operator++();
        iterator& operator--();
        iterator& operator+=(difference_type n);
        iterator& operator-=(difference_type n);

        bool operator==(const iterator& mi) const;
        bool operator<(const iterator& mi) const;

        reference operator[](difference_type n) const;

      private:
        metaprogram* mp;
        std::optional<metaprogram::size_type> at;
      };

      friend iterator;

      typedef iterator const_iterator;

      metaprogram(std::unique_ptr<iface::event_data_sequence> trace,
                  bool caching_enabled);

      bool is_empty();

      const data::type_or_code_or_error& get_evaluation_result();

      data::metaprogram_mode get_mode() const;

      bool is_at_endpoint(data::direction_t direction) const;
      bool is_finished() const;
      bool is_at_start() const;

      void step(data::direction_t direction);
      void step();
      void step_back();

      const data::frame& get_current_frame() const;
      const data::backtrace& get_backtrace();

      size_type size();

      iterator begin(bool include_original_expression = true);
      iterator current_position();
      iterator end();

      bool caching_enabled() const;

    private:
      std::unique_ptr<iface::event_data_sequence> event_source;
      std::optional<data::event_data> next_unread_event;

      std::optional<data::frame_only_event> current_frame;
      bool read_open_or_flat = false;
      bool has_unread_event = true;
      size_type read_event_count = 1; // The root event
      data::tree_depth tree_depth;

      // using indices to avoid invalidation during copy/move
      size_type next_event = 0;
      std::optional<data::backtrace> current_bt;
      data::backtrace final_bt{true};

      data::metaprogram_mode mode;

      std::optional<debugger_history> history;

      data::type_or_code_or_error result;

      void cache_current_frame();

      std::optional<data::debugger_event> read_next_event();

      bool
      try_reading_until(size_type pos,
                        std::optional<data::debugger_event>* last_event_read_);

      void read_remaining_events();

      bool cached_ahead_of(size_type loc) const;
    };
  }
}

#endif
