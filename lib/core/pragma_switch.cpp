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

#include <metashell/pragma_switch.hpp>
#include <metashell/metashell_pragma.hpp>
#include <metashell/shell.hpp>
#include <metashell/metashell.hpp>
#include <metashell/version.hpp>

#include <boost/algorithm/string/join.hpp>

#include <algorithm>

using namespace metashell;

namespace
{
  const std::string true_values[] = {"on", "1"};
  const std::string false_values[] = {"off", "0"};

  template <int N>
  bool element_of(const std::string(&a_)[N], const std::string& item_)
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

pragma_switch::pragma_switch(const std::string& name_,
                             const std::function<bool()>& query_,
                             const std::function<void(bool)>& update_)
  : _query(query_), _update(update_), _name(name_)
{
}

iface::pragma_handler* pragma_switch::clone() const
{
  return new pragma_switch(_name, _query, _update);
}

std::string pragma_switch::arguments() const
{
  return "[" + valid_arguments() + "]";
}

std::string pragma_switch::description() const
{
  return "Turns " + _name +
         " on or off. When no arguments are used, it displays if " + _name +
         " is turned on.";
}

void pragma_switch::run(const data::command::iterator&,
                        const data::command::iterator&,
                        const data::command::iterator& args_begin_,
                        const data::command::iterator& args_end_,
                        iface::displayer& displayer_) const
{
  auto i = args_begin_;

  if (i != args_end_)
  {
    const std::string v = i->value();
    if (valid_argument(v))
    {
      ++i;
      if (i == args_end_)
      {
        _update(element_of(true_values, v));
      }
      else
      {
        displayer_.show_error("Invalid arguments after " + v + ": " +
                              tokens_to_string(i, args_end_));
      }
    }
    else
    {
      displayer_.show_error("Invalid argument " + v + ". Valid values are: " +
                            valid_arguments());
    }
  }
  displayer_.show_comment(
      data::text(_name + " is " + (_query() ? "on" : "off")));
}
