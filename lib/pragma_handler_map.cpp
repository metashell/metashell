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
#include <metashell/exception.hpp>

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
#include <metashell/pragma_environment_save.hpp>
#include <metashell/pragma_mdb.hpp>
#include <metashell/pragma_evaluate.hpp>

#include <cassert>
#include <iostream>
#include <sstream>

using namespace metashell;

namespace
{
  bool can_be_part_of_name(const token& t_)
  {
    return t_.type() == token_type::identifier;
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
      && *i == begin_->value())
    {
      ++i;
      begin_ = skip_whitespace(skip(begin_), end_);
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
      begin_ = skip_whitespace(skip(begin_), end_)
    )
    {
      s << (first ? "" : " ") << begin_->value();
      first = false;
    }
    return s.str();
  }
}

void pragma_handler_map::process(
  const command::iterator& begin_,
  const command::iterator& end_,
  iface::displayer& displayer_
) const
{
  using boost::optional;

  const command::iterator e = end_of_pragma_argument_list(begin_, end_);

  command::iterator longest_fit_begin = e;
  const pragma_handler* longest_fit_handler = 0;
  int longest_fit_len = -1;

  typedef std::pair<const std::vector<std::string>, pragma_handler> np;
  for (const np& p : _handlers)
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
    longest_fit_handler->run(longest_fit_begin, e, displayer_);
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

pragma_handler_map pragma_handler_map::build_default(
  shell& shell_,
  command_processor_queue* cpq_
)
{
  return
    pragma_handler_map()
      .add("help", pragma_help(shell_.pragma_handlers()))
      .add(
        "verbose",
        pragma_switch(
          "verbose mode",
          [&shell_] () { return shell_.verbose(); },
          [&shell_] (bool v_) { shell_.verbose(v_); }
        )
      )
      .add(
        "precompiled_headers",
        pragma_switch(
          "precompiled header usage",
          [&shell_] () { return shell_.using_precompiled_headers(); },
          [&shell_] (bool v_) { shell_.using_precompiled_headers(v_); }
        )
      )
      .add("environment", pragma_environment(shell_.env()))
      .add("environment", "push", pragma_environment_push(shell_))
      .add("environment", "pop", pragma_environment_pop(shell_))
      .add("environment", "stack", pragma_environment_stack(shell_))
      .add("environment", "add", pragma_environment_add(shell_))
      .add("environment", "reset", pragma_environment_reset(shell_))
      .add("environment", "reload", pragma_environment_reload(shell_))
      .add(
        "environment",
        "save",
        pragma_environment_save(shell_.get_config(), shell_.env())
      )
      .add("mdb", pragma_mdb(shell_, cpq_))
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

