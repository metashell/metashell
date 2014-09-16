
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
    const environment& env) :
  mdb_shell(conf, env) {}

void readline_mdb_shell::run() {
  for (boost::optional<std::string> line;
      !stopped() && (line = readline_env.readline(prompt())); )
  {
    line_available(*line);
  }
  std::cout << std::endl;
}

void readline_mdb_shell::add_history(const std::string& str) {
  readline_env.add_history(str);
}

void readline_mdb_shell::display(
    const colored_string& cs,
    colored_string::size_type first,
    colored_string::size_type length) const
{
  cs.print_to_cout(first, length);
}

unsigned readline_mdb_shell::width() const {
  return readline_env.width();
}

}

