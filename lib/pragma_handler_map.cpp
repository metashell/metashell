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

#include <metashell/pragma_handler_map.hpp>
#include <metashell/metashell_pragma.hpp>
#include <metashell/shell.hpp>

#include <metashell/pragma_help.hpp>
#include <metashell/pragma_switch.hpp>
#include <metashell/pragma_quit.hpp>
#include <metashell/pragma_environment.hpp>
#include <metashell/pragma_environment_push.hpp>
#include <metashell/pragma_environment_pop.hpp>
#include <metashell/pragma_environment_stack.hpp>
#include <metashell/pragma_environment_add.hpp>
#include <metashell/pragma_environment_reset.hpp>
#include <metashell/pragma_environment_reload.hpp>
#include <metashell/pragma_evaluate.hpp>

#include "exception.hpp"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include <cassert>
#include <iostream>
#include <sstream>

using namespace metashell;

namespace
{
  bool can_be_part_of_name(const command::iterator::token_type& t_)
  {
    return t_ == boost::wave::T_IDENTIFIER;
  }

  boost::optional<command::iterator> is_this_pragma(
    const std::vector<std::string>& name_,
    command::iterator begin_,
    const command::iterator& end_
  )
  {
    using std::string;
    using std::vector;
    using boost::optional;

    const vector<string>::const_iterator e = name_.end();
    vector<string>::const_iterator i = name_.begin();
    while (
      begin_ != end_ && i != e
      && can_be_part_of_name(*begin_)
      && *i == string(begin_->get_value().begin(), begin_->get_value().end()))
    {
      ++i;
      begin_ = skip(begin_);
      // Don't accidentally skip over end_
      if (begin_ == end_)
      {
        break;
      }
      begin_ = skip_whitespace(begin_);
    }
    return
      i == e ?
        optional<command::iterator>(begin_) :
        optional<command::iterator>();
  }

  std::string name_of_pragma(
    command::iterator begin_,
    const command::iterator& end_
  )
  {
    std::ostringstream s;
    bool first = true;
    for (
      ;
      begin_ != end_ && can_be_part_of_name(*begin_);
      begin_ = skip_whitespace(skip(begin_))
    )
    {
      s << (first ? "" : " ") << begin_->get_value();
      first = false;
    }
    return s.str();
  }
}

void pragma_handler_map::process(
  const command::iterator& begin_,
  const command::iterator& end_
) const
{
  using boost::optional;

  const command::iterator e = end_of_pragma_argument_list(begin_, end_);

  command::iterator longest_fit_begin = e;
  const pragma_handler* longest_fit_handler = 0;
  int longest_fit_len = -1;

  typedef std::pair<const std::vector<std::string>, pragma_handler> np;
  BOOST_FOREACH(const np& p, _handlers)
  {
    if (
      const optional<command::iterator> i = is_this_pragma(p.first, begin_, e)
    )
    {
      if (longest_fit_len < int(p.first.size()))
      {
        longest_fit_begin = *i;
        longest_fit_handler = &p.second;
        longest_fit_len = p.first.size();
      }
    }
  }
  if (longest_fit_handler)
  {
    longest_fit_handler->run(longest_fit_begin, e);
  }
  else
  {
    throw exception("Pragma " + name_of_pragma(begin_, e) + " not found");
  }
}

pragma_handler_map::iterator pragma_handler_map::begin() const
{
  return _handlers.begin();
}

pragma_handler_map::iterator pragma_handler_map::end() const
{
  return _handlers.end();
}

pragma_handler_map pragma_handler_map::build_default(shell& shell_)
{
  using boost::bind;

  return
    pragma_handler_map()
      .add("help", pragma_help(shell_))
      .add(
        "verbose",
        pragma_switch(
          "verbose mode",
          bind(&shell::verbose, &shell_),
          bind(&shell::verbose, &shell_, _1),
          shell_
        )
      )
      .add(
        "precompiled_headers",
        pragma_switch(
          "precompiled header usage",
          bind(&shell::using_precompiled_headers, &shell_),
          bind(&shell::using_precompiled_headers, &shell_, _1),
          shell_
        )
      )
      .add("environment", pragma_environment(shell_))
      .add("environment", "push", pragma_environment_push(shell_))
      .add("environment", "pop", pragma_environment_pop(shell_))
      .add("environment", "stack", pragma_environment_stack(shell_))
      .add("environment", "add", pragma_environment_add(shell_))
      .add("environment", "reset", pragma_environment_reset(shell_))
      .add("environment", "reload", pragma_environment_reload(shell_))
      .add("evaluate", pragma_evaluate(shell_))
      .add("quit", pragma_quit(shell_))
    ;
}

pragma_handler_map::iterator pragma_handler_map::find(
  const std::vector<std::string>& p_
) const
{
  return _handlers.find(p_);
}

