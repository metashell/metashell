
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

#include <metashell/forward_trace_iterator.hpp>
#include <metashell/highlight_syntax.hpp>
#include <metashell/mdb_shell.hpp>
#include <metashell/metashell.hpp>
#include <metashell/null_history.hpp>
#include <metashell/some_feature_not_supported.hpp>

#include <cmath>
#include <sstream>
#include <stdexcept>

#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi.hpp>

#include <boost/range/iterator_range.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace
{
  std::function<void(
      metashell::mdb_shell&, const std::string&, metashell::iface::displayer&)>
  callback(void (metashell::mdb_shell::*f)(const std::string&,
                                           metashell::iface::displayer&))
  {
    using namespace std::placeholders;
    return std::bind(f, _1, _2, _3);
  }
}

namespace metashell
{
  mdb_shell::mdb_shell(iface::environment& env_arg,
                       iface::engine& engine_,
                       const boost::filesystem::path& env_path_,
                       const boost::filesystem::path& mdb_temp_dir_,
                       bool preprocessor_,
                       logger* logger_)
    : command_handler(build_command_handler(preprocessor_)),
      env(env_arg),
      _logger(logger_),
      _engine(engine_),
      _env_path(env_path_),
      _mdb_temp_dir(mdb_temp_dir_),
      _preprocessor(preprocessor_)
  {
  }

  mdb_command_handler_map mdb_shell::build_command_handler(bool preprocessor_)
  {
    const std::string expr = preprocessor_ ? "<expression>" : "<type>";
    // clang-format off
    return
      mdb_command_handler_map({
        {{"evaluate"}, repeatable_t::non_repeatable,
          callback(&mdb_shell::command_evaluate),
          std::string(preprocessor_ ? "[-profile]" : "[-full|-profile]") +
            " [" + expr + "|-]",
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
          "Previous breakpoints are cleared." +
          std::string(preprocessor_ ? "" :
            "\n\nUnlike metashell, evaluate doesn't use metashell::format to avoid cluttering\n"
            "the debugged metaprogram with unrelated code. If you need formatting, you can\n"
            "explicitly enter `metashell::format< <type> >::type` for the same effect."
          )},
        {{"step"}, repeatable_t::repeatable,
          callback(&mdb_shell::command_step),
          "[over|out] [n]",
          "Step the program.",
          "Argument n means step n times. n defaults to 1 if not specified.\n"
          "Negative n means step the program backwards.\n\n"
          "`step over` is an alias for next.\n"
          "Use of the `out` qualifier will jump out of the current instantiation frame.\n"
          "Similarly to `next`, `step out -1` is not always the inverse of `step out`."},
        {{"next"}, repeatable_t::repeatable,
          callback(&mdb_shell::command_next),
          "[n]",
          "Jump over to the next instantiation skipping sub instantiations.",
          "Argument n means jump n times. n defaults to 1 if not specified.\n"
          "Negative n means step the program backwards.\n\n"
          "Please note that `next -1` is not always the inverse of `next`.\n"
          "In particular when there are no more instantiations that got instantiated\n"
          "by the current parent, then `next` will behave like a normal `step`,\n"
          "and will step out of one or more instantiation frames.\n\n"
          "`step over` is an alias for next."},
        {{"rbreak"}, repeatable_t::non_repeatable,
          callback(&mdb_shell::command_rbreak),
          "<regex>",
          "Add breakpoint for all types matching `<regex>`.",
          ""},
        {{"break"}, repeatable_t::non_repeatable,
          callback(&mdb_shell::command_break),
          "list",
          "List breakpoints.",
          ""},
        {{"continue"}, repeatable_t::repeatable,
          callback(&mdb_shell::command_continue),
          "[n]",
          "Continue program being debugged.",
          "The program is continued until the nth breakpoint or the end of the program\n"
          "is reached. n defaults to 1 if not specified.\n"
          "Negative n means continue the program backwards."},
        {{"finish"}, repeatable_t::repeatable,
          callback(&mdb_shell::command_finish),
          "",
          "Finish program being debugged.",
          "The program is continued until the end ignoring any breakpoints."},
        {{"forwardtrace", "ft"}, repeatable_t::non_repeatable,
          callback(&mdb_shell::command_forwardtrace),
          "[n]",
          "Print forwardtrace from the current point.",
          "The n specifier limits the depth of the trace. If n is not specified, then the\n"
          "trace depth is unlimited."},
        {{"backtrace", "bt"}, repeatable_t::non_repeatable,
          callback(&mdb_shell::command_backtrace),
          "",
          "Print backtrace from the current point.",
          ""},
        {{"frame", "f"}, repeatable_t::non_repeatable,
          callback(&mdb_shell::command_frame),
          "n",
          "Inspect the nth frame of the current backtrace.",
          ""},
        {{"help"}, repeatable_t::non_repeatable,
          callback(&mdb_shell::command_help),
          "[<command>]",
          "Show help for commands.",
          "If <command> is not specified, show a list of all available commands."},
        {{"quit"} , repeatable_t::non_repeatable,
          callback(&mdb_shell::command_quit),
          "",
          "Quit metadebugger.",
          ""}
      });
    // clang-format on
  }

  std::string mdb_shell::prompt() const
  {
    return _preprocessor ? "(pdb)" : "(mdb)";
  }

  bool mdb_shell::stopped() const { return is_stopped; }

  void mdb_shell::display_splash(iface::displayer& displayer_) const
  {
    displayer_.show_raw_text("For help, type \"help\".");
  }

  void mdb_shell::line_available(const std::string& line_arg,
                                 iface::displayer& displayer_,
                                 iface::history& history_)
  {

    try
    {
      using boost::algorithm::all;
      using boost::is_space;

      std::string line = line_arg;

      if (line != prev_line && !line.empty())
      {
        history_.add(line);
      }

      if (line.empty())
      {
        if (!last_command_repeatable)
        {
          return;
        }
        line = prev_line;
      }
      else
      {
        prev_line = line;
      }

      if (all(line, is_space()))
      {
        return;
      }

      auto command_arg_pair = command_handler.get_command_for_line(line);
      if (!command_arg_pair)
      {
        displayer_.show_error("Command parsing failed\n");
        last_command_repeatable = false;
        return;
      }

      mdb_command cmd;
      std::string args;
      std::tie(cmd, args) = *command_arg_pair;

      last_command_repeatable = cmd.is_repeatable();

      cmd.get_func()(*this, args, displayer_);
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

  bool mdb_shell::require_empty_args(const std::string& args,
                                     iface::displayer& displayer_) const
  {
    if (!args.empty())
    {
      displayer_.show_error("This command doesn't accept arguments");
      return false;
    }
    return true;
  }

  bool
  mdb_shell::require_evaluated_metaprogram(iface::displayer& displayer_) const
  {
    if (!mp)
    {
      displayer_.show_error("Metaprogram not evaluated yet");
      return false;
    }
    return true;
  }

  bool mdb_shell::require_running_metaprogram(iface::displayer& displayer_)
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

  bool mdb_shell::require_running_or_errored_metaprogram(
      iface::displayer& displayer_)
  {
    if (!require_evaluated_metaprogram(displayer_))
    {
      return false;
    }

    if (mp->get_evaluation_result().is_error())
    {
      return true;
    }

    if (mp->is_finished())
    {
      display_metaprogram_finished(displayer_);
      return false;
    }
    return true;
  }

  void mdb_shell::command_continue(const std::string& arg,
                                   iface::displayer& displayer_)
  {
    if (!require_evaluated_metaprogram(displayer_))
    {
      return;
    }

    const auto continue_count = parse_defaultable_integer(arg, 1);
    if (!continue_count)
    {
      display_argument_parsing_failed(displayer_);
      return;
    }

    data::direction_t direction = *continue_count >= 0 ?
                                      data::direction_t::forward :
                                      data::direction_t::backwards;

    const breakpoint* breakpoint_ptr = nullptr;
    for (int i = 0;
         i < std::abs(*continue_count) && !mp->is_at_endpoint(direction); ++i)
    {
      breakpoint_ptr = continue_metaprogram(direction);
    }

    if (breakpoint_ptr)
    {
      displayer_.show_raw_text(breakpoint_ptr->to_string() + " reached");
    }
    display_movement_info(*continue_count != 0, displayer_);
  }

  void mdb_shell::command_finish(const std::string& arg,
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

  void mdb_shell::command_step(const std::string& arg,
                               iface::displayer& displayer_)
  {
    if (!require_evaluated_metaprogram(displayer_))
    {
      return;
    }

    using boost::spirit::qi::lit;
    using boost::spirit::qi::int_;
    using boost::spirit::ascii::space;
    using boost::phoenix::ref;
    using boost::spirit::qi::_1;

    auto begin = arg.begin(), end = arg.end();

    int step_count = 1;

    enum class step_t
    {
      normal,
      over,
      out
    };

    step_t step_type = step_t::normal;

    bool result = boost::spirit::qi::phrase_parse(
        begin, end,

        -(lit("over")[ref(step_type) = step_t::over] |
          lit("out")[ref(step_type) = step_t::out]) >>
            -int_[ref(step_count) = _1],

        space);

    if (!result || begin != end)
    {
      display_argument_parsing_failed(displayer_);
      return;
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

  void mdb_shell::command_next(const std::string& arg,
                               iface::displayer& displayer_)
  {
    if (!require_evaluated_metaprogram(displayer_))
    {
      return;
    }

    const auto next_count = parse_defaultable_integer(arg, 1);
    if (!next_count)
    {
      display_argument_parsing_failed(displayer_);
      return;
    }

    next_metaprogram(next_count >= 0 ? data::direction_t::forward :
                                       data::direction_t::backwards,
                     std::abs(*next_count));

    display_movement_info(next_count != 0, displayer_);
  }

  void mdb_shell::command_evaluate(const std::string& arg_copy,
                                   iface::displayer& displayer_)
  {
    // Easier not to use spirit here (or probably not...)
    // TODO OK. after -profile, this parsing really needs some refactoring
    std::string arg = arg_copy;

    const std::string full_flag = "-full";
    const std::string profile_flag = "-profile";

    bool has_full = false;
    bool has_profile = false;

    // Intentionally left really ugly for more motivation to refactor
    while (true)
    {
      if (!_preprocessor && boost::starts_with(arg, full_flag) &&
          (arg.size() == full_flag.size() ||
           std::isspace(arg[full_flag.size()])))
      {
        has_full = true;
        arg = boost::trim_left_copy(arg.substr(full_flag.size()));
      }
      else if (boost::starts_with(arg, profile_flag) &&
               (arg.size() == profile_flag.size() ||
                std::isspace(arg[profile_flag.size()])))
      {
        has_profile = true;
        arg = boost::trim_left_copy(arg.substr(profile_flag.size()));
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

    boost::optional<data::cpp_code> expression = data::cpp_code(arg);
    if (expression->empty())
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
      expression = boost::none;
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
    if (run_metaprogram_with_templight(expression, mode, displayer_))
    {
      displayer_.show_raw_text("Metaprogram started");
      assert(mp);
    }
  }

  void mdb_shell::command_forwardtrace(const std::string& arg,
                                       iface::displayer& displayer_)
  {
    if (!require_running_metaprogram(displayer_))
    {
      return;
    }

    using boost::spirit::qi::lit;
    using boost::spirit::qi::uint_;
    using boost::spirit::ascii::space;
    using boost::spirit::qi::_1;

    namespace phx = boost::phoenix;

    auto begin = arg.begin(), end = arg.end();

    boost::optional<int> max_depth;

    bool result =
        boost::spirit::qi::phrase_parse(begin, end,

                                        -uint_[phx::ref(max_depth) = _1],

                                        space);

    if (!result || begin != end)
    {
      display_argument_parsing_failed(displayer_);
      return;
    }
    display_current_forwardtrace(max_depth, displayer_);
  }

  void mdb_shell::command_backtrace(const std::string& arg,
                                    iface::displayer& displayer_)
  {
    if (require_empty_args(arg, displayer_) &&
        require_running_or_errored_metaprogram(displayer_))
    {
      display_backtrace(displayer_);
    }
  }

  void mdb_shell::command_frame(const std::string& arg,
                                iface::displayer& displayer_)
  {
    if (!require_running_or_errored_metaprogram(displayer_))
    {
      return;
    }

    const auto frame_index = parse_mandatory_integer(arg);
    if (!frame_index)
    {
      display_argument_parsing_failed(displayer_);
      return;
    }

    auto backtrace = mp->get_backtrace();

    if (frame_index < 0 || *frame_index >= static_cast<int>(backtrace.size()))
    {
      displayer_.show_error("Frame index out of range");
      return;
    }

    display_frame(backtrace[*frame_index], displayer_);
  }

  void mdb_shell::command_rbreak(const std::string& arg,
                                 iface::displayer& displayer_)
  {
    if (arg.empty())
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
      breakpoint bp{next_breakpoint_id, boost::regex(arg)};
      ++next_breakpoint_id;

      const auto match_count = std::count_if(
          mp->begin(false), mp->end(), [&bp](const data::debugger_event& e) {
            const auto p = mpark::get_if<data::frame>(&e);
            return p && bp.match(p->node());
          });

      if (match_count == 0)
      {
        displayer_.show_raw_text("Breakpoint \"" + arg +
                                 "\" will never stop the execution");
      }
      else
      {
        displayer_.show_raw_text(
            "Breakpoint \"" + arg + "\" will stop the execution on " +
            std::to_string(match_count) +
            (match_count > 1 ? " locations" : " location"));
        breakpoints.push_back(bp);
      }
    }
    catch (const boost::regex_error&)
    {
      displayer_.show_error("\"" + arg + "\" is not a valid regex");
    }
  }

  void mdb_shell::command_break(const std::string& arg,
                                iface::displayer& displayer_)
  {
    // TODO there will other more kinds of arguments here but needs a proper but
    // it needs a proper command parser
    if (arg != "list")
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

  void mdb_shell::command_help(const std::string& arg,
                               iface::displayer& displayer_)
  {
    if (arg.empty())
    {
      displayer_.show_raw_text("List of available commands:");
      displayer_.show_raw_text("");
      for (const mdb_command& cmd : command_handler.get_commands())
      {
        displayer_.show_raw_text(cmd.get_keys().front() + " -- " +
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

    auto command_arg_pair = command_handler.get_command_for_line(arg);
    if (!command_arg_pair)
    {
      displayer_.show_error("Command not found\n");
      return;
    }

    using boost::algorithm::join;

    mdb_command cmd;
    std::string command_args;
    std::tie(cmd, command_args) = *command_arg_pair;

    if (!command_args.empty())
    {
      displayer_.show_error("Only one argument expected\n");
      return;
    }

    displayer_.show_raw_text(join(cmd.get_keys(), "|") + " " + cmd.get_usage());
    displayer_.show_raw_text(cmd.get_full_description());
  }

  void mdb_shell::command_quit(const std::string& arg,
                               iface::displayer& displayer_)
  {
    if (!require_empty_args(arg, displayer_))
    {
      return;
    }
    is_stopped = true;
  }

  bool mdb_shell::run_metaprogram_with_templight(
      const boost::optional<data::cpp_code>& expression,
      data::metaprogram_mode mode,
      iface::displayer& displayer_)
  {
    try
    {
      mp = metaprogram(
          _preprocessor ?
              _engine.preprocessor_tracer().eval(env, expression, mode) :
              _engine.metaprogram_tracer().eval(
                  env, _mdb_temp_dir, expression, mode, displayer_));
      if (mp && mp->is_empty() && mp->get_evaluation_result().is_error())
      {
        // Most errors will cause templight to generate an empty trace
        // We're only interested in non-empty traces
        // Throwing here to ensure this error is handled the same way as other
        // ones thrown by eval()
        throw exception(mp->get_evaluation_result().get_error());
      }
    }
    catch (const some_feature_not_supported&)
    {
      throw;
    }
    catch (const std::exception& error)
    {
      displayer_.show_error(error.what());
      mp = boost::none;
      return false;
    }
    return true;
  }

  boost::optional<int>
  mdb_shell::parse_defaultable_integer(const std::string& arg,
                                       int default_value)
  {
    using boost::spirit::qi::int_;
    using boost::spirit::ascii::space;
    using boost::phoenix::ref;
    using boost::spirit::qi::_1;

    auto begin = arg.begin(), end = arg.end();

    int value = default_value;

    bool result = boost::spirit::qi::phrase_parse(begin, end,

                                                  -int_[ref(value) = _1],

                                                  space);

    if (!result || begin != end)
    {
      return boost::none;
    }
    return value;
  }

  boost::optional<int>
  mdb_shell::parse_mandatory_integer(const std::string& arg)
  {
    using boost::spirit::qi::int_;
    using boost::spirit::ascii::space;
    using boost::phoenix::ref;
    using boost::spirit::qi::_1;

    auto begin = arg.begin(), end = arg.end();

    int value = 0;

    bool result = boost::spirit::qi::phrase_parse(begin, end,

                                                  int_[ref(value) = _1],

                                                  space);

    if (!result || begin != end)
    {
      return boost::none;
    }
    return value;
  }

  const breakpoint* mdb_shell::continue_metaprogram(data::direction_t direction)
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

  unsigned mdb_shell::finish_metaprogram()
  {
    unsigned steps = 0;
    for (; !mp->is_finished(); ++steps)
    {
      mp->step();
    }
    return steps;
  }

  void mdb_shell::next_metaprogram(data::direction_t direction, int n)
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

  void mdb_shell::display_frame(const data::frame& frame,
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

  void mdb_shell::display_current_frame(iface::displayer& displayer_) const
  {
    assert(mp);
    assert(!mp->is_at_start());
    assert(!mp->is_finished());

    display_frame(mp->get_current_frame(), displayer_);
  }

  void mdb_shell::display_current_forwardtrace(boost::optional<int> max_depth,
                                               iface::displayer& displayer_)
  {
    displayer_.show_call_graph(boost::make_iterator_range(
        forward_trace_iterator(mp->current_position(), mp->end(), max_depth),
        forward_trace_iterator()));
  }

  void mdb_shell::display_backtrace(iface::displayer& displayer_)
  {
    displayer_.show_backtrace(mp->get_backtrace());
  }

  void
  mdb_shell::display_argument_parsing_failed(iface::displayer& displayer_) const
  {
    displayer_.show_error("Argument parsing failed");
  }

  void mdb_shell::display_metaprogram_reached_the_beginning(
      iface::displayer& displayer_) const
  {
    displayer_.show_raw_text("Metaprogram reached the beginning");
  }

  void mdb_shell::display_metaprogram_finished(iface::displayer& displayer_)
  {
    displayer_.show_raw_text("Metaprogram finished");
    displayer_.show_type_or_code_or_error(mp->get_evaluation_result());
  }

  void mdb_shell::display_movement_info(bool moved,
                                        iface::displayer& displayer_)
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

  void mdb_shell::cancel_operation()
  {
    // TODO
  }

  void mdb_shell::code_complete(const std::string&, std::set<std::string>&)
  {
    // TODO
  }

  void mdb_shell::line_available(const std::string& line,
                                 iface::displayer& displayer_)
  {
    null_history h;
    line_available(line, displayer_, h);
  }
}
