#ifndef METASHELL_INDEXING_ITERATOR_HPP
#define METASHELL_INDEXING_ITERATOR_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/function.hpp>
#include <boost/operators.hpp>

namespace metashell
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
    typedef boost::function<ValueType(int)> getter;

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

#endif

