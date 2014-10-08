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
#include "mdb_test_shell.hpp"

#include <metashell/config.hpp>
#include <metashell/in_memory_environment.hpp>

mdb_test_shell::mdb_test_shell(const std::string& line) :
  metashell::mdb_shell(
      test_shell().get_config(),
      test_shell().env()
  )
{
  env.append(line);
}

void mdb_test_shell::run() {}

void mdb_test_shell::add_history(const std::string& str) {
  history.push_back(str);
}

void mdb_test_shell::display(
    const metashell::colored_string& cs,
    metashell::colored_string::size_type first,
    metashell::colored_string::size_type length) const
{
  output += cs.get_string().substr(first, length);
}

unsigned mdb_test_shell::width() const {
  return terminal_width;
}

void mdb_test_shell::set_terminal_width(unsigned new_width) {
  terminal_width = new_width;
}

bool mdb_test_shell::has_metaprogram() const {
  return static_cast<bool>(mp);
}

const metashell::metaprogram& mdb_test_shell::get_metaprogram() const {
  return *mp;
}

const metashell::mdb_shell::breakpoints_t&
mdb_test_shell::get_breakpoints() const {
  return breakpoints;
}

const std::string& mdb_test_shell::get_output() const {
  return output;
}

const mdb_test_shell::history_t&
mdb_test_shell::get_history() const {
  return history;
}

void mdb_test_shell::clear_output() {
  output.clear();
}

void mdb_test_shell::clear_history() {
  history.clear();
}

