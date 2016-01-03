#ifndef PATTERN_STRING_HPP
#define PATTERN_STRING_HPP

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

#include <pattern/placeholder.hpp>

#include <boost/variant.hpp>
#include <boost/optional.hpp>

#include <string>

namespace pattern
{
  // This class implements simple pattern matching. An object of type
  // string represents a pattern a string can be matched against.
  // A pattern can be:
  //
  // 1.  a specified value (created by providing a string in the constructor)
  // 2a. an unspecified value that matches everything (created by providing the
  //     _ object in the constructor)
  // 2b. the same as 2a, but a string variable is provided. In this case the
  //     value the pattern is matched against gets stored in that variable.
  //     This is created by providing a pointer to the variable to store the
  //     value in.
  class string
  {
  public:
    string(std::string value_) : _pattern(move(value_)) {} // case 1.
    string(std::string* var_) : _pattern(var_) {} // case 2a.
    string(placeholder) : _pattern(static_cast<std::string*>(nullptr)) {} // case
                                                                          // 2b.

    template <class T>
    string(T value_)
      : _pattern(std::string(value_))
    {
    } // case 1.

    template <class T>
    bool match(T value_) const
    {
      if (std::string* const* p = boost::get<std::string*>(&_pattern))
      {
        if (*p)
        {
          **p = value_;
        }
        return true;
      }
      else if (const T* p = boost::get<std::string>(&_pattern))
      {
        return *p == value_;
      }
      else
      {
        return false;
      }
    }

    boost::optional<std::string> value() const
    {
      if (const std::string* p = boost::get<std::string>(&_pattern))
      {
        return *p;
      }
      else
      {
        return boost::none;
      }
    }

  private:
    boost::variant<std::string, std::string*> _pattern;
  };

  template <class Writer>
  void write(const string& pattern_, Writer& writer_)
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
