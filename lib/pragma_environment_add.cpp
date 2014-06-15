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

#include <metashell/pragma_environment_add.hpp>
#include <metashell/shell.hpp>

using namespace metashell;

pragma_environment_add::pragma_environment_add(shell& shell_) :
  _shell(shell_)
{}

pragma_handler_interface* pragma_environment_add::clone() const
{
  return new pragma_environment_add(_shell);
}

std::string pragma_environment_add::arguments() const
{
  return "<code>";
}

std::string pragma_environment_add::description() const
{
  return
    "Appends code to the environment. Use this if Metashell thinks about the"
    " code that it is an evaluation.";
}

void pragma_environment_add::run(
  const token_iterator& args_begin_,
  const token_iterator& args_end_
) const
{
  _shell.store_in_buffer(tokens_to_string(args_begin_, args_end_));
}

