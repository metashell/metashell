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

#include "metadebugger_test_shell.hpp"

#include <metashell/shell_stub.hpp>

metadebugger_test_shell metadebugger_test_shell::create_default() {
  return create_with_environment({});
}

metadebugger_test_shell metadebugger_test_shell::create_with_environment(
      const std::vector<std::string>& lines)
{
  std::shared_ptr<metashell::shell> sh(new metashell::shell_stub());

  for (const std::string& line : lines) {
    sh->store_in_buffer(line);
  }

  return metadebugger_test_shell(sh);
}

metadebugger_test_shell::metadebugger_test_shell(
  std::shared_ptr<metashell::shell> sh_) :
  metashell::metadebugger_shell(sh_->get_config(), sh_->env()), sh(sh_)
{}

void metadebugger_test_shell::run() {}

void metadebugger_test_shell::add_history(const std::string& str) {
  history.push_back(str);
}

void metadebugger_test_shell::display(
    const std::string& str,
    optional_color) const
{
  //TODO We don't test colors at the moment
  output += str;
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

