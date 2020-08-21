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

#include <metashell/data/command_line_argument_list.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>

namespace metashell
{
  namespace data
  {
    bool command_line_argument_list::empty() const { return _items.empty(); }

    command_line_argument_list::command_line_argument_list(int argc_,
                                                           const char* argv_[])
      : _items{argc_ > 0 ? container(argv_ + 1, argv_ + argc_) : container{}}
    {
    }

    command_line_argument_list::command_line_argument_list(
        const command_line_argument_list& a_)
      : _items(a_._items)
    {
    }

    command_line_argument_list::command_line_argument_list(
        command_line_argument_list&& a_)
      : _items(std::move(a_._items))
    {
    }

    command_line_argument_list::command_line_argument_list(
        std::initializer_list<command_line_argument> args_)
      : _items(args_)
    {
    }

    command_line_argument_list::command_line_argument_list(
        std::initializer_list<std::string> args_)
    {
      _items.reserve(args_.size());
      std::transform(
          args_.begin(), args_.end(), std::back_inserter(_items),
          [](const std::string& arg_) { return command_line_argument(arg_); });
    }

    command_line_argument_list::size_type
    command_line_argument_list::size() const
    {
      return _items.size();
    }

    std::optional<command_line_argument>
    command_line_argument_list::front() const
    {
      return _items.empty() ? std::nullopt : std::make_optional(_items.front());
    }

    std::optional<command_line_argument>
    command_line_argument_list::back() const
    {
      return _items.empty() ? std::nullopt : std::make_optional(_items.back());
    }

    command_line_argument_list::iterator
    command_line_argument_list::begin() const
    {
      return _items.begin();
    }

    command_line_argument_list::iterator command_line_argument_list::end() const
    {
      return _items.end();
    }

    std::vector<const char*>
    command_line_argument_list::argv(const executable_path& exe_) const
    {
      std::vector<const char*> result(_items.size() + 2);
      result[0] = exe_.c_str();
      const auto last = std::transform(
          _items.begin(), _items.end(), result.begin() + 1,
          [](const command_line_argument& a_) { return a_.c_str(); });
      assert(last != result.end());
      *last = nullptr;
      return result;
    }

    command_line_argument_list& command_line_argument_list::
    operator+=(const command_line_argument_list& rhs_)
    {
      _items.insert(_items.end(), rhs_.begin(), rhs_.end());
      return *this;
    }

    void command_line_argument_list::push_back(command_line_argument item_)
    {
      _items.push_back(std::move(item_));
    }

    void command_line_argument_list::pop_back()
    {
      assert(!_items.empty());
      _items.pop_back();
    }

    void
    command_line_argument_list::push_back(const boost::filesystem::path& item_)
    {
      _items.emplace_back(item_);
    }

    void command_line_argument_list::clear() { _items.clear(); }

    command_line_argument_list command_line_argument_list::tail() const
    {
      auto b = begin();
      const auto e = end();
      if (b != e)
      {
        ++b;
      }
      return command_line_argument_list(b, e);
    }

    std::pair<command_line_argument_list, command_line_argument_list>
    command_line_argument_list::split_at_first(
        const command_line_argument& arg_) const
    {
      const auto sep = std::find(_items.begin(), _items.end(), arg_);
      return {command_line_argument_list(_items.begin(), sep),
              command_line_argument_list(
                  sep == _items.end() ? sep : sep + 1, _items.end())};
    }

    std::ostream& operator<<(std::ostream& out_,
                             const command_line_argument_list& l_)
    {
      return out_ << to_string(l_);
    }

    std::string to_string(const command_line_argument_list& l_)
    {
      return boost::algorithm::join(
          l_ | boost::adaptors::transformed(&quote), " ");
    }

    bool operator==(const command_line_argument_list& lhs_,
                    const command_line_argument_list& rhs_)
    {
      return lhs_.size() == rhs_.size() &&
             std::equal(lhs_.begin(), lhs_.end(), rhs_.begin());
    }

    std::pair<std::optional<std::string>, command_line_argument_list>
    remove_multiple_arch_arguments(const command_line_argument_list& args_)
    {
      const command_line_argument arch{"-arch"};

      command_line_argument_list result;
      bool was_arch = false;
      std::optional<command_line_argument> first_arch;
      std::vector<command_line_argument> removed;
      for (const command_line_argument& arg : args_)
      {
        if (was_arch)
        {
          if (first_arch)
          {
            removed.push_back(arg);
          }
          else
          {
            result.push_back(arch);
            result.push_back(arg);
            first_arch = arg;
          }
          was_arch = false;
        }
        else if (arg == arch)
        {
          was_arch = true;
        }
        else
        {
          result.push_back(arg);
        }
      }
      if (was_arch)
      {
        result.push_back(arch);
      }

      if (removed.empty())
      {
        return {std::nullopt, result};
      }
      else
      {
        std::string warning = "Removed argument";
        if (removed.size() > 1)
        {
          warning += "s";
        }
        for (const command_line_argument& arg : removed)
        {
          warning += " " + to_string(arch) + " " + to_string(arg);
        }
        warning += " because of earlier argument: " + to_string(arch) + " " +
                   to_string(*first_arch) + ".";
        return {warning, result};
      }
    }
  }
}
