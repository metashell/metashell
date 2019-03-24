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

#include <metashell/data/exception.hpp>
#include <metashell/data/mdb_command.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/range.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <limits>
#include <tuple>

namespace metashell
{
  namespace data
  {
    namespace
    {
      bool is_space(char c_) { return std::isspace(c_); }
    }

    mdb_command::name_type::name_type(std::string value_)
      : _value(std::move(value_))
    {
      if (std::find_if(_value.begin(), _value.end(), is_space) != _value.end())
      {
        throw exception("Invalid MDB command name: " + _value);
      }
    }

    const std::string& mdb_command::name_type::value() const { return _value; }

    bool mdb_command::name_type::empty() const { return _value.empty(); }

    mdb_command::name_type::const_iterator mdb_command::name_type::begin() const
    {
      return _value.begin();
    }

    mdb_command::name_type::const_iterator mdb_command::name_type::end() const
    {
      return _value.end();
    }

    std::ostream& operator<<(std::ostream& out_,
                             const mdb_command::name_type& name_)
    {
      return out_ << name_.value();
    }

    std::string to_string(const mdb_command::name_type& name_)
    {
      return name_.value();
    }

    bool operator==(const mdb_command::name_type& lhs_,
                    const mdb_command::name_type& rhs_)
    {
      return lhs_.value() == rhs_.value();
    }

    bool operator<(const mdb_command::name_type& lhs_,
                   const mdb_command::name_type& rhs_)
    {
      return lhs_.value() < rhs_.value();
    }

    mdb_command::argument_type::argument_type(std::string value_,
                                              whitespace suffix_)
      : _value(std::move(value_)), _suffix(std::move(suffix_))
    {
    }

    const std::string& mdb_command::argument_type::value() const
    {
      return _value;
    }

    const whitespace& mdb_command::argument_type::suffix() const
    {
      return _suffix;
    }

    mdb_command::arguments_type::iterator::iterator(
        std::string::const_iterator begin_, std::string::const_iterator end_)
      : _value(), _at(std::find_if_not(begin_, end_, is_space)), _end(end_)
    {
      ++*this;
    }

    mdb_command::arguments_type::iterator&
        mdb_command::arguments_type::iterator::operator++()
    {
      const auto b = _at;
      const auto e = std::find_if(b, _end, is_space);
      _at = std::find_if_not(e, _end, is_space);
      _value = argument_type(b, e, whitespace(e, _at));
      return *this;
    }

    const mdb_command::argument_type& mdb_command::arguments_type::iterator::
    operator*() const
    {
      return _value;
    }

    bool operator==(const mdb_command::argument_type& lhs_,
                    const mdb_command::argument_type& rhs_)
    {
      return lhs_.value() == rhs_.value() && lhs_.suffix() == rhs_.suffix();
    }

    mdb_command::arguments_type::arguments_type(std::string value_)
      : _value(std::move(value_))
    {
    }

    const std::string& mdb_command::arguments_type::value() const
    {
      return _value;
    }

    bool mdb_command::arguments_type::empty() const { return _value.empty(); }

    bool mdb_command::arguments_type::iterator::
    operator==(const iterator& rhs_) const
    {
      assert(_end == rhs_._end);
      return _at == rhs_._at && _value == rhs_._value;
    }

    mdb_command::arguments_type::iterator
    mdb_command::arguments_type::begin() const
    {
      return iterator(_value.begin(), _value.end());
    }

    mdb_command::arguments_type::iterator
    mdb_command::arguments_type::end() const
    {
      return iterator(_value.end(), _value.end());
    }

    std::string join(mdb_command::arguments_type::iterator begin_,
                     mdb_command::arguments_type::iterator end_)
    {
      return boost::algorithm::join(
          boost::make_iterator_range(begin_, end_) |
              boost::adaptors::transformed(
                  [](const mdb_command::argument_type& a_) {
                    return a_.value() + a_.suffix();
                  }),
          "");
    }

    mdb_command::argument_type::operator int() const
    {
      if (_value.empty())
      {
        throw exception("Integer expected");
      }

      unsigned int result = 0;
      bool minus = false;
      unsigned int limit = std::numeric_limits<int>::max();
      for (char c : _value)
      {
        if (c >= '0' && c <= '9')
        {
          const auto val = c - '0';
          if (result > (limit - val) / 10)
          {
            throw exception("Integer value is too large: " + _value);
          }
          else
          {
            result = result * 10 + val;
          }
        }
        else if (c == '-' && !minus)
        {
          minus = true;
          limit = static_cast<unsigned int>(
                      -(std::numeric_limits<int>::min() + 1)) +
                  1;
        }
        else
        {
          throw exception("Invalid integer: " + _value);
        }
      }

      if (!minus)
      {
        return result;
      }
      else if (result == 0)
      {
        throw exception("Invalid integer: " + _value);
      }
      else
      {
        return -int(result);
      }
    }

    mdb_command::arguments_type::operator int() const
    {
      auto i = begin();
      const auto e = end();
      if (i == e)
      {
        throw data::exception(
            "Empty value found where an integer is expected.");
      }
      const int result(*i);
      ++i;
      if (i == e)
      {
        return result;
      }
      else
      {
        throw data::exception("Unexpected argument: " + join(i, e));
      }
    }

    mdb_command::arguments_type::operator regex() const
    {
      return regex(_value);
    }

    mdb_command::arguments_type::operator mdb_command() const
    {
      return mdb_command(_value);
    }

    std::ostream& operator<<(std::ostream& out_,
                             const mdb_command::arguments_type& a_)
    {
      return out_ << a_.value();
    }

    std::string to_string(const mdb_command::arguments_type& a_)
    {
      return a_.value();
    }

    bool operator==(const mdb_command::arguments_type& lhs_,
                    const mdb_command::arguments_type& rhs_)
    {
      return lhs_.value() == rhs_.value();
    }

    mdb_command::mdb_command(const std::string& value_)
    {
      const auto e = value_.end();

      auto command_begin = std::find_if_not(value_.begin(), e, is_space);
      auto command_end = std::find_if(command_begin, e, is_space);
      auto arguments_begin = std::find_if_not(command_end, e, is_space);

      std::get<0>(_val) = whitespace(value_.begin(), command_begin);
      std::get<1>(_val) = name_type(command_begin, command_end);
      std::get<2>(_val) = whitespace(command_end, arguments_begin);
      std::get<3>(_val) = arguments_type(arguments_begin, e);
    }

    bool mdb_command::empty() const
    {
      assert(!(name().empty() && !separator().empty()));
      assert(!(name().empty() && !arguments().empty()));

      return prefix().empty() && name().empty();
    }

    bool mdb_command::only_whitespace() const
    {
      assert(arguments().empty() ||
             !std::all_of(arguments().value().begin(),
                          arguments().value().end(), is_space));

      return name().empty();
    }

    const whitespace& mdb_command::prefix() const { return std::get<0>(_val); }

    const mdb_command::name_type& mdb_command::name() const
    {
      return std::get<1>(_val);
    }

    const whitespace& mdb_command::separator() const
    {
      return std::get<2>(_val);
    }

    const mdb_command::arguments_type& mdb_command::arguments() const
    {
      return std::get<3>(_val);
    }

    const mdb_command::tuple_t mdb_command::tuple() const { return _val; }

    std::ostream& operator<<(std::ostream& out_, const mdb_command& c_)
    {
      return out_ << c_.prefix() << c_.name() << c_.separator()
                  << c_.arguments();
    }

    std::string to_string(const mdb_command& c_)
    {
      return c_.prefix().value() + c_.name().value() + c_.separator().value() +
             c_.arguments().value();
    }

    bool operator==(const mdb_command& lhs_, const mdb_command& rhs_)
    {
      return lhs_.tuple() == rhs_.tuple();
    }
  }
}
