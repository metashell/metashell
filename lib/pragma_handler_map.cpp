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

#include "exception.hpp"

#include <boost/bind.hpp>

using namespace metashell;

void pragma_handler_map::process(const metashell_pragma& p_) const
{
  const std::map<std::string, pragma_handler>::const_iterator i =
    _handlers.find(p_.name());

  if (i == _handlers.end())
  {
    throw exception("Pragma " + p_.name() + " not found");
  }
  else
  {
    i->second.run(p_);
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
      .add("quit", pragma_quit(shell_))
    ;
}

