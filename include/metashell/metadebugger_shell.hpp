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

#include <just/console.hpp>

#include <metashell/config.hpp>
#include <metashell/environment.hpp>
#include <metashell/metaprogram.hpp>
#include <metashell/metadebugger_command_handler_map.hpp>

namespace metashell {

class metadebugger_shell {
public:
  typedef boost::optional<just::console::color> optional_color;

  static metadebugger_command_handler_map::commands_t
    create_default_command_map();

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

  std::string prompt() const;
  bool stopped() const;
  void line_available(const std::string& line);

  void command_continue(const std::string& arg);
  void command_step(const std::string& arg);
  void command_evaluate(const std::string& arg);
  void command_forwardtrace(const std::string& arg);
  void command_backtrace(const std::string& arg);
  void command_break(const std::string& arg);
  void command_help(const std::string& arg);
  void command_quit(const std::string& arg);

protected:
  // breakpoint is simply a regex for now
  typedef std::string breakpoint_t;
  typedef std::vector<breakpoint_t> breakpoints_t;

  bool require_empty_args(const std::string& args) const;
  bool require_running_metaprogram() const;

  void run_metaprogram_with_templight(const std::string& str);
  void run_metaprogram(const std::string& str);

  void continue_metaprogram();

  void display_error(const std::string& str) const;
  void display_info(const std::string& str) const;
  void display_current_frame() const;
  void display_current_forwardtrace() const;
  void display_backtrace() const;

  const config& conf;
  environment& env;
  metadebugger_command_handler_map command_handler;

  metaprogram mp;
  breakpoints_t breakpoints;

  // Optional is empty, when there is non previous line, or
  // the last command is non_repeatable
  boost::optional<std::string> prev_line;

  bool is_stopped;

private:
  // Helpers for display_forwardtrace
  typedef std::pair<
      std::string::const_iterator,
      std::string::const_iterator
    > string_range;

  string_range find_type_emphasize(const std::string& type) const;

  void display_range(
      std::string::const_iterator begin,
      std::string::const_iterator end,
      optional_color c) const;

  void display_trace_content(
      string_range range,
      string_range emphasize) const;

  void display_trace_graph(
      unsigned depth,
      const std::vector<unsigned>& depth_counter,
      bool print_mark) const;

  void display_trace_line(
      metaprogram::vertex_descriptor vertex,
      unsigned depth,
      const std::vector<unsigned>& depth_counter,
      const boost::optional<instantiation_kind>& kind,
      unsigned width) const;

  void display_trace_visit(
      metaprogram::vertex_descriptor root_vertex,
      metaprogram::discovered_t& discovered,
      unsigned width) const;

  void display_frame(const metaprogram::frame_t& frame) const;

  const static std::vector<just::console::color> colors;
};

}

#endif
