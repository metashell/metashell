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

#include "test_shell.hpp"
#include "metadebugger_test_shell.hpp"

#include <metashell/config.hpp>
#include <metashell/in_memory_environment.hpp>

metadebugger_test_shell::metadebugger_test_shell(const std::string& line) :
  metashell::metadebugger_shell(
      test_shell().get_config(),
      test_shell().env()
  )
{
  env.append(line);
}

void metadebugger_test_shell::run() {}

void metadebugger_test_shell::add_history(const std::string& str) {
  history.push_back(str);
}

void metadebugger_test_shell::display(
    const metashell::colored_string& cs) const
{
  //TODO We don't test colors at the moment
  output += cs.get_string();
}

unsigned metadebugger_test_shell::width() const {
  return 80;
}

const std::string& metadebugger_test_shell::get_output() const {
  return output;
}

const metadebugger_test_shell::history_t&
metadebugger_test_shell::get_history() const {
  return history;
}

void metadebugger_test_shell::clear_output() {
  output.clear();
}

void metadebugger_test_shell::clear_history() {
  history.clear();
}

