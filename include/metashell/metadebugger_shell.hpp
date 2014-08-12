#ifndef METASHELL_METADEBUGGER_SHELL_HPP
#define METASHELL_METADEBUGGER_SHELL_HPP

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

#include <string>

#include <boost/optional.hpp>

#include <metashell/config.hpp>
#include <metashell/environment.hpp>
#include <metashell/templight_trace.hpp>
#include <metashell/readline_environment.hpp>

namespace metashell {

class metadebugger_shell {
public:
  typedef boost::optional<just::console::color> optional_color;

  metadebugger_shell(
      const config& conf,
      environment& env);

  virtual ~metadebugger_shell();

  virtual void run() = 0;

  virtual void add_history(const std::string& str) = 0;

  virtual void display(
      const std::string& str,
      optional_color color = boost::none) const = 0;

  virtual unsigned width() const = 0;

protected:
  std::string prompt() const;
  bool stopped() const;
  void line_available(const std::string& line);

  void get_templight_trace_from_metaprogram(const std::string& str);
  // TODO put this method somewhere global (shell has it too)
  void run_metaprogram(const std::string& str);

  const config& conf;
  environment& env;

  templight_trace trace;

  std::string prev_line;
  bool is_stopped;
};

}

#endif
