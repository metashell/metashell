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
#include <metashell/pragma_macro.hpp>
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
#include <metashell/pragma_pp.hpp>
#include <metashell/pragma_echo.hpp>

#include <cassert>
#include <sstream>

using namespace metashell;

namespace
{
  bool can_be_part_of_name(const data::token& t_)
  {
    return t_.type() == data::token_type::identifier;
  }

  boost::optional<data::command::iterator> is_this_pragma(
    const std::vector<std::string>& name_,
    data::command::iterator begin_,
    const data::command::iterator& end_
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
        optional<data::command::iterator>(begin_) :
        optional<data::command::iterator>();
  }

  std::string name_of_pragma(
    data::command::iterator begin_,
    const data::command::iterator& end_
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

  std::string on_off(bool v_)
  {
    return v_ ? "on" : "off";
  }

  pragma_macro shell_mode(
    const std::string& name_,
    const std::string& comment_,
    bool preprocessing_mode_,
    iface::command_processor& shell_
  )
  {
    std::vector<std::string>
      cmds{
        "#msh preprocessed echo " + on_off(preprocessing_mode_),
        "#msh show cpp_errors " + on_off(!preprocessing_mode_),
        "#msh metaprogram evaluation " + on_off(!preprocessing_mode_)
      };

    if (!comment_.empty())
    {
      cmds.push_back("#msh echo " + comment_);
    }

    return
      pragma_macro("Set Metashell to " + name_ + " mode", move(cmds), shell_);
  }
}

void pragma_handler_map::process(
  const data::command::iterator& begin_,
  const data::command::iterator& end_,
  iface::displayer& displayer_
) const
{
  using boost::optional;

  const data::command::iterator e = end_of_pragma_argument_list(begin_, end_);

  auto longest_fit_begin = e;
  const pragma_handler* longest_fit_handler = 0;
  int longest_fit_len = -1;

  typedef std::pair<const std::vector<std::string>, pragma_handler> np;
  for (const np& p : _handlers)
  {
    if (
      const optional<data::command::iterator>
        i = is_this_pragma(p.first, begin_, e)
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
    longest_fit_handler->run(
      begin_,
      longest_fit_begin,
      longest_fit_begin,
      e,
      displayer_
    );
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
  command_processor_queue* cpq_,
  logger* logger_
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
      .add(
        "preprocessed", "echo",
        pragma_switch(
          "display preprocessed commands",
          [&shell_] () { return shell_.echo(); },
          [&shell_] (bool v_) { shell_.echo(v_); }
        )
      )
      .add("mdb", pragma_mdb(shell_, cpq_, logger_))
      .add("evaluate", pragma_evaluate(shell_))
      .add("pp", pragma_pp(shell_))
      .add(
        "show", "cpp_errors",
        pragma_switch(
          "display C++ errors",
          [&shell_] () { return shell_.show_cpp_errors(); },
          [&shell_] (bool v_) { shell_.show_cpp_errors(v_); }
        )
      )
      .add(
        "metaprogram", "evaluation",
        pragma_switch(
          "evaluation of metaprograms",
          [&shell_] () { return shell_.evaluate_metaprograms(); },
          [&shell_] (bool v_) { shell_.evaluate_metaprograms(v_); }
        )
      )
      .add(
        "preprocessor", "mode",
        shell_mode(
          "preprocessor",
          "To switch back to the default mode, run #msh metaprogram mode",
          true,
          shell_
        )
      )
      .add("metaprogram", "mode", shell_mode("metaprogram", "", false, shell_))
      .add("echo", pragma_echo())
      .add("quit", pragma_quit(shell_))
    ;
}

pragma_handler_map::iterator pragma_handler_map::find(
  const std::vector<std::string>& p_
) const
{
  return _handlers.find(p_);
}

