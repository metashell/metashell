#ifndef METASHELL_DATA_COUNTER_HPP
#define METASHELL_DATA_COUNTER_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <boost/operators.hpp>

#include <cassert>
#include <limits>

namespace metashell
{
  namespace data
  {
    template <class CounterT>
    class basic_counter : boost::unit_steppable<basic_counter<CounterT>>
    {
    public:
      basic_counter() : _value(std::numeric_limits<CounterT>::min()) {}

      basic_counter& operator++()
      {
#ifndef NDEBUG
        const CounterT old = _value;
#endif
        ++_value;
#ifndef NDEBUG
        assert(old < _value);
#endif
        return *this;
      }

      basic_counter& operator--()
      {
#ifndef NDEBUG
        const CounterT old = _value;
#endif
        --_value;
#ifndef NDEBUG
        assert(_value < old);
#endif
        return *this;
      }

      bool empty() const
      {
        return _value == std::numeric_limits<CounterT>::min();
      }

    private:
      CounterT _value;
    };

    typedef basic_counter<int> counter;
  }
}

#endif
