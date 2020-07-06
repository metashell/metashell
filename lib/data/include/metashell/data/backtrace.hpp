#ifndef METASHELL_BACKTRACE_HPP
#define METASHELL_BACKTRACE_HPP

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

#include <metashell/data/debugger_event.hpp>
#include <metashell/data/frame.hpp>

#include <boost/operators.hpp>

#include <iosfwd>
#include <optional>
#include <vector>

namespace metashell
{
  namespace data
  {
    class backtrace : boost::equality_comparable<backtrace>
    {
    public:
      typedef std::vector<frame>::size_type size_type;
      typedef std::vector<frame>::const_reverse_iterator iterator;
      typedef iterator const_iterator;

      backtrace(bool buffered_ = false);

      backtrace(std::initializer_list<frame> frames_);

      void push_front(const frame& f_);
      void pop_front();

      size_type size() const;
      bool empty() const;

      const frame& operator[](size_type i) const;

      iterator begin() const;
      iterator end() const;

      void update(const frame& event_);
      void update(const pop_frame& event_);

    private:
      std::vector<frame> _frames;
      std::optional<int> _buffered_pop_count;

      void flush();
      bool flushed_empty() const;
      const frame& flushed_front() const;
      int buffered_pop_count() const;

      void pop_flat();
    };

    std::ostream& operator<<(std::ostream& o_, const backtrace& t_);
    bool operator==(const backtrace& a_, const backtrace& b_);

    void update(backtrace& bt_, const debugger_event& event_);
  }
}

#endif
