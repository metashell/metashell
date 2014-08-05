#ifndef METASHELL_TEST_SHELL_HPP
#define METASHELL_TEST_SHELL_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/shell.hpp>

#include <vector>

class test_shell : public metashell::shell
{
public:
  test_shell();
  test_shell(const metashell::config& cfg_, int width_);
  test_shell(std::vector<std::string>& history_);

  // Takes ownership of env_
  test_shell(const metashell::config& cfg_, metashell::environment* env_);

  virtual void add_history(const std::string& s_);

  virtual void display_normal(const std::string& s_) const;
  virtual void display_info(const std::string& s_) const;
  virtual void display_error(const std::string& s_) const;
  virtual void display(
      const std::string& s_,
      optional_color c_ = boost::none) const;

  virtual unsigned int width() const;

  const std::string& output() const;
  const std::string& info() const;
  const std::string& error() const;
private:
  mutable std::string _output;
  mutable std::string _info;
  mutable std::string _error;

  int _width;
  std::vector<std::string>* _history; // no ownership
};

#endif

