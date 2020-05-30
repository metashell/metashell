#ifndef METASHELL_SHELL_CONFIG_NAME_HPP
#define METASHELL_SHELL_CONFIG_NAME_HPP

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

#include <algorithm>
#include <cassert>
#include <iosfwd>
#include <numeric>
#include <optional>
#include <string>

namespace metashell
{
  namespace data
  {
    class shell_config_name : boost::totally_ordered<shell_config_name>,
                              boost::addable<shell_config_name>
    {
    public:
      class element : boost::equality_comparable<element>
      {
      public:
        explicit element(std::string);
        element(std::string, char);

        const std::string& value() const;
        std::optional<char> separator() const;

      private:
        std::string _value;
        std::optional<char> _separator;

        element(std::string, std::optional<char>);
      };

      class iterator : public boost::forward_iterator_helper<iterator,
                                                             const element,
                                                             std::ptrdiff_t,
                                                             const element*,
                                                             const element&>
      {
      public:
        iterator(const std::string*,
                 std::string::size_type next_from_,
                 std::ptrdiff_t index_);

        bool operator==(const iterator&) const;

        iterator& operator++();

        const element& operator*() const;

        std::ptrdiff_t operator-(const iterator&) const;

      private:
        const std::string* _str;
        std::string::size_type _next_from;
        std::optional<element> _value;
        std::ptrdiff_t _index;
      };

      explicit shell_config_name(std::string);
      explicit shell_config_name(const element&);

      const std::string& value() const;
      std::size_t size() const;

      iterator begin() const;
      iterator end() const;

      shell_config_name& operator+=(const shell_config_name&);
      shell_config_name& operator+=(const element&);

      element front() const;
      element back() const;

    private:
      std::string _value;
      std::size_t _size;

      void update_size();
    };

    bool operator==(const shell_config_name::element&,
                    const shell_config_name::element&);

    std::string to_string(const shell_config_name::element&);
    std::ostream& operator<<(std::ostream&, const shell_config_name::element&);

    std::string operator+(const std::string&, const shell_config_name&);
    std::string operator+(const shell_config_name&, const std::string&);

    shell_config_name operator+(const shell_config_name&,
                                const shell_config_name::element&);
    shell_config_name operator+(const shell_config_name::element&,
                                const shell_config_name&);

    bool operator==(const shell_config_name&, const shell_config_name&);
    bool operator<(const shell_config_name&, const shell_config_name&);

    std::size_t size(const shell_config_name&);

    std::optional<shell_config_name> remove_prefix(const shell_config_name&,
                                                   std::size_t);

    shell_config_name substr(const shell_config_name& s_,
                             std::string::size_type pos_ = 0,
                             std::string::size_type len_ = std::string::npos);

    std::size_t common_prefix_length(const shell_config_name&,
                                     const shell_config_name&);

    template <class Container>
    std::size_t common_prefix_length(const Container& names_)
    {
      auto i = names_.begin();
      const auto e = names_.end();
      if (i == e)
      {
        return 0;
      }

      const shell_config_name& first = *i;
      const auto first_size = size(first);
      if (first_size <= 1)
      {
        return 0;
      }

      ++i;

      return std::accumulate(
          i, e, first_size - 1,
          [&first](std::size_t sub_result_, const shell_config_name& name_) {
            return std::min(sub_result_, common_prefix_length(first, name_));
          });
    }

    std::string
    to_string_with_standard_suffix(const shell_config_name::element&);
  }
}

#endif
