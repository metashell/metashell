#ifndef METASHELL_DATA_ARG_NUMBER_HPP
#define METASHELL_DATA_ARG_NUMBER_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/counter.hpp>

#include <cassert>

namespace metashell
{
  namespace data
  {
    enum class arg_number
    {
      any,
      at_most_once
    };

    template <class CounterT>
    bool allow_one_more(arg_number num_, basic_counter<CounterT> so_far_)
    {
      switch (num_)
      {
      case arg_number::any:
        return true;
      case arg_number::at_most_once:
        return so_far_.empty();
      }
      assert(!"Invalid arg_number value");
      return false;
    }
  }
}

#endif
