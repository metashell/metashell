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

#include <metashell/core/null_history.hpp>
#include <metashell/core/pragma_macro.hpp>

namespace metashell
{
  namespace core
  {
    pragma_macro::pragma_macro(std::string description_,
                               std::vector<std::string> commands_,
                               iface::command_processor& shell_)
      : _shell(shell_),
        _commands(move(commands_)),
        _description(move(description_))
    {
    }

    iface::pragma_handler* pragma_macro::clone() const
    {
      return new pragma_macro(_description, _commands, _shell);
    }

    std::string pragma_macro::description() const { return _description; }

    void pragma_macro::run(iface::displayer& displayer_) const
    {
      null_history ignore;

      for (const std::string& cmd : _commands)
      {
        _shell.line_available(cmd, displayer_, ignore);
      }
    }
  }
}
