// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/core/pragma_macros.hpp>

namespace metashell
{
  namespace core
  {
    pragma_macros::pragma_macros(shell& shell_) : _shell(shell_) {}

    iface::pragma_handler* pragma_macros::clone() const
    {
      return new pragma_macros(_shell);
    }

    std::string pragma_macros::description() const
    {
      return "Displays the macro definitions";
    }

    void pragma_macros::run(iface::displayer& displayer_) const
    {
      displayer_.show_cpp_code(
          _shell.engine().macro_discovery().macros(_shell.env()));
    }
  }
}
