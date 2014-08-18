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

#include <metashell/pragma_environment.hpp>
#include <metashell/shell.hpp>

using namespace metashell;

pragma_environment::pragma_environment(shell& shell_) :
  _shell(shell_)
{}

pragma_handler_interface* pragma_environment::clone() const
{
  return new pragma_environment(_shell);
}

std::string pragma_environment::arguments() const
{
  return "";
}

std::string pragma_environment::description() const
{
  return "Displays the entire content of the environment.";
}

void pragma_environment::run(
  const command::iterator& args_begin_,
  const command::iterator& args_end_
) const
{
  _shell.display_normal(_shell.env().get_all());
}

