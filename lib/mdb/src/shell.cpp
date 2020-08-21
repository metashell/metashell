
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

#include <metashell/mdb/caching_disabled.hpp>
#include <metashell/mdb/forward_trace_iterator.hpp>
#include <metashell/mdb/shell.hpp>

#include <metashell/core/highlight_syntax.hpp>
#include <metashell/core/null_history.hpp>

#include <metashell/data/exception.hpp>
#include <metashell/data/mdb_usage.hpp>
#include <metashell/data/some_feature_not_supported.hpp>

#include <boost/range/iterator_range.hpp>

#include <cmath>
#include <optional>
#include <stdexcept>

namespace metashell
{
  namespace mdb
  {
    namespace
    {
      std::function<void(
          shell&, const data::mdb_command::arguments_type&, iface::displayer&)>
      callback(void (shell::*f)(const data::mdb_command::arguments_type&,
                                iface::displayer&))
      {
        return std::bind(f,
                         std::placeholders::_1,
                         std::placeholders::_2,
                         std::placeholders::_3);
      }
    }

    shell::shell(iface::environment& env_arg,
                 iface::engine& engine_,
                 const boost::filesystem::path& env_path_,
                 const boost::filesystem::path& mdb_temp_dir_,
                 bool preprocessor_,
                 core::logger* logger_)
      : command_handler(build_command_handler(preprocessor_)),
        env(env_arg),
        _logger(logger_),
        _engine(engine_),
        _env_path(env_path_),
        _mdb_temp_dir(mdb_temp_dir_),
        _preprocessor(preprocessor_)
    {
    }

    command_handler_map shell::build_command_handler(bool preprocessor_)
    {
      using name_type = data::mdb_command::name_type;

      const std::string expr = preprocessor_ ? "<expression>" : "<type>";
      // clang-format off
    return
      command_handler_map({
        {{name_type("evaluate")}, repeatable_t::non_repeatable,
          callback(&shell::command_evaluate),
          data::mdb_usage(preprocessor_),
          "Evaluate and start debugging a new metaprogram.",
          std::string(preprocessor_ ? "" :
            "Evaluating a metaprogram using the `-full` qualifier will expand all\n"
            "Memoization events.\n\n"
          ) +
          "Evaluating a metaprogram using the `-profile` qualifier will enable\n"
          "profile mode.\n\n"
          "Instead of `" + expr + "`, evaluate can be called with `-`, in which case the\n"
          "whole environment is being debugged not just a single type expression.\n\n"
          "If called without `" + expr + "` or `-`, then the last evaluated metaprogram will\n"
          "be reevaluated.\n\n"
          "Previous breakpoints are cleared.\n\n" +
          "Evaluating a metaprogram using the `-nocache` qualifier will disable caching of the events, which will prevent stepping backwards, predicting how many times a breakpoint will be hit and displaying forwardtrace." +
          std::string(preprocessor_ ? "" :
            "\n\nUnlike metashell, evaluate doesn't use metashell::format to avoid cluttering\n"
            "the debugged metaprogram with unrelated code. If you need formatting, you can\n"
            "explicitly enter `metashell::format< <type> >::type` for the same effect."
          )},
        {{name_type("step")}, repeatable_t::repeatable,
          callback(&shell::command_step),
          "[over|out] [n]",
          "Step the program.",
          "Argument n means step n times. n defaults to 1 if not specified.\n"
          "Negative n means step the program backwards.\n\n"
          "`step over` is an alias for next.\n"
          "Use of the `out` qualifier will jump out of the current instantiation frame.\n"
          "Similarly to `next`, `step out -1` is not always the inverse of `step out`."},
        {{name_type("next")}, repeatable_t::repeatable,
          callback(&shell::command_next),
          "[n]",
          "Jump over to the next instantiation skipping sub instantiations.",
          "Argument n means jump n times. n defaults to 1 if not specified.\n"
          "Negative n means step the program backwards.\n\n"
          "Please note that `next -1` is not always the inverse of `next`.\n"
          "In particular when there are no more instantiations that got instantiated\n"
          "by the current parent, then `next` will behave like a normal `step`,\n"
          "and will step out of one or more instantiation frames.\n\n"
          "`step over` is an alias for next."},
        {{name_type("rbreak")}, repeatable_t::non_repeatable,
          callback(&shell::command_rbreak),
          "<regex>",
          "Add breakpoint for all types matching `<regex>`.",
          ""},
        {{name_type("break")}, repeatable_t::non_repeatable,
          callback(&shell::command_break),
          "list",
          "List breakpoints.",
          ""},
        {{name_type("continue")}, repeatable_t::repeatable,
          callback(&shell::command_continue),
          "[n]",
          "Continue program being debugged.",
          "The program is continued until the nth breakpoint or the end of the program\n"
          "is reached. n defaults to 1 if not specified.\n"
          "Negative n means continue the program backwards."},
        {{name_type("finish")}, repeatable_t::repeatable,
          callback(&shell::command_finish),
          "",
          "Finish program being debugged.",
          "The program is continued until the end ignoring any breakpoints."},
        {{name_type("forwardtrace"), name_type("ft")}, repeatable_t::non_repeatable,
          callback(&shell::command_forwardtrace),
          "[n]",
          "Print forwardtrace from the current point.",
          "The n specifier limits the depth of the trace. If n is not specified, then the\n"
          "trace depth is unlimited."},
        {{name_type("backtrace"), name_type("bt")}, repeatable_t::non_repeatable,
          callback(&shell::command_backtrace),
          "",
          "Print backtrace from the current point.",
          ""},
        {{name_type("frame"), name_type("f")}, repeatable_t::non_repeatable,
          callback(&shell::command_frame),
          "n",
          "Inspect the nth frame of the current backtrace.",
          ""},
        {{name_type("help")}, repeatable_t::non_repeatable,
          callback(&shell::command_help),
          "[<command>]",
          "Show help for commands.",
          "If <command> is not specified, show a list of all available commands."},
        {{name_type("quit")} , repeatable_t::non_repeatable,
          callback(&shell::command_quit),
          "",
          "Quit metadebugger.",
          ""}
      });
      // clang-format on
    }

    std::string shell::prompt() const
    {
      return _preprocessor ? "(pdb)" : "(mdb)";
    }

    bool shell::stopped() const { return is_stopped; }

    void shell::display_splash(iface::displayer& displayer_) const
    {
      displayer_.show_raw_text("For help, type \"help\".");
    }

    std::optional<data::mdb_command>
    shell::command_to_execute(const data::user_input& line_arg,
                              iface::history& history_)
    {
      if (auto line = std::optional<data::mdb_command>(line_arg))
      {
        if (line != prev_line)
        {
          history_.add(line_arg);
        }

        prev_line = line;
        last_command_repeatable = false;

        return line;
      }
      else
      {
        return (empty(line_arg) && last_command_repeatable && prev_line) ?
                   prev_line :
                   std::nullopt;
      }
    }

    void shell::line_available(const data::user_input& line_arg,
                               iface::displayer& displayer_,
                               iface::history& history_)
    {
      try
      {
        if (std::optional<data::mdb_command> line =
                command_to_execute(line_arg, history_))
        {
          if (auto cmd = command_handler.get_command(line->name()))
          {
            last_command_repeatable = cmd->is_repeatable();
            cmd->get_func()(*this, line->arguments(), displayer_);
          }
          else
          {
            displayer_.show_error("Command parsing failed\n");
            last_command_repeatable = false;
          }
        }
      }
      catch (const std::exception& ex)
      {
        displayer_.show_error(std::string("Error: ") + ex.what() + "\n");
      }
      catch (...)
      {
        displayer_.show_error("Unknown error\n");
      }
    }

    bool
    shell::require_empty_args(const data::mdb_command::arguments_type& args,
                              iface::displayer& displayer_) const
    {
      if (!empty(args))
      {
        displayer_.show_error("This command doesn't accept arguments");
        return false;
      }
      return true;
    }

    bool
    shell::require_evaluated_metaprogram(iface::displayer& displayer_) const
    {
      if (!mp)
      {
        displayer_.show_error("Metaprogram not evaluated yet");
        return false;
      }
      return true;
    }

    bool shell::require_running_metaprogram(iface::displayer& displayer_)
    {
      if (!require_evaluated_metaprogram(displayer_))
      {
        return false;
      }

      if (mp->is_finished())
      {
        display_metaprogram_finished(displayer_);
        return false;
      }
      return true;
    }

    bool
    shell::require_running_or_errored_metaprogram(iface::displayer& displayer_)
    {
      if (!require_evaluated_metaprogram(displayer_))
      {
        return false;
      }

      if (!mp->is_finished() || mp->get_evaluation_result().is_error())
      {
        return true;
      }
      else
      {
        display_metaprogram_finished(displayer_);
        return false;
      }
    }

    void shell::command_continue(const data::mdb_command::arguments_type& arg,
                                 iface::displayer& displayer_)
    {
      if (!require_evaluated_metaprogram(displayer_))
      {
        return;
      }

      const int continue_count = empty(arg) ? 1 : int(arg);

      data::direction_t direction = continue_count >= 0 ?
                                        data::direction_t::forward :
                                        data::direction_t::backwards;

      const breakpoint* breakpoint_ptr = nullptr;
      for (int i = 0;
           i < std::abs(continue_count) && !mp->is_at_endpoint(direction); ++i)
      {
        breakpoint_ptr = continue_metaprogram(direction);
      }

      if (breakpoint_ptr)
      {
        displayer_.show_raw_text(breakpoint_ptr->to_string() + " reached");
      }
      display_movement_info(continue_count != 0, displayer_);
    }

    void shell::command_finish(const data::mdb_command::arguments_type& arg,
                               iface::displayer& displayer_)
    {
      if (!require_empty_args(arg, displayer_) ||
          !require_evaluated_metaprogram(displayer_))
      {
        return;
      }

      auto steps = finish_metaprogram();

      display_movement_info(steps > 0, displayer_);
    }

    void shell::command_step(const data::mdb_command::arguments_type& arg,
                             iface::displayer& displayer_)
    {
      if (!require_evaluated_metaprogram(displayer_))
      {
        return;
      }

      int step_count = 1;

      enum class step_t
      {
        normal,
        over,
        out
      };

      step_t step_type = step_t::normal;

      auto begin = arg.begin();
      const auto end = arg.end();
      if (begin != end)
      {
        if (*begin == "over")
        {
          step_type = step_t::over;
          ++begin;
        }
        else if (*begin == "out")
        {
          step_type = step_t::out;
          ++begin;
        }
      }
      if (begin != end)
      {
        step_count = int(*begin);
        ++begin;
      }
      if (begin != end)
      {
        throw data::exception("Unexpected argument: " + join(begin, end));
      }

      data::direction_t direction = step_count >= 0 ?
                                        data::direction_t::forward :
                                        data::direction_t::backwards;

      int iteration_count = std::abs(step_count);

      switch (step_type)
      {
      case step_t::normal:
        for (int i = 0; i < iteration_count && !mp->is_at_endpoint(direction);
             ++i)
        {
          mp->step(direction);
        }
        break;
      case step_t::over:
        next_metaprogram(direction, iteration_count);
        break;
      case step_t::out:
      {
        for (int i = 0; i < iteration_count && !mp->is_at_endpoint(direction);
             ++i)
        {
          const auto bt_depth = mp->get_backtrace().size();
          while (!mp->is_at_endpoint(direction) &&
                 mp->get_backtrace().size() + 1 > bt_depth)
          {
            mp->step(direction);
          }
        }
      }
      break;
      default:
        assert(false);
        break;
      }

      display_movement_info(step_count != 0, displayer_);
    }

    void shell::command_next(const data::mdb_command::arguments_type& arg,
                             iface::displayer& displayer_)
    {
      if (!require_evaluated_metaprogram(displayer_))
      {
        return;
      }

      const int next_count = empty(arg) ? 1 : int(arg);

      next_metaprogram(next_count >= 0 ? data::direction_t::forward :
                                         data::direction_t::backwards,
                       std::abs(next_count));

      display_movement_info(next_count != 0, displayer_);
    }

    void shell::command_evaluate(const data::mdb_command::arguments_type& arg,
                                 iface::displayer& displayer_)
    {
      bool has_full = false;
      bool has_profile = false;
      bool caching = true;

      auto i = arg.begin();
      const auto e = arg.end();

      for (; i != e; ++i)
      {
        if (!_preprocessor && *i == "-full")
        {
          has_full = true;
        }
        else if (*i == "-profile")
        {
          has_profile = true;
        }
        else if (*i == "-nocache")
        {
          caching = false;
        }
        else
        {
          break;
        }
      }

      if (has_full && has_profile)
      {
        displayer_.show_error(
            "-full and -profile flags cannot be used together.");
        return;
      }

      if (has_profile && !caching)
      {
        displayer_.show_error(
            "-profile and -noncache flags cannot be used together.");
        return;
      }

      std::optional<data::cpp_code> expression = data::cpp_code(join(i, e));
      if (empty(*expression))
      {
        if (!mp)
        {
          displayer_.show_error("Nothing has been evaluated yet.");
          return;
        }
        expression = last_evaluated_expression;
      }
      else if (*expression == "-")
      {
        expression = std::nullopt;
      }

      next_breakpoint_id = 1;
      breakpoints.clear();

      data::metaprogram_mode mode = [&] {
        if (has_full)
        {
          return data::metaprogram_mode::full;
        }
        if (has_profile)
        {
          return data::metaprogram_mode::profile;
        }
        return data::metaprogram_mode::normal;
      }();

      last_evaluated_expression = expression;
      if (run_metaprogram_with_templight(expression, mode, caching, displayer_))
      {
        displayer_.show_raw_text("Metaprogram started");
        assert(mp);
      }
    }

    void
    shell::command_forwardtrace(const data::mdb_command::arguments_type& arg,
                                iface::displayer& displayer_)
    {
      if (!require_running_metaprogram(displayer_))
      {
        return;
      }

      std::optional<int> max_depth;

      auto begin = arg.begin();
      const auto end = arg.end();
      if (begin != end)
      {
        max_depth = int(*begin);
        if (*max_depth < 0)
        {
          throw data::exception("Non-negative max depth expected.");
        }
        ++begin;
      }
      if (begin != end)
      {
        throw data::exception("Unexpected argument: " + join(begin, end));
      }

      display_current_forwardtrace(max_depth, displayer_);
    }

    void shell::command_backtrace(const data::mdb_command::arguments_type& arg,
                                  iface::displayer& displayer_)
    {
      if (require_empty_args(arg, displayer_) &&
          require_running_or_errored_metaprogram(displayer_))
      {
        display_backtrace(displayer_);
      }
    }

    void shell::command_frame(const data::mdb_command::arguments_type& arg,
                              iface::displayer& displayer_)
    {
      if (!require_running_or_errored_metaprogram(displayer_))
      {
        return;
      }

      const int frame_index = int(arg);

      auto backtrace = mp->get_backtrace();

      if (frame_index < 0 || frame_index >= static_cast<int>(backtrace.size()))
      {
        displayer_.show_error("Frame index out of range");
        return;
      }

      display_frame(backtrace[frame_index], displayer_);
    }

    void shell::command_rbreak(const data::mdb_command::arguments_type& arg,
                               iface::displayer& displayer_)
    {
      if (empty(arg))
      {
        displayer_.show_error("Argument expected");
        return;
      }
      if (!require_evaluated_metaprogram(displayer_))
      {
        return;
      }
      try
      {
        breakpoint bp{next_breakpoint_id, data::regex(arg)};
        ++next_breakpoint_id;

        if (mp->caching_enabled())
        {
          const auto match_count =
              std::count_if(mp->begin(false), mp->end(),
                            [&bp](const data::debugger_event& e) {
                              const auto p = mpark::get_if<data::frame>(&e);
                              return p && bp.match(p->node());
                            });

          if (match_count == 0)
          {
            displayer_.show_raw_text("Breakpoint \"" + arg.value() +
                                     "\" will never stop the execution");
          }
          else
          {
            displayer_.show_raw_text(
                "Breakpoint \"" + arg.value() +
                "\" will stop the execution on " + std::to_string(match_count) +
                (match_count > 1 ? " locations" : " location"));
            breakpoints.push_back(bp);
          }
        }
        else
        {
          displayer_.show_raw_text("Breakpoint \"" + arg.value() +
                                   "\" created");
          breakpoints.push_back(bp);
        }
      }
      catch (const std::regex_error&)
      {
        displayer_.show_error("\"" + arg.value() + "\" is not a valid regex");
      }
    }

    void shell::command_break(const data::mdb_command::arguments_type& arg,
                              iface::displayer& displayer_)
    {
      // TODO there will other more kinds of arguments here but needs a proper
      // but it needs a proper command parser
      if (arg.value() != "list")
      {
        displayer_.show_error("Call break like this: \"break list\"");
        return;
      }

      if (breakpoints.empty())
      {
        displayer_.show_raw_text("No breakpoints currently set");
        return;
      }

      for (const breakpoint& bp : breakpoints)
      {
        displayer_.show_raw_text(bp.to_string());
      }
    }

    void shell::command_help(const data::mdb_command::arguments_type& arg,
                             iface::displayer& displayer_)
    {
      if (empty(arg))
      {
        displayer_.show_raw_text("List of available commands:");
        displayer_.show_raw_text("");
        for (const command& cmd : command_handler.get_commands())
        {
          displayer_.show_raw_text(cmd.get_keys().front().value() + " -- " +
                                   cmd.get_short_description());
        }
        displayer_.show_raw_text("");
        displayer_.show_raw_text(
            "Type \"help\" followed by a command name for more information.");
        displayer_.show_raw_text(
            "Command name abbreviations are allowed if unambiguous.");
        displayer_.show_raw_text(
            "A blank line as an input will repeat the last command,"
            " if it makes sense.");
        return;
      }

      if (const auto arg_cmd = std::optional<data::mdb_command>(arg))
      {
        if (auto cmd = command_handler.get_command(arg_cmd->name()))
        {
          if (!empty(arg_cmd->arguments()))
          {
            displayer_.show_error("Only one argument expected\n");
            return;
          }

          displayer_.show_raw_text(
              boost::algorithm::join(
                  cmd->get_keys() |
                      boost::adaptors::transformed(
                          [](const data::mdb_command::name_type& n_) {
                            return n_.value();
                          }),
                  "|") +
              " " + cmd->get_usage());
          displayer_.show_raw_text(cmd->get_full_description());
        }
        else
        {
          displayer_.show_error("Command not found\n");
        }
      }
      else
      {
        displayer_.show_error("Invalid command\n");
      }
    }

    void shell::command_quit(const data::mdb_command::arguments_type& arg,
                             iface::displayer& displayer_)
    {
      if (!require_empty_args(arg, displayer_))
      {
        return;
      }
      is_stopped = true;
    }

    bool shell::run_metaprogram_with_templight(
        const std::optional<data::cpp_code>& expression,
        data::metaprogram_mode mode,
        bool caching_enabled,
        iface::displayer& displayer_)
    {
      try
      {
        mp = metaprogram(
            _preprocessor ?
                _engine.preprocessor_tracer().eval(env, expression, mode) :
                _engine.metaprogram_tracer().eval(
                    env, _mdb_temp_dir, expression, mode, displayer_),
            caching_enabled);
        if (mp && mp->is_empty() && mp->get_evaluation_result().is_error())
        {
          // Most errors will cause templight to generate an empty trace
          // We're only interested in non-empty traces
          // Throwing here to ensure this error is handled the same way as other
          // ones thrown by eval()
          throw data::exception(mp->get_evaluation_result().get_error());
        }
      }
      catch (const data::some_feature_not_supported&)
      {
        throw;
      }
      catch (const std::exception& error)
      {
        displayer_.show_error(error.what());
        mp = std::nullopt;
        return false;
      }
      return true;
    }

    const breakpoint* shell::continue_metaprogram(data::direction_t direction)
    {
      assert(!mp->is_at_endpoint(direction));

      while (true)
      {
        mp->step(direction);
        if (mp->is_at_endpoint(direction))
        {
          return nullptr;
        }
        for (const breakpoint& bp : breakpoints)
        {
          if (bp.match(mp->get_current_frame().node()))
          {
            return &bp;
          }
        }
      }
    }

    unsigned shell::finish_metaprogram()
    {
      unsigned steps = 0;
      for (; !mp->is_finished(); ++steps)
      {
        mp->step();
      }
      return steps;
    }

    void shell::next_metaprogram(data::direction_t direction, int n)
    {
      assert(n >= 0);
      for (int i = 0; i < n && !mp->is_at_endpoint(direction); ++i)
      {
        const auto bt_depth = mp->get_backtrace().size();
        do
        {
          mp->step(direction);
        } while (!mp->is_at_endpoint(direction) &&
                 mp->get_backtrace().size() > bt_depth);
      }
    }

    void shell::display_frame(const data::frame& frame,
                              iface::displayer& displayer_) const
    {
      displayer_.show_frame(frame);

      data::file_location source_location = frame.source_location();
      if (source_location.name == _env_path)
      {
        // We don't want to show stuff from the internal header
        source_location = data::file_location();
      }
      // TODO: we should somehow compensate the file_locations returned by
      // clang for the <stdin> file. This is hard because the file clang sees
      // is just two lines (an include for the PCH and the current line)
      // Until this is figured out, printing file sections for <stdin> is
      // turned off
      // displayer_.show_file_section(source_location, env.get());
      displayer_.show_file_section(source_location, "");
    }

    void shell::display_current_frame(iface::displayer& displayer_) const
    {
      assert(mp);
      assert(!mp->is_at_start());
      assert(!mp->is_finished());

      display_frame(mp->get_current_frame(), displayer_);
    }

    void shell::display_current_forwardtrace(std::optional<int> max_depth,
                                             iface::displayer& displayer_)
    {
      if (mp->caching_enabled())
      {
        displayer_.show_call_graph(boost::make_iterator_range(
            forward_trace_iterator(
                mp->current_position(), mp->end(), max_depth),
            forward_trace_iterator()));
      }
      else
      {
        throw caching_disabled("displaying forward trace");
      }
    }

    void shell::display_backtrace(iface::displayer& displayer_)
    {
      if (!mp)
      {
        displayer_.show_error("Metaprogram not evaluated yet");
      }
      else if (mp->is_finished() && !mp->get_evaluation_result().is_error())
      {
        display_metaprogram_finished(displayer_);
      }
      else
      {
        displayer_.show_backtrace(mp->get_backtrace());
      }
    }

    void shell::display_metaprogram_reached_the_beginning(
        iface::displayer& displayer_) const
    {
      displayer_.show_raw_text("Metaprogram reached the beginning");
    }

    void shell::display_metaprogram_finished(iface::displayer& displayer_)
    {
      displayer_.show_raw_text("Metaprogram finished");
      displayer_.show_type_or_code_or_error(mp->get_evaluation_result());
    }

    void shell::display_movement_info(bool moved, iface::displayer& displayer_)
    {
      if (mp->is_finished())
      {
        if (moved)
        {
          display_metaprogram_finished(displayer_);
        }
      }
      else if (mp->is_at_start())
      {
        if (moved)
        {
          display_metaprogram_reached_the_beginning(displayer_);
        }
      }
      else
      {
        display_current_frame(displayer_);
      }
    }

    void shell::cancel_operation()
    {
      // TODO
    }

    data::code_completion shell::code_complete(const data::user_input&, bool)
    {
      // TODO
      return data::code_completion{};
    }

    void shell::line_available(const data::user_input& line,
                               iface::displayer& displayer_)
    {
      core::null_history h;
      line_available(line, displayer_, h);
    }
  }
}
