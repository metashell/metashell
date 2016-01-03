#ifndef PATTERN_BASIC_STRING_HPP
#define PATTERN_BASIC_STRING_HPP

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

#include <pattern/placeholder.hpp>
#include <pattern/basic_regex.hpp>

#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <boost/regex.hpp>

#include <string>

namespace pattern
{
  // This template class implements simple pattern matching. An object of type
  // string represents a pattern a string can be matched against.
  // A pattern can be:
  //
  // 1. a specified value (created by providing a string in the constructor)
  // 2. an unspecified value that matches everything (created by providing the
  //    _ object in the constructor)
  // 3. a regular expression that is searched for in the value
  template <class CharT>
  class basic_string
  {
  public:
    // case 1.
    basic_string(std::basic_string<CharT> value_) : _pattern(move(value_)) {}

    // case 2.
    basic_string(placeholder) : _pattern(basic_regex<CharT>()) {}

    // case 3.
    basic_string(basic_regex<CharT> regex_) : _pattern(std::move(regex_)) {}

    // case 1.
    template <class T>
    basic_string(T value_)
      : _pattern(std::basic_string<CharT>(value_))
    {
    }

    template <class T>
    bool match(T value_) const
    {
      if (const std::basic_string<CharT>* p =
              boost::get<std::basic_string<CharT>>(&_pattern))
      {
        return *p == value_;
      }
      else if (const basic_regex<CharT>* re =
                   boost::get<basic_regex<CharT>>(&_pattern))
      {
        return boost::regex_search(std::basic_string<CharT>(value_),
                                   boost::basic_regex<CharT>(re->value()));
      }
      else
      {
        return false;
      }
    }

    boost::optional<std::basic_string<CharT>> value() const
    {
      if (const std::basic_string<CharT>* p =
              boost::get<std::basic_string<CharT>>(&_pattern))
      {
        return *p;
      }
      else if (const basic_regex<CharT>* re =
                   boost::get<basic_regex<CharT>>(&_pattern))
      {
        if (re->value().empty())
        {
          return boost::none;
        }
        else
        {
          return std::basic_string<CharT>(re->value());
        }
      }
      else
      {
        return boost::none;
      }
    }

  private:
    boost::variant<std::basic_string<CharT>, basic_regex<CharT>> _pattern;
  };
}

#endif
