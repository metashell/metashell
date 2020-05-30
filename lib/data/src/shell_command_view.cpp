// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/shell_command_view.hpp>

#include <cassert>
#include <cctype>

namespace metashell
{
  namespace data
  {
    shell_command_view::iterator::iterator(
        std::string_view::const_iterator begin_,
        std::string_view::const_iterator end_)
      : _begin(begin_), _end(end_), _value(std::nullopt)
    {
      ++*this;
    }

    bool shell_command_view::iterator::operator==(const iterator& rhs_) const
    {
      assert(_end == rhs_._end);

      return _begin == rhs_._begin && _value == rhs_._value;
    }

    shell_command_view::iterator& shell_command_view::iterator::operator++()
    {
      std::string value;
      for (; _begin != _end && (!std::isspace(*_begin) || value.empty());
           ++_begin)
      {
        if (!std::isspace(*_begin))
        {
          value += *_begin;
        }
      }

      _value = value.empty() ? std::nullopt :
                               std::make_optional(command_line_argument(value));

      _begin =
          std::find_if(_begin, _end, [](char c_) { return !std::isspace(c_); });

      return *this;
    }

    const command_line_argument& shell_command_view::iterator::operator*() const
    {
      assert(_value);
      return *_value;
    }

    shell_command_view::shell_command_view(std::string_view value_)
      : _value(value_)
    {
    }

    shell_command_view::iterator shell_command_view::begin() const
    {
      return iterator(_value.begin(), _value.end());
    }

    shell_command_view::iterator shell_command_view::end() const
    {
      return iterator(_value.end(), _value.end());
    }
  }
}
