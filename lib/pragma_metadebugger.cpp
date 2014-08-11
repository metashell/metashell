
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

#include <metashell/shell.hpp>
#include <metashell/token_iterator.hpp>
#include <metashell/pragma_metadebugger.hpp>
#include <metashell/readline_metadebugger_shell.hpp>

using namespace metashell;

pragma_metadebugger::pragma_metadebugger(shell& shell_) :
  _shell(shell_)
{}

pragma_handler_interface* pragma_metadebugger::clone() const
{
  return new pragma_metadebugger(_shell);
}

std::string pragma_metadebugger::arguments() const
{
  return "";
}

std::string pragma_metadebugger::description() const
{
  return "Starts the metadebugger.";
}

void pragma_metadebugger::run(
  const token_iterator& args_begin_,
  const token_iterator& args_end_
) const
{
  std::string args = tokens_to_string(args_begin_, args_end_);
  std::cout << args << std::endl;

  readline_metadebugger_shell mdb_shell(_shell.get_config(), _shell.env(), args);
  mdb_shell.run();
}

