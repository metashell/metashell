#ifndef METASHELL_FRAME_STACK_HPP
#define METASHELL_FRAME_STACK_HPP

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

#include <metashell/data/debugger_event.hpp>
#include <metashell/data/metaprogram_mode.hpp>

#include <vector>

namespace metashell
{
  namespace data
  {
    class frame_stack
    {
    public:
      typedef std::vector<debugger_event>::size_type value_type;

      bool empty() const;

      void push_back(value_type item_);
      frame& back(std::vector<debugger_event>& events_) const;
      void pop_back();

      void running_at(std::vector<debugger_event>& events_, double timestamp_);

    private:
      std::vector<value_type> _stack;
    };

    void running_at(frame_stack& stack_,
                    std::vector<debugger_event>& events_,
                    const std::optional<double>& timestamp_,
                    metaprogram_mode mode_);
  }
}

#endif
