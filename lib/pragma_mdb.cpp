
// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/pragma_mdb.hpp>
#include <metashell/shell.hpp>
#include <metashell/readline_mdb_shell.hpp>

using namespace metashell;

pragma_mdb::pragma_mdb(
  iface::displayer& displayer_,
  const config& config_,
  environment& env_
) :
  _displayer(displayer_),
  _config(config_),
  _env(env_)
{}

iface::pragma_handler* pragma_mdb::clone() const
{
  return new pragma_mdb(_displayer, _config, _env);
}

std::string pragma_mdb::arguments() const
{
  return "[-full] [<type>]";
}

std::string pragma_mdb::description() const
{
  return "Starts the metadebugger. For more information see evaluate in the "
    "Metadebugger command reference.";
}

void pragma_mdb::run(
  const command::iterator& args_begin_,
  const command::iterator& args_end_
) const
{
  std::string args = tokens_to_string(args_begin_, args_end_);

  readline_mdb_shell mdb_shell(_config, _env, _displayer);
  mdb_shell.display_splash();

  if (!args.empty()) {
    mdb_shell.command_evaluate(args);
  }

  mdb_shell.run();
}

