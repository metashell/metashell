#ifndef READLINE_SHELL_HPP
#define READLINE_SHELL_HPP

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
#include <metashell/config.hpp>
#include <metashell/readline_environment.hpp>

#include <vector>
#include <string>

class readline_shell : public metashell::shell
{
public:
  readline_shell(
    const metashell::config& config_,
    metashell::iface::displayer& displayer_
  );
  virtual ~readline_shell() override;

  virtual void add_history(const std::string& s_) override;

  void run();
private:
  metashell::readline_environment _readline_environment;

  static char* tab_generator(const char* text_, int state_);
  static char** tab_completion(const char* text_, int start_, int end_);

  static readline_shell* _instance;

  static int _completion_end;
};

#endif

