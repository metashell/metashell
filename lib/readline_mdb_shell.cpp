
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

#include <iostream>

#include <boost/optional.hpp>

#include <metashell/readline_mdb_shell.hpp>

namespace metashell {

readline_mdb_shell::readline_mdb_shell(
    const config& conf,
    const environment& env,
    iface::displayer& displayer_) :
  mdb_shell(conf, env, displayer_) {}

void readline_mdb_shell::run() {
  for (boost::optional<std::string> line;
      !stopped() && (line = readline_env.readline(prompt())); )
  {
    line_available(*line);
  }
  displayer().show_raw_text("");
}

}

