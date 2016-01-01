#ifndef METASHELL_SYSTEM_TEST_PATTERN_HPP
#define METASHELL_SYSTEM_TEST_PATTERN_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell_system_test/placeholder.hpp>

#include <boost/variant.hpp>
#include <boost/optional.hpp>

#include <string>

namespace metashell_system_test
{
  // This (template) class implements simple pattern matching. An object of type
  // pattern<T> represents a pattern a value of T can be matched against.
  // A pattern can be:
  //
  // 1.  a specified value (created by providing a value of type T in the
  //     constructor)
  // 2a. an unspecified value that matches everything (created by providing the
  //     _ object in the constructor)
  // 2b. the same as 2a, but a variable of type T is provided. In this case the
  //     value the pattern is matched against gets stored in that variable.
  //     This is created by providing a pointer to the variable to store the
  //     value in.
  template <class T>
  class pattern
  {
  public:
    pattern(T value_) : _pattern(value_) {} // case 1.
    pattern(T* var_) : _pattern(var_) {} // case 2a.
    pattern(placeholder) : _pattern(static_cast<T*>(nullptr)) {} // case 2b.

    template <class U>
    pattern(U value_)
      : _pattern(T(value_))
    {
    } // case 1.

    template <class U>
    bool match(U value_) const
    {
      if (T* const* p = boost::get<T*>(&_pattern))
      {
        if (*p)
        {
          **p = value_;
        }
        return true;
      }
      else if (const T* p = boost::get<T>(&_pattern))
      {
        return *p == value_;
      }
      else
      {
        return false;
      }
    }

    boost::optional<T> value() const
    {
      if (const T* p = boost::get<T>(&_pattern))
      {
        return *p;
      }
      else
      {
        return boost::none;
      }
    }

  private:
    boost::variant<T, T*> _pattern;
  };

  template <class Writer>
  void write(const pattern<std::string>& pattern_, Writer& writer_)
  {
    if (const boost::optional<std::string> value = pattern_.value())
    {
      writer_.String(value->c_str());
    }
    else
    {
      writer_.Null();
    }
  }
}

#endif
