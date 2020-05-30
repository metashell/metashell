#ifndef METASHELL_CACHED_HPP
#define METASHELL_CACHED_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <functional>
#include <optional>

namespace metashell
{
  namespace core
  {
    template <class ValueType>
    class cached
    {
    public:
      cached() {}

      explicit cached(std::function<ValueType()> getter_)
        : _getter(std::move(getter_))
      {
      }

      const ValueType& operator*()
      {
        if (!_value)
        {
          _value = _getter();
        }
        return *_value;
      }

      const ValueType* operator->() { return &**this; }
    private:
      std::optional<ValueType> _value;
      std::function<ValueType()> _getter;
    };
  }
}

#endif
