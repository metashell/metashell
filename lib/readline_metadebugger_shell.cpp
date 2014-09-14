
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

#include <metashell/readline_metadebugger_shell.hpp>

namespace metashell {

readline_metadebugger_shell::readline_metadebugger_shell(
    const config& conf,
    const environment& env) :
  metadebugger_shell(conf, env) {}

void readline_metadebugger_shell::run() {
  for (boost::optional<std::string> line;
      !stopped() && (line = readline_env.readline(prompt())); )
  {
    line_available(*line);
  }
  std::cout << std::endl;
}

void readline_metadebugger_shell::add_history(const std::string& str) {
  readline_env.add_history(str);
}

void readline_metadebugger_shell::display(const colored_string& cs) const {
  std::cout << cs;
}

unsigned readline_metadebugger_shell::width() const {
  return readline_env.width();
}

}

