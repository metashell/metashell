#ifndef METASHELL_OVERRIDE_GUARD_HPP
#define METASHELL_OVERRIDE_GUARD_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/utility.hpp>

template <class T>
class override_guard : boost::noncopyable
{
public:
  override_guard(T& var_, T new_value_) :
    _var(var_),
    _old_value(_var)
  {
    _var = new_value_;
  }

  ~override_guard()
  {
    _var = _old_value;
  }
private:
  T& _var;
  T _old_value;
};

#endif

