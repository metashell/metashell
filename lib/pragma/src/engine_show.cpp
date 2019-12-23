// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/pragma/engine_show.hpp>

namespace metashell
{
  namespace pragma
  {
    std::string engine_show::description() const
    {
      return "Displays information about the current engine.";
    }

    void engine_show::run(iface::main_shell& shell_,
                          iface::displayer& displayer_) const
    {
      const iface::engine& engine = shell_.engine();

      displayer_.show_comment(data::text("{\"name\":\"" + engine.name() +
                                         "\",\"display_name\":\"" +
                                         engine.display_name() + "\"}"));
    }
  }
}
