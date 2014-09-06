#ifndef METASHELL_METADEBUGGER_TEST_SHELL_HPP
#define METASHELL_METADEBUGGER_TEST_SHELL_HPP

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

#include <vector>

#include <metashell/metadebugger_shell.hpp>

class metadebugger_test_shell : public metashell::metadebugger_shell {
public:
  typedef std::vector<std::string> history_t;

  virtual void run();

  virtual void add_history(const std::string& str);

  virtual void display(
      const std::string& str,
      optional_color color = boost::none) const;

  virtual unsigned width() const;

  const std::string& get_output() const;
  const history_t& get_history() const;
  void clear_output();
  void clear_history();

private:
  history_t history;
  mutable std::string output;
};

#endif
