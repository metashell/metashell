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
#include <metashell/data/shell_config_name.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>

namespace metashell
{
  namespace data
  {
    namespace
    {
      const char path_separators[] = "/\\";

      bool path_separator(char c_) { return c_ == '/' || c_ == '\\'; }
    }

    std::size_t common_prefix_length(const shell_config_name& name1_,
                                     const shell_config_name& name2_)
    {
      const auto size1 = size(name1_);
      if (size1 <= 1)
      {
        return 0;
      }

      const auto size2 = size(name2_);
      if (size2 <= 1)
      {
        return 0;
      }

      const auto max_result = std::min(size1 - 1, size2 - 1);

      auto at1 = name1_.begin();
      auto at2 = name2_.begin();
      while (std::size_t(at1 - name1_.begin()) != max_result &&
             at1->value() == at2->value())
      {
        ++at1;
        ++at2;
      }
      return at1 - name1_.begin();
    }

    shell_config_name::element::element(std::string value_)
      : element(std::move(value_), std::nullopt)
    {
    }

    shell_config_name::element::element(std::string value_, char sep_)
      : element(std::move(value_), std::make_optional(sep_))
    {
    }

    shell_config_name::element::element(std::string value_,
                                        std::optional<char> sep_)
      : _value(std::move(value_)), _separator(sep_)
    {
      if (std::find_if(_value.begin(), _value.end(), path_separator) !=
          _value.end())
      {
        throw exception("Shell config name element contains path separator: " +
                        _value);
      }

      if (_separator && !path_separator(*_separator))
      {
        throw exception("Invalid path separator: " +
                        std::string(1, *_separator));
      }
    }

    const std::string& shell_config_name::element::value() const
    {
      return _value;
    }

    std::optional<char> shell_config_name::element::separator() const
    {
      return _separator;
    }

    bool operator==(const shell_config_name::element& lhs_,
                    const shell_config_name::element& rhs_)
    {
      return lhs_.separator() == rhs_.separator() &&
             lhs_.value() == rhs_.value();
    }

    std::string to_string(const shell_config_name::element& e_)
    {
      std::string result = e_.value();
      if (const auto sep = e_.separator())
      {
        result += *sep;
      }
      return result;
    }

    std::ostream& operator<<(std::ostream& out_,
                             const shell_config_name::element& e_)
    {
      return out_ << to_string(e_);
    }

    shell_config_name::iterator::iterator(const std::string* str_,
                                          std::string::size_type next_from_,
                                          std::ptrdiff_t index_)
      : _str(str_), _next_from(next_from_), _value(std::nullopt), _index(index_)
    {
      assert(_str);
      ++*this;
      _index = index_;
    }

    bool shell_config_name::iterator::operator==(const iterator& rhs_) const
    {
      return _str == rhs_._str && _next_from == rhs_._next_from &&
             _value == rhs_._value && _index == rhs_._index;
    }

    shell_config_name::iterator& shell_config_name::iterator::operator++()
    {
      ++_index;

      if (_next_from == _str->size())
      {
        _value = element("");
        return *this;
      }

      const auto next = _str->find_first_of(path_separators, _next_from);
      if (next == std::string::npos)
      {
        _value = element(_str->substr(_next_from));
        _next_from = _str->size();
      }
      else
      {
        _value =
            element(_str->substr(_next_from, next - _next_from), (*_str)[next]);
        _next_from = next + 1;
      }

      return *this;
    }

    const shell_config_name::element& shell_config_name::iterator::
    operator*() const
    {
      assert(_value);
      return *_value;
    }

    std::ptrdiff_t shell_config_name::iterator::
    operator-(const iterator& rhs_) const
    {
      return _index - rhs_._index;
    }

    shell_config_name::shell_config_name(std::string value_)
      : _value(std::move(value_))
    {
      if (_value.empty())
      {
        throw exception("Empty shell config name value");
      }
      update_size();
    }

    shell_config_name::shell_config_name(const element& e_)
      : shell_config_name(to_string(e_))
    {
    }

    void shell_config_name::update_size()
    {
      _size = std::count_if(_value.begin(), _value.end(), path_separator) + 1;
    }

    const std::string& shell_config_name::value() const { return _value; }

    std::size_t shell_config_name::size() const { return _size; }

    shell_config_name::iterator shell_config_name::begin() const
    {
      return iterator(&_value, 0, 0);
    }

    shell_config_name::iterator shell_config_name::end() const
    {
      return iterator(&_value, _value.size(), _size);
    }

    shell_config_name& shell_config_name::
    operator+=(const shell_config_name& rhs_)
    {
      _value += rhs_._value;
      update_size();
      return *this;
    }

    shell_config_name& shell_config_name::operator+=(const element& rhs_)
    {
      _value += to_string(rhs_);
      update_size();
      return *this;
    }

    shell_config_name::element shell_config_name::front() const
    {
      const auto sep = _value.find_first_of(path_separators);
      return sep == std::string::npos ?
                 element(_value) :
                 element(_value.substr(0, sep), _value[sep]);
    }

    shell_config_name::element shell_config_name::back() const
    {
      const auto sep = _value.find_last_of(path_separators);
      return element(sep == std::string::npos ? _value :
                                                _value.substr(sep + 1));
    }

    bool operator==(const shell_config_name& lhs_,
                    const shell_config_name& rhs_)
    {
      return lhs_.value() == rhs_.value();
    }

    bool operator<(const shell_config_name& lhs_, const shell_config_name& rhs_)
    {
      return lhs_.value() < rhs_.value();
    }

    std::string operator+(const std::string& lhs_,
                          const shell_config_name& rhs_)
    {
      return lhs_ + rhs_.value();
    }

    std::string operator+(const shell_config_name& lhs_,
                          const std::string& rhs_)
    {
      return lhs_.value() + rhs_;
    }

    shell_config_name operator+(const shell_config_name& lhs_,
                                const shell_config_name::element& rhs_)
    {
      return shell_config_name(lhs_) += rhs_;
    }

    shell_config_name operator+(const shell_config_name::element& lhs_,
                                const shell_config_name& rhs_)
    {
      return shell_config_name(lhs_) += rhs_;
    }

    std::size_t size(const shell_config_name& name_) { return name_.size(); }

    std::optional<shell_config_name>
    remove_prefix(const shell_config_name& name_, std::size_t len_)
    {
      if (len_ >= size(name_))
      {
        return std::nullopt;
      }

      auto i = name_.begin();
      const auto e = name_.end();
      std::advance(i, len_);
      assert(i != e);

      if (i->value().empty() && !i->separator())
      {
        assert(++i == e); // assert changes i, but its value is not read later
        return std::nullopt;
      }

      shell_config_name first(*i);
      ++i;
      return std::accumulate(i, e, first);
    }

    shell_config_name substr(const shell_config_name& s_,
                             std::string::size_type pos_,
                             std::string::size_type len_)
    {
      return shell_config_name(s_.value().substr(pos_, len_));
    }

    std::string
    to_string_with_standard_suffix(const shell_config_name::element& s_)
    {
      return s_.value() + (s_.separator() ? "/" : "");
    }
  }
}
