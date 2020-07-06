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

#include <metashell/mdb/breakpoint.hpp>
#include <metashell/mdb/command_handler_map.hpp>
#include <metashell/mdb/metaprogram.hpp>

#include <metashell/core/logger.hpp>

#include <metashell/iface/call_graph.hpp>
#include <metashell/iface/command_processor.hpp>
#include <metashell/iface/displayer.hpp>
#include <metashell/iface/engine.hpp>
#include <metashell/iface/environment.hpp>
#include <metashell/iface/history.hpp>

#include <metashell/data/mdb_command.hpp>

#include <optional>
#include <string>

namespace metashell
{
  namespace mdb
  {
    class shell : public iface::command_processor
    {
    public:
      const command_handler_map command_handler;

      shell(iface::environment& env,
            iface::engine& engine_,
            const boost::filesystem::path& env_path_,
            const boost::filesystem::path& mdb_temp_dir_,
            bool _preprocessor,
            core::logger* logger_);

      virtual std::string prompt() const override;
      virtual bool stopped() const override;

      void display_splash(iface::displayer& displayer_) const;
      virtual void line_available(const data::user_input& line,
                                  iface::displayer& displayer_,
                                  iface::history& history_) override;
      void line_available(const data::user_input& line,
                          iface::displayer& displayer_);
      virtual void cancel_operation() override;

      void command_continue(const data::mdb_command::arguments_type& arg,
                            iface::displayer& displayer_);
      void command_finish(const data::mdb_command::arguments_type& arg,
                          iface::displayer& displayer_);
      void command_step(const data::mdb_command::arguments_type& arg,
                        iface::displayer& displayer_);
      void command_next(const data::mdb_command::arguments_type& arg,
                        iface::displayer& displayer_);
      void command_evaluate(const data::mdb_command::arguments_type& arg,
                            iface::displayer& displayer_);
      void command_forwardtrace(const data::mdb_command::arguments_type& arg,
                                iface::displayer& displayer_);
      void command_backtrace(const data::mdb_command::arguments_type& arg,
                             iface::displayer& displayer_);
      void command_frame(const data::mdb_command::arguments_type& arg,
                         iface::displayer& displayer_);
      void command_rbreak(const data::mdb_command::arguments_type& arg,
                          iface::displayer& displayer_);
      void command_break(const data::mdb_command::arguments_type& arg,
                         iface::displayer& displayer_);
      void command_help(const data::mdb_command::arguments_type& arg,
                        iface::displayer& displayer_);
      void command_quit(const data::mdb_command::arguments_type& arg,
                        iface::displayer& displayer_);

      virtual data::code_completion
      code_complete(const data::user_input& s_,
                    bool metashell_extensions_) override;

      static command_handler_map build_command_handler(bool preprocessor_);

    protected:
      bool require_empty_args(const data::mdb_command::arguments_type& args,
                              iface::displayer& displayer_) const;
      bool require_evaluated_metaprogram(iface::displayer& displayer_) const;
      bool require_running_metaprogram(iface::displayer& displayer_);
      bool require_running_or_errored_metaprogram(iface::displayer& displayer_);

      bool run_metaprogram_with_templight(
          const std::optional<data::cpp_code>& expression,
          data::metaprogram_mode mode,
          bool caching_enabled,
          iface::displayer& displayer_);

      // may return nullptr
      const breakpoint* continue_metaprogram(data::direction_t direction);
      unsigned finish_metaprogram();

      void next_metaprogram(data::direction_t direction, int n);

      void display_frame(const data::frame& frame,
                         iface::displayer& displayer_) const;
      void display_current_frame(iface::displayer& displayer_) const;
      void display_current_forwardtrace(std::optional<int> max_depth,
                                        iface::displayer& displayer_);
      void display_backtrace(iface::displayer& displayer_);
      void display_metaprogram_reached_the_beginning(
          iface::displayer& displayer_) const;
      void display_metaprogram_finished(iface::displayer& displayer_);
      void display_movement_info(bool moved, iface::displayer& displayer_);

      std::optional<data::mdb_command>
      command_to_execute(const data::user_input&, iface::history&);

      iface::environment& env;

      std::optional<metaprogram> mp;

      int next_breakpoint_id = 1;
      breakpoints_t breakpoints;

      std::optional<data::mdb_command> prev_line;
      bool last_command_repeatable = false;

      // It is empty if evaluate was called with "-".
      // mp is empty when there were no evaluations at all
      std::optional<data::cpp_code> last_evaluated_expression;

      bool is_stopped = false;
      core::logger* _logger;
      iface::engine& _engine;
      boost::filesystem::path _env_path;
      boost::filesystem::path _mdb_temp_dir;

      bool _preprocessor;
    };
  }
}

#endif
