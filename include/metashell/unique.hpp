#ifndef METASHELL_UNIQUE_HPP
#define METASHELL_UNIQUE_HPP

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

#include <iosfwd>
#include <string>

namespace metashell
{
  template <class T>
  class unique : boost::totally_ordered<unique<T>>
  {
  public:
    unique() : _value(), _id(_counter) { ++_counter; }

    explicit unique(T value_) : _value(std::move(value_)), _id(_counter)
    {
      ++_counter;
    }

    const T& value() const { return _value; }
    int id() const { return _id; }
  private:
    T _value;
    int _id;

    static int _counter;
  };

  template <class T>
  int unique<T>::_counter = 0;

  template <class T>
  unique<T> unique_value(T value_)
  {
    return unique<T>(std::move(value_));
  }

  template <class T, class U>
  bool operator==(const unique<T>& a_, const unique<U>& b_)
  {
    return a_.id() == b_.id() && a_.value() == b_.value();
  }

  template <class T, class U>
  bool operator<(const unique<T>& a_, const unique<U>& b_)
  {
    return a_.id() < b_.id() || (a_.id() == b_.id() && a_.value() < b_.value());
  }

  template <class T>
  std::ostream& operator<<(std::ostream& out_, const unique<T>& u_)
  {
    return out_ << u_.value();
  }

  template <class T>
  std::string to_string(const unique<T>& u_)
  {
    using std::to_string;
    return to_string(u_.value());
  }
}

#endif
