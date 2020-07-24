#ifndef METASHELL_DATA_COMMAND_LINE_ARGUMENT_LIST_HPP
#define METASHELL_DATA_COMMAND_LINE_ARGUMENT_LIST_HPP

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

#include <metashell/data/command_line_argument.hpp>
#include <metashell/data/executable_path.hpp>

#include <boost/operators.hpp>

#include <iosfwd>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace metashell
{
  namespace data
  {
    class command_line_argument_list
        : boost::addable<command_line_argument_list>,
          boost::equality_comparable<command_line_argument_list>
    {
    public:
      using container = std::vector<command_line_argument>;

      using size_type = container::size_type;

      using iterator = container::const_iterator;
      using const_iterator = iterator;

      command_line_argument_list() = default;

      command_line_argument_list(int argc_, const char* argv_[]);

      command_line_argument_list(const command_line_argument_list&);
      command_line_argument_list(command_line_argument_list&&);

      command_line_argument_list(std::initializer_list<std::string>);

      command_line_argument_list(std::initializer_list<command_line_argument>);

      template <class InputIt>
      command_line_argument_list(InputIt begin_, InputIt end_)
      {
        for (InputIt i = begin_; i != end_; ++i)
        {
          _items.emplace_back(*i);
        }
      }

      command_line_argument_list&
      operator=(const command_line_argument_list&) = default;

      command_line_argument_list&
      operator=(command_line_argument_list&&) = default;

      command_line_argument_list tail() const;

      bool empty() const;
      size_type size() const;

      std::optional<command_line_argument> front() const;
      std::optional<command_line_argument> back() const;

      void push_back(command_line_argument);
      void push_back(const boost::filesystem::path&);

      void pop_back();

      template <size_t Len>
      void push_back(const char (&item_)[Len])
      {
        _items.emplace_back(item_);
      }

      template <class... Args>
      void push_back(Args&&... args_)
      {
        _items.reserve(_items.size() + sizeof...(Args));
        (_items.emplace_back(std::forward<Args>(args_)), ...);
      }

      template <size_t Len>
      void
      append_with_prefix(const char (&prefix_)[Len],
                         const std::vector<boost::filesystem::path>& paths_)
      {
        _items.reserve(_items.size() + paths_.size());
        for (const boost::filesystem::path& path : paths_)
        {
          _items.emplace_back(prefix_ + path.string());
        }
      }

      template <size_t Len>
      void append_with_prefix(const char (&prefix_)[Len],
                              const std::vector<std::string>& values_)
      {
        _items.reserve(_items.size() + values_.size());
        for (const std::string& value : values_)
        {
          _items.emplace_back(prefix_ + value);
        }
      }

      template <class InputIt>
      void append(InputIt begin_, InputIt end_)
      {
        _items.insert(_items.end(), begin_, end_);
      }

      void clear();

      iterator begin() const;
      iterator end() const;

      std::vector<const char*> argv(const executable_path&) const;

      command_line_argument_list& operator+=(const command_line_argument_list&);

      std::pair<command_line_argument_list, command_line_argument_list>
      split_at_first(const command_line_argument&) const;

      template <size_t Len>
      std::pair<command_line_argument_list, command_line_argument_list>
      split_at_first(const char (&arg_)[Len]) const
      {
        return split_at_first(command_line_argument(arg_));
      }

    private:
      container _items;
    };

    std::ostream& operator<<(std::ostream&, const command_line_argument_list&);
    std::string to_string(const command_line_argument_list&);

    bool operator==(const command_line_argument_list&,
                    const command_line_argument_list&);

    std::pair<std::optional<std::string>, command_line_argument_list>
    remove_multiple_arch_arguments(const command_line_argument_list&);
  }
}

#endif
