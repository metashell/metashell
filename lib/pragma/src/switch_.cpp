// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/pragma/switch_.hpp>

#include <metashell/core/code_complete.hpp>

#include <boost/algorithm/string/join.hpp>

#include <algorithm>

namespace metashell
{
  namespace pragma
  {
    namespace
    {
      const std::string true_values[] = {"on", "1"};
      const std::string false_values[] = {"off", "0"};

      template <int N>
      bool element_of(const std::string (&a_)[N], const std::string& item_)
      {
        return std::find(a_, a_ + N, item_) != a_ + N;
      }

      bool valid_argument(const std::string& v_)
      {
        return element_of(true_values, v_) || element_of(false_values, v_);
      }

      std::string valid_arguments()
      {
        using boost::algorithm::join;
        return join(true_values, "|") + "|" + join(false_values, "|");
      }
    }

    switch_::switch_(data::shell_flag flag_) : _flag(flag_) {}

    std::string switch_::arguments() const
    {
      return "[" + valid_arguments() + "]";
    }

    std::string switch_::description() const
    {
      return "Turns " + to_string(_flag) +
             " on or off. When no arguments are used, it displays if " +
             to_string(_flag) + " is turned on.";
    }

    void switch_::run(const data::command::iterator&,
                      const data::command::iterator&,
                      const data::command::iterator& args_begin_,
                      const data::command::iterator& args_end_,
                      iface::main_shell& shell_,
                      iface::displayer& displayer_) const
    {
      auto i = args_begin_;

      if (i != args_end_)
      {
        const std::string v = value(*i).value();
        if (valid_argument(v))
        {
          ++i;
          if (i == args_end_)
          {
            shell_.enabled(_flag, element_of(true_values, v));
          }
          else
          {
            displayer_.show_error("Invalid arguments after " + v + ": " +
                                  tokens_to_string(i, args_end_).value());
          }
        }
        else
        {
          displayer_.show_error("Invalid argument " + v +
                                ". Valid values are: " + valid_arguments());
        }
      }
      displayer_.show_comment(data::text(
          to_string(_flag) + " is " + (shell_.enabled(_flag) ? "on" : "off")));
    }

    data::code_completion
    switch_::code_complete(data::command::const_iterator begin_,
                           data::command::const_iterator end_,
                           iface::main_shell&) const
    {
      data::code_completion result =
          core::code_complete::fixed_values(begin_, end_, true_values);
      result.insert(
          core::code_complete::fixed_values(begin_, end_, false_values));
      return result;
    }
  }
}
