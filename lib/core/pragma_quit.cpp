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

#include <metashell/core/pragma_quit.hpp>
#include <metashell/core/shell.hpp>

using namespace metashell;

pragma_quit::pragma_quit(shell& shell_) : _shell(shell_) {}

iface::pragma_handler* pragma_quit::clone() const
{
  return new pragma_quit(_shell);
}

std::string pragma_quit::description() const { return "Terminates the shell."; }

void pragma_quit::run(iface::displayer&) const { _shell.stop(); }
