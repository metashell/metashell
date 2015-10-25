
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

#include <metashell/mdb_shell.hpp>
#include <metashell/highlight_syntax.hpp>
#include <metashell/metashell.hpp>
#include <metashell/temporary_file.hpp>
#include <metashell/is_template_type.hpp>
#include <metashell/forward_trace_iterator.hpp>
#include <metashell/null_history.hpp>

#include <cmath>
#include <sstream>
#include <fstream>

#include <boost/assign.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_core.hpp>

#include <boost/range/iterator_range.hpp>

namespace
{
  //Note: this how clang calls the file when the input comes from stdin
  const std::string stdin_name = "<stdin>";

  const std::string wrap_prefix = "metashell::impl::wrap<";
  const std::string wrap_suffix = ">";

  typedef
    std::tuple<
      metashell::data::file_location,
      metashell::data::instantiation_kind,
      metashell::metaprogram::vertex_descriptor
    >
    set_element_t;

  bool less_than(const set_element_t& lhs, const set_element_t& rhs)
  {
    return lhs < rhs;
  }

  bool less_than_ignore_instantiation_kind(
    const set_element_t& lhs,
    const set_element_t& rhs
  )
  {
    using std::get;

    return
      std::tie(get<0>(lhs), get<2>(lhs)) <
      std::tie(get<0>(rhs), get<2>(rhs));
  }

  std::function<
    void(
      metashell::mdb_shell&,
      const std::string&,
      metashell::iface::displayer&
    )
  >
  callback(
    void (metashell::mdb_shell::*f)(
      const std::string&,
      metashell::iface::displayer&
    )
  )
  {
    using namespace std::placeholders;
    return std::bind(f, _1, _2, _3);
  }
}

namespace metashell {

const mdb_command_handler_map mdb_shell::command_handler =
  mdb_command_handler_map(
    {
      {{"evaluate"}, repeatable_t::non_repeatable,
        callback(&mdb_shell::command_evaluate),
        "[-full|-profile] [<type>|-]",
        "Evaluate and start debugging a new metaprogram.",
        "Evaluating a metaprogram using the `-full` qualifier will expand all\n"
        "Memoization events.\n\n"
        "Evaluating a metaprogram using the `-profile` qualifier will enable\n"
        "profile mode.\n\n"
        "Instead of `<type>`, evaluate can be called with `-`, in which case the\n"
        "whole environment is being debugged not just a single type expression.\n\n"
        "If called without `<type>` or `-`, then the last evaluated metaprogram will\n"
        "be reevaluated.\n\n"
        "Previous breakpoints are cleared.\n\n"
        "Unlike metashell, evaluate doesn't use metashell::format to avoid cluttering\n"
        "the debugged metaprogram with unrelated code. If you need formatting, you can\n"
        "explicitly enter `metashell::format< <type> >::type` for the same effect."},
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

namespace {

config set_pch_false(config c) {
  c.use_precompiled_headers = false;
  return c;
}

}

mdb_shell::mdb_shell(
    const config& conf_,
    const iface::environment& env_arg,
    logger* logger_) :
  conf(set_pch_false(conf_)),
  env(conf),
  _logger(logger_)
{
  env.append(env_arg.get_all());
}

std::string mdb_shell::prompt() const {
  return "(mdb)";
}

bool mdb_shell::stopped() const {
  return is_stopped;
}

void mdb_shell::display_splash(iface::displayer& displayer_) const {
  displayer_.show_raw_text("For help, type \"help\".");
}

void mdb_shell::line_available(
    const std::string& line_arg,
    iface::displayer& displayer_,
    iface::history& history_)
{

  try {
    using boost::algorithm::all;
    using boost::is_space;

    std::string line = line_arg;

    if (line != prev_line && !line.empty()) {
      history_.add(line);
    }

    if (line.empty()) {
      if (!last_command_repeatable) {
        return;
      }
      line = prev_line;
    } else {
      prev_line = line;
    }

    if (all(line, is_space())) {
      return;
    }

    auto command_arg_pair = command_handler.get_command_for_line(line);
    if (!command_arg_pair) {
      displayer_.show_error("Command parsing failed\n");
      last_command_repeatable = false;
      return;
    }

    mdb_command cmd;
    std::string args;
    std::tie(cmd, args) = *command_arg_pair;

    last_command_repeatable = cmd.is_repeatable();

    cmd.get_func()(*this, args, displayer_);
  } catch (const std::exception& ex) {
    displayer_.show_error(std::string("Error: ") + ex.what() + "\n");
  } catch (...) {
    displayer_.show_error("Unknown error\n");
  }
}
bool mdb_shell::breakpoint_match(
    metaprogram::vertex_descriptor vertex, const breakpoint_t& breakpoint)
{
  return boost::regex_search(
      mp->get_vertex_property(vertex).name, std::get<1>(breakpoint));
}

bool mdb_shell::require_empty_args(
    const std::string& args,
    iface::displayer& displayer_) const
{
  if (!args.empty()) {
    displayer_.show_error("This command doesn't accept arguments");
    return false;
  }
  return true;
}

bool mdb_shell::require_evaluated_metaprogram(
    iface::displayer& displayer_) const
{
  if (!mp) {
    displayer_.show_error("Metaprogram not evaluated yet");
    return false;
  }
  return true;
}

bool mdb_shell::require_running_metaprogram(iface::displayer& displayer_) const
{
  if (!require_evaluated_metaprogram(displayer_)) {
    return false;
  }

  if (mp->is_finished()) {
    display_metaprogram_finished(displayer_);
    return false;
  }
  return true;
}

bool mdb_shell::require_running_or_errored_metaprogram(
    iface::displayer& displayer_) const
{
  if (!require_evaluated_metaprogram(displayer_)) {
    return false;
  }

  if (mp->get_evaluation_result().is_error()) {
    return true;
  }

  if (mp->is_finished()) {
    display_metaprogram_finished(displayer_);
    return false;
  }
  return true;
}

void mdb_shell::command_continue(
    const std::string& arg,
    iface::displayer& displayer_)
{
  if (!require_evaluated_metaprogram(displayer_)) {
    return;
  }

  const auto continue_count = parse_defaultable_integer(arg, 1);
  if (!continue_count) {
    display_argument_parsing_failed(displayer_);
    return;
  }

  direction_t direction =
    *continue_count >= 0 ? direction_t::forward : direction_t::backwards;

  breakpoints_t::iterator breakpoint_it = breakpoints.end();
  for (int i = 0;
      i < std::abs(*continue_count) && !mp->is_at_endpoint(direction); ++i)
  {
    breakpoint_it = continue_metaprogram(direction);
  }

  if (breakpoint_it != breakpoints.end()) {
    displayer_.show_raw_text(
        "Breakpoint \"" + std::get<0>(*breakpoint_it) + "\" reached");
  }
  display_movement_info(*continue_count != 0, displayer_);
}

void mdb_shell::command_finish(
    const std::string& arg,
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

void mdb_shell::command_step(
    const std::string& arg,
    iface::displayer& displayer_)
{
  if (!require_evaluated_metaprogram(displayer_)) {
    return;
  }

  using boost::spirit::qi::lit;
  using boost::spirit::qi::int_;
  using boost::spirit::ascii::space;
  using boost::phoenix::ref;
  using boost::spirit::qi::_1;

  auto begin = arg.begin(),
       end = arg.end();

  int step_count = 1;

  enum class step_t { normal, over, out };

  step_t step_type = step_t::normal;

  bool result =
    boost::spirit::qi::phrase_parse(
        begin, end,

        -(
          lit("over")[ref(step_type) = step_t::over] |
          lit("out")[ref(step_type) = step_t::out]
        ) >>
        -int_[ref(step_count) = _1],

        space
    );

  if (!result || begin != end) {
    display_argument_parsing_failed(displayer_);
    return;
  }

  direction_t direction =
    step_count >= 0 ? direction_t::forward : direction_t::backwards;

  int iteration_count = std::abs(step_count);

  switch (step_type) {
    case step_t::normal:
      for (int i = 0;
          i < iteration_count && !mp->is_at_endpoint(direction); ++i)
      {
        mp->step(direction);
      }
      break;
    case step_t::over:
      next_metaprogram(direction, iteration_count);
      break;
    case step_t::out:
      {
        for (int i = 0;
            i < iteration_count && !mp->is_at_endpoint(direction); ++i)
        {
          unsigned bt_depth = mp->get_backtrace_length();
          while (!mp->is_at_endpoint(direction) &&
              mp->get_backtrace_length()+1 > bt_depth)
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

void mdb_shell::command_next(
    const std::string& arg,
    iface::displayer& displayer_)
{
  if (!require_evaluated_metaprogram(displayer_)) {
    return;
  }

  const auto next_count = parse_defaultable_integer(arg, 1);
  if (!next_count) {
    display_argument_parsing_failed(displayer_);
    return;
  }

  next_metaprogram(
    next_count >= 0 ? direction_t::forward : direction_t::backwards,
    std::abs(*next_count));

  display_movement_info(next_count != 0, displayer_);
}

bool mdb_shell::is_wrap_type(const std::string& type) {
  // TODO this check could be made more strict,
  // since we know whats inside wrap<...> (mp->get_evaluation_result)
  return boost::starts_with(type, wrap_prefix) &&
         boost::ends_with(type, wrap_suffix);
}

std::string mdb_shell::trim_wrap_type(const std::string& type) {
  assert(is_wrap_type(type));
  return boost::trim_copy(type.substr(
         wrap_prefix.size(),
         type.size() - wrap_prefix.size() - wrap_suffix.size()));
}

void mdb_shell::filter_disable_everything() {
  for (metaprogram::edge_descriptor edge : mp->get_edges()) {
    mp->get_edge_property(edge).enabled = false;
  }
}

void mdb_shell::filter_enable_reachable(bool for_current_line) {
  using vertex_descriptor = metaprogram::vertex_descriptor;
  using edge_descriptor = metaprogram::edge_descriptor;
  using edge_property = metaprogram::edge_property;
  using discovered_t = metaprogram::discovered_t;

  std::string env_buffer = env.get();
  int line_number = std::count(env_buffer.begin(), env_buffer.end(), '\n');

  // We will traverse the interesting edges later
  std::stack<edge_descriptor> edge_stack;

  // Enable the interesting root edges
  for (edge_descriptor edge : mp->get_out_edges(mp->get_root_vertex())) {
    edge_property& property = mp->get_edge_property(edge);
    const std::string& target_name =
      mp->get_vertex_property(mp->get_target(edge)).name;
    // Filter out edges, that is not instantiated
    // by the entered type if requested
    const bool current_line_filter = !for_current_line || (
      property.point_of_instantiation.name == stdin_name &&
      property.point_of_instantiation.row == line_number + 1
    );

    if (!current_line_filter) {
      continue;
    }

    if (!is_instantiation_kind_enabled(property.kind)) {
      continue;
    }

    if (property.kind == data::instantiation_kind::memoization &&
        is_wrap_type(target_name))
    {
      continue;
    }

    property.enabled = true;
    edge_stack.push(edge);
  }

  discovered_t discovered(mp->get_num_vertices());
  // Traverse the graph to enable all edges which are reachable from the
  // edges enabled above
  while (!edge_stack.empty()) {
    edge_descriptor edge = edge_stack.top();
    edge_stack.pop();

    assert(mp->get_edge_property(edge).enabled);

    vertex_descriptor vertex = mp->get_target(edge);

    if (discovered[vertex]) {
      continue;
    }
    discovered[vertex] = true;

    for (edge_descriptor out_edge : mp->get_out_edges(vertex)) {
      edge_property& property = mp->get_edge_property(out_edge);
      if (is_instantiation_kind_enabled(property.kind)) {
        property.enabled = true;
        edge_stack.push(out_edge);
      }
    }
  }
}

void mdb_shell::filter_unwrap_vertices() {
  for (metaprogram::vertex_descriptor vertex : mp->get_vertices()) {
    std::string& name = mp->get_vertex_property(vertex).name;
    if (is_wrap_type(name)) {
      name = trim_wrap_type(name);
      if (!is_template_type(name)) {
        for (metaprogram::edge_descriptor in_edge : mp->get_in_edges(vertex)) {
          mp->get_edge_property(in_edge).kind =
            data::instantiation_kind::non_template_type;
        }
      }
    }
  }
}

void mdb_shell::filter_similar_edges() {

  using vertex_descriptor = metaprogram::vertex_descriptor;
  using edge_descriptor = metaprogram::edge_descriptor;
  using edge_property = metaprogram::edge_property;

  auto comparator = mp->get_mode() == metaprogram::mode_t::full ?
    less_than_ignore_instantiation_kind : less_than;

  // Clang sometimes produces equivalent instantiations events from the same
  // point. Filter out all but one of each
  for (vertex_descriptor vertex : mp->get_vertices()) {

    std::set<set_element_t, decltype(comparator)> similar_edges(comparator);

    for (edge_descriptor edge : mp->get_out_edges(vertex)) {
      edge_property& edge_property = mp->get_edge_property(edge);

      set_element_t set_element = std::make_tuple(
            edge_property.point_of_instantiation,
            edge_property.kind,
            mp->get_target(edge));

      if (similar_edges.count(set_element) > 0) {
        edge_property.enabled = false;
      } else {
        similar_edges.insert(set_element);
      }
    }
  }
}

void mdb_shell::filter_metaprogram(bool for_current_line) {
  assert(mp);

  filter_disable_everything();
  filter_enable_reachable(for_current_line);
  filter_unwrap_vertices();
  filter_similar_edges();

  mp->init_full_time_taken();
}

bool mdb_shell::is_instantiation_kind_enabled(data::instantiation_kind kind) {
  switch (kind) {
    case data::instantiation_kind::memoization:
    case data::instantiation_kind::template_instantiation:
    case data::instantiation_kind::deduced_template_argument_substitution:
    case data::instantiation_kind::explicit_template_argument_substitution:
      return true;
    default:
      return false;
  }
}

void mdb_shell::command_evaluate(
    const std::string& arg_copy,
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
  while (true) {
    if (boost::starts_with(arg, full_flag) &&
       (arg.size() == full_flag.size() ||
       std::isspace(arg[full_flag.size()])))
    {
      has_full = true;
      arg = boost::trim_left_copy(arg.substr(full_flag.size()));
    } else if (boost::starts_with(arg, profile_flag) &&
       (arg.size() == profile_flag.size() ||
       std::isspace(arg[profile_flag.size()])))
    {
      has_profile = true;
      arg = boost::trim_left_copy(arg.substr(profile_flag.size()));
    } else {
      break;
    }
  }

  if (has_full && has_profile) {
    displayer_.show_error("-full and -profile flags cannot be used together.");
    return;
  }

  boost::optional<std::string> expression = arg;
  if (expression->empty()) {
    if (!mp) {
      displayer_.show_error("Nothing has been evaluated yet.");
      return;
    }
    expression = last_evaluated_expression;
  } else if (*expression == "-") {
    expression = boost::none;
  }

  breakpoints.clear();

  metaprogram::mode_t mode = [&] {
    if (has_full) { return metaprogram::mode_t::full; }
    if (has_profile) { return metaprogram::mode_t::profile; }
    return metaprogram::mode_t::normal;
  }();

  last_evaluated_expression = expression;
  if (!run_metaprogram_with_templight(expression, mode, displayer_)) {
    return;
  }

  displayer_.show_raw_text("Metaprogram started");

  filter_metaprogram(bool(expression));
}

void mdb_shell::command_forwardtrace(
    const std::string& arg,
    iface::displayer& displayer_)
{
  if (!require_running_metaprogram(displayer_)) {
    return;
  }

  using boost::spirit::qi::lit;
  using boost::spirit::qi::uint_;
  using boost::spirit::ascii::space;
  using boost::spirit::qi::_1;

  namespace phx = boost::phoenix;

  auto begin = arg.begin(),
       end = arg.end();

  boost::optional<int> max_depth;

  bool result =
    boost::spirit::qi::phrase_parse(
        begin, end,

        -uint_ [phx::ref(max_depth) =_1],

        space
    );

  if (!result || begin != end) {
    display_argument_parsing_failed(displayer_);
    return;
  }

  display_current_forwardtrace(max_depth, displayer_);
}

void mdb_shell::command_backtrace(
    const std::string& arg,
    iface::displayer& displayer_)
{
  if (require_empty_args(arg, displayer_) &&
    require_running_or_errored_metaprogram(displayer_)) {
    display_backtrace(displayer_);
  }
}

void mdb_shell::command_frame(
    const std::string& arg,
    iface::displayer& displayer_)
{
  if (!require_running_or_errored_metaprogram(displayer_)) {
    return;
  }

  const auto frame_index = parse_mandatory_integer(arg);
  if (!frame_index) {
    display_argument_parsing_failed(displayer_);
    return;
  }

  auto backtrace = mp->get_backtrace();

  if (frame_index < 0 || *frame_index >= static_cast<int>(backtrace.size())) {
    displayer_.show_error("Frame index out of range");
    return;
  }

  display_frame(backtrace[*frame_index], displayer_);
}

void mdb_shell::command_rbreak(
    const std::string& arg,
    iface::displayer& displayer_)
{
  if (arg.empty()) {
    displayer_.show_error("Argument expected");
    return;
  }
  if (!require_evaluated_metaprogram(displayer_)) {
    return;
  }
  try {
    breakpoint_t breakpoint = std::make_tuple(arg, boost::regex(arg));

    unsigned match_count = 0;
    for (metaprogram::vertex_descriptor vertex : mp->get_vertices()) {
      if (breakpoint_match(vertex, breakpoint)) {
        match_count += mp->get_traversal_count(vertex);
      }
    }
    if (match_count == 0) {
      displayer_.show_raw_text(
          "Breakpoint \"" + arg + "\" will never stop the execution");
    } else {
      displayer_.show_raw_text(
          "Breakpoint \"" + arg + "\" will stop the execution on " +
          std::to_string(match_count) +
          (match_count > 1 ? " locations" : " location"));
      breakpoints.push_back(breakpoint);
    }
  } catch (const boost::regex_error&) {
    displayer_.show_error("\"" + arg + "\" is not a valid regex");
  }
}

void mdb_shell::command_help(
    const std::string& arg,
    iface::displayer& displayer_)
{
  if (arg.empty()) {
    displayer_.show_raw_text("List of available commands:");
    displayer_.show_raw_text("");
    for (const mdb_command& cmd : command_handler.get_commands()) {
      displayer_.show_raw_text(
          cmd.get_keys().front() + " -- " +
          cmd.get_short_description());
    }
    displayer_.show_raw_text("");
    displayer_.show_raw_text(
      "Type \"help\" followed by a command name for more information."
    );
    displayer_.show_raw_text(
      "Command name abbreviations are allowed if unambiguous."
    );
    displayer_.show_raw_text(
      "A blank line as an input will repeat the last command,"
      " if it makes sense."
    );
    return;
  }

  auto command_arg_pair = command_handler.get_command_for_line(arg);
  if (!command_arg_pair) {
    displayer_.show_error("Command not found\n");
    return;
  }

  using boost::algorithm::join;

  mdb_command cmd;
  std::string command_args;
  std::tie(cmd, command_args) = *command_arg_pair;

  if (!command_args.empty()) {
    displayer_.show_error("Only one argument expected\n");
    return;
  }

  displayer_.show_raw_text(join(cmd.get_keys(), "|") + " " + cmd.get_usage());
  displayer_.show_raw_text(cmd.get_full_description());
}

void mdb_shell::command_quit(
    const std::string& arg,
    iface::displayer& displayer_)
{
  if (!require_empty_args(arg, displayer_)) {
    return;
  }
  is_stopped = true;
}

bool mdb_shell::run_metaprogram_with_templight(
    const boost::optional<std::string>& expression,
    metaprogram::mode_t mode,
    iface::displayer& displayer_)
{
  temporary_file templight_output_file("templight.pb");
  std::string output_path = templight_output_file.get_path();

  env.set_output_location(output_path);

  data::type_or_error evaluation_result =
    run_metaprogram(expression, displayer_);

  // Opening in binary mode, because some platforms interpret some characters
  // specially in text mode, which caused parsing to fail.
  std::ifstream protobuf_stream(output_path + ".trace.pbf",
      std::ios_base::in | std::ios_base::binary);

  if (!protobuf_stream) {
    if (evaluation_result.is_error()) {
      displayer_.show_error(evaluation_result.get_error());
    } else {
      // Shouldn't happen
      displayer_.show_error("Unexpected type type_or_error result");
    }
    mp = boost::none;
    return false;
  }

  mp = metaprogram::create_from_protobuf_stream(
      protobuf_stream,
      mode,
      expression ? *expression : "<environment>",
      data::file_location{}, // TODO something sensible here?
      evaluation_result);

  assert(mp);
  if (mp->is_empty() && evaluation_result.is_error()) {
    // Most errors will cause templight to generate an empty trace
    // We're only intrested in non-empty traces
    displayer_.show_error(evaluation_result.get_error());
    mp = boost::none;
    return false;
  }

  return true;
}

data::type_or_error mdb_shell::run_metaprogram(
    const boost::optional<std::string>& expression,
    iface::displayer& displayer_)
{
  result res = expression ?
    eval_tmp(env, *expression, conf, _logger) :
    eval_environment(env, conf, _logger);

  if (!res.info.empty()) {
    displayer_.show_raw_text(res.info);
  }

  if (!res.successful) {
    return data::type_or_error::make_error(res.error);
  } else if (expression) {
    return data::type_or_error::make_type(data::type(res.output));
  } else {
    return data::type_or_error::make_none();
  }
}

boost::optional<int> mdb_shell::parse_defaultable_integer(
    const std::string& arg, int default_value)
{
  using boost::spirit::qi::int_;
  using boost::spirit::ascii::space;
  using boost::phoenix::ref;
  using boost::spirit::qi::_1;

  auto begin = arg.begin(),
       end = arg.end();

  int value = default_value;

  bool result =
    boost::spirit::qi::phrase_parse(
        begin, end,

        -int_[ref(value) = _1],

        space
    );

  if (!result || begin != end) {
    return boost::none;
  }
  return value;
}

boost::optional<int> mdb_shell::parse_mandatory_integer(
    const std::string& arg)
{
  using boost::spirit::qi::int_;
  using boost::spirit::ascii::space;
  using boost::phoenix::ref;
  using boost::spirit::qi::_1;

  auto begin = arg.begin(),
       end = arg.end();

  int value = 0;

  bool result =
    boost::spirit::qi::phrase_parse(
        begin, end,

        int_[ref(value) = _1],

        space
    );

  if (!result || begin != end) {
    return boost::none;
  }
  return value;
}

mdb_shell::breakpoints_t::iterator mdb_shell::continue_metaprogram(
    direction_t direction)
{
  assert(!mp->is_at_endpoint(direction));

  while (true) {
    mp->step(direction);
    if (mp->is_at_endpoint(direction)) {
      return breakpoints.end();
    }
    for (auto it = breakpoints.begin(); it != breakpoints.end(); ++it) {
      if (breakpoint_match(mp->get_current_vertex(), *it)) {
        return it;
      }
    }
  }
}

unsigned mdb_shell::finish_metaprogram() {
  unsigned steps = 0;
  for (; !mp->is_finished(); ++steps) {
    mp->step();
  }
  return steps;
}

void mdb_shell::next_metaprogram(direction_t direction, int n) {
  assert(n >= 0);
  for (int i = 0; i < n && !mp->is_at_endpoint(direction); ++i)
  {
    unsigned bt_depth = mp->get_backtrace_length();
    do {
      mp->step(direction);
    } while (!mp->is_at_endpoint(direction) &&
        mp->get_backtrace_length() > bt_depth);
  }
}

void mdb_shell::display_frame(
  const data::frame& frame, iface::displayer& displayer_) const
{
  displayer_.show_frame(frame);

  data::file_location source_location = frame.source_location();
  if (source_location.name == env.env_filename()) {
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

void mdb_shell::display_current_frame(iface::displayer& displayer_) const {
  assert(mp);
  assert(!mp->is_at_start());
  assert(!mp->is_finished());

  display_frame(mp->get_current_frame(), displayer_);
}

void mdb_shell::display_current_forwardtrace(
    boost::optional<int> max_depth,
    iface::displayer& displayer_) const
{
  displayer_.show_call_graph(
    boost::make_iterator_range(
      forward_trace_iterator(*mp, max_depth),
      forward_trace_iterator()
    )
  );
}

void mdb_shell::display_backtrace(iface::displayer& displayer_) const {
  displayer_.show_backtrace(mp->get_backtrace());
}

void mdb_shell::display_argument_parsing_failed(
    iface::displayer& displayer_) const
{
  displayer_.show_error("Argument parsing failed");
}

void mdb_shell::display_metaprogram_reached_the_beginning(
    iface::displayer& displayer_) const
{
  displayer_.show_raw_text("Metaprogram reached the beginning");
}

void mdb_shell::display_metaprogram_finished(
    iface::displayer& displayer_) const
{
  displayer_.show_raw_text("Metaprogram finished");
  displayer_.show_type_or_error(mp->get_evaluation_result());
}

void mdb_shell::display_movement_info(
    bool moved,
    iface::displayer& displayer_) const
{
  if (mp->is_finished()) {
    if (moved) {
      display_metaprogram_finished(displayer_);
    }
  } else if (mp->is_at_start()) {
    if (moved) {
      display_metaprogram_reached_the_beginning(displayer_);
    }
  } else {
    display_current_frame(displayer_);
  }
}

void mdb_shell::cancel_operation() {
  // TODO
}

void mdb_shell::code_complete(
    const std::string&,
    std::set<std::string>&) const
{
  // TODO
}

void mdb_shell::line_available(
  const std::string& line, iface::displayer& displayer_)
{
  null_history h;
  line_available(line, displayer_, h);
}

}

