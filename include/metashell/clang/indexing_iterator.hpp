#ifndef METASHELL_INDEXING_ITERATOR_HPP
#define METASHELL_INDEXING_ITERATOR_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <functional>

namespace metashell
{
  namespace clang
  {
    template <class ValueType>
    class indexing_iterator :
      public
        boost::random_access_iterator_helper<
          indexing_iterator<ValueType>,
          ValueType*,
          int,
          ValueType&,
          ValueType
        >
    {
    public:
      typedef std::function<ValueType(int)> getter;

      explicit indexing_iterator(int n_ = 0) :
        _f(),
        _n(n_),
        _value_valid(false)
      {}

      indexing_iterator(const getter& f_, int n_) :
        _f(f_),
        _n(n_),
        _value_valid(false)
      {}

      bool operator<(const indexing_iterator& i_) const
      {
        return _n < i_._n;
      }

      bool operator==(const indexing_iterator& i_) const
      {
        return _n == i_._n;
      }

      indexing_iterator& operator+=(int n_)
      {
        _n += n_;
        _value_valid = false;
        return *this;
      }

      indexing_iterator& operator++()
      {
        ++_n;
        _value_valid = false;
        return *this;
      }

      indexing_iterator& operator-=(int n_)
      {
        _n -= n_;
        _value_valid = false;
        return *this;
      }

      indexing_iterator& operator--()
      {
        --_n;
        _value_valid = false;
        return *this;
      }

      int operator-(const indexing_iterator& i_) const
      {
        return _n - i_._n;
      }

      const ValueType& operator*() const
      {
        if (!_value_valid)
        {
          _value = _f(_n);
          _value_valid = true;
        }
        return _value;
      }
    private:
      getter _f;
      int _n;
      mutable ValueType _value;
      mutable bool _value_valid;
    };
  }
}

#endif

