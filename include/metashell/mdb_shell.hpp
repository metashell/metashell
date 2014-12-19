#ifndef METASHELL_MDB_SHELL_HPP
#define METASHELL_MDB_SHELL_HPP

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

#include <boost/regex.hpp>
#include <boost/optional.hpp>

#include <metashell/config.hpp>
#include <metashell/metaprogram.hpp>
#include <metashell/colored_string.hpp>
#include <metashell/templight_environment.hpp>
#include <metashell/mdb_command_handler_map.hpp>

#include <metashell/iface/call_graph.hpp>
#include <metashell/iface/displayer.hpp>
#include <metashell/iface/history.hpp>
#include <metashell/iface/command_processor.hpp>

namespace metashell {

class mdb_shell : public iface::command_processor {
public:
  const static mdb_command_handler_map command_handler;

  mdb_shell(
      const config& conf,
      const environment& env);

  virtual std::string prompt() const override;
  virtual bool stopped() const override;

  void display_splash(iface::displayer& displayer_) const;
  virtual void line_available(
    const std::string& line,
    iface::displayer& displayer_,
    iface::history& history_
  ) override;
  void line_available(const std::string& line, iface::displayer& displayer_);
  virtual void cancel_operation() override;

  void command_continue(const std::string& arg, iface::displayer& displayer_);
  void command_step(const std::string& arg, iface::displayer& displayer_);
  void command_evaluate(const std::string& arg, iface::displayer& displayer_);
  void command_forwardtrace(
    const std::string& arg,
    iface::displayer& displayer_
  );
  void command_backtrace(const std::string& arg, iface::displayer& displayer_);
  void command_rbreak(const std::string& arg, iface::displayer& displayer_);
  void command_help(const std::string& arg, iface::displayer& displayer_);
  void command_quit(const std::string& arg, iface::displayer& displayer_);

  virtual void code_complete(
    const std::string& s_,
    std::set<std::string>& out_
  ) const override;
protected:
  // breakpoint is simply a regex for now
  typedef std::tuple<std::string, boost::regex> breakpoint_t;
  typedef std::vector<breakpoint_t> breakpoints_t;

  bool breakpoint_match(
      metaprogram::vertex_descriptor vertex, const breakpoint_t& breakpoint);

  bool require_empty_args(
    const std::string& args,
    iface::displayer& displayer_
  ) const;
  bool require_evaluated_metaprogram(iface::displayer& displayer_) const;
  bool require_running_metaprogram(iface::displayer& displayer_) const;

  bool run_metaprogram_with_templight(
    const std::string& str,
    bool full_mode,
    iface::displayer& displayer_
  );
  boost::optional<type> run_metaprogram(
    const std::string& str,
    iface::displayer& displayer_
  );

  bool is_wrap_type(const std::string& type);
  std::string trim_wrap_type(const std::string& type);

  void filter_disable_everything();
  void filter_enable_reachable_from_current_line();
  void filter_unwrap_vertices();
  void filter_similar_edges();
  void filter_metaprogram();

  breakpoints_t::iterator continue_metaprogram(
      metaprogram::direction_t direction);

  void display_current_frame(iface::displayer& displayer_) const;
  void display_current_forwardtrace(
    boost::optional<int> max_depth,
    iface::displayer& displayer_
  ) const;
  void display_backtrace(iface::displayer& displayer_) const;
  void display_argument_parsing_failed(iface::displayer& displayer_) const;
  void display_metaprogram_reached_the_beginning(
    iface::displayer& displayer_
  ) const;
  void display_metaprogram_finished(iface::displayer& displayer_) const;

  config conf;
  templight_environment env;

  boost::optional<metaprogram> mp;
  breakpoints_t breakpoints;

  std::string prev_line;
  bool last_command_repeatable = false;

  bool is_stopped = false;
};

}

#endif
