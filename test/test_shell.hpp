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
#include <memory>

class test_shell : public metashell::shell
{
public:
  explicit test_shell(metashell::iface::displayer& displayer_);
  test_shell(
    const metashell::config& cfg_,
    metashell::iface::displayer& displayer_
  );
  test_shell(
    std::vector<std::string>& history_,
    metashell::iface::displayer& displayer_
  );

  // Takes ownership of env_
  test_shell(
    const metashell::config& cfg_,
    std::unique_ptr<metashell::environment> env_,
    metashell::iface::displayer& displayer_
  );

  virtual void add_history(const std::string& s_) override;
private:
  std::vector<std::string>* _history; // no ownership
};

#endif

