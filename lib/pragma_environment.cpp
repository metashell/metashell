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
  pragma_without_arguments(shell_, "environment")
{}

pragma_handler_interface* pragma_environment::clone() const
{
  return new pragma_environment(get_shell());
}

std::string pragma_environment::description() const
{
  return "Displays the entire content of the environment.";
}

void pragma_environment::run() const
{
  get_shell().display_normal(get_shell().env().get_all());
}

