
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

#include <boost/assign.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_core.hpp>

#include <boost/range/iterator_range.hpp>

namespace
{
  const std::string internal_file_name = "mdb-stdin";

  const std::string wrap_prefix = "metashell::impl::wrap<";
  const std::string wrap_suffix = ">";

  typedef
    std::tuple<
      metashell::file_location,
      metashell::instantiation_kind,
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
}

namespace metashell {

const mdb_command_handler_map mdb_shell::command_handler =
  mdb_command_handler_map(
    {
      {{"evaluate"}, non_repeatable, &mdb_shell::command_evaluate,
        "[-full] [<type>]",
        "Evaluate and start debugging a new metaprogram.",
        "Evaluating a metaprogram using the `-full` qualifier will expand all\n"
        "Memoization events.\n\n"
        "If called without <type>, then the last evaluated metaprogram will be\n"
        "reevaluated.\n\n"
        "Previous breakpoints are cleared.\n\n"
        "Unlike metashell, evaluate doesn't use metashell::format to avoid cluttering\n"
        "the debugged metaprogram with unrelated code. If you need formatting, you can\n"
        "explicitly enter `metashell::format< <type> >::type` for the same effect."},
      {{"step"}, repeatable, &mdb_shell::command_step,
        "[over] [n]",
        "Step the program.",
        "Argument n means step n times. n defaults to 1 if not specified.\n"
        "Negative n means step the program backwards.\n\n"
        "Use of the `over` qualifier will jump over sub instantiations.\n"
        "Please note that `step over -1` is not always the inverse of `step over`.\n"
        "In particular when there are no more instantiations that got instantiated\n"
        "by the current parent, then `step over` will behave like a normal `step`,\n"
        "and will step out of one or more instantiation frames."},
      {{"rbreak"}, non_repeatable, &mdb_shell::command_rbreak,
        "<regex>",
        "Add breakpoint for all types matching `<regex>`.",
        ""},
      {{"continue"}, repeatable, &mdb_shell::command_continue,
        "[n]",
        "Continue program being debugged.",
        "The program is continued until the nth breakpoint or the end of the program\n"
        "is reached. n defaults to 1 if not specified.\n"
        "Negative n means continue the program backwards."},
      {{"forwardtrace", "ft"}, non_repeatable, &mdb_shell::command_forwardtrace,
        "[n]",
        "Print forwardtrace from the current point.",
        "The n specifier limits the depth of the trace. If n is not specified, then the\n"
        "trace depth is unlimited."},
      {{"backtrace", "bt"}, non_repeatable, &mdb_shell::command_backtrace,
        "",
        "Print backtrace from the current point.",
        ""},
      {{"help"}, non_repeatable, &mdb_shell::command_help,
        "[<command>]",
        "Show help for commands.",
        "If <command> is not specified, show a list of all available commands."},
      {{"quit"} , non_repeatable, &mdb_shell::command_quit,
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
    const environment& env_arg) :
  conf(set_pch_false(conf_)),
  env(conf)
{
  env.append(env_arg.get_all());
}

std::string mdb_shell::prompt() const {
  return "(mdb) ";
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

    (this->*cmd.get_func())(args, displayer_);
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

void mdb_shell::command_continue(
    const std::string& arg,
    iface::displayer& displayer_)
{
  if (!require_evaluated_metaprogram(displayer_)) {
    return;
  }

  using boost::spirit::qi::int_;
  using boost::spirit::ascii::space;
  using boost::phoenix::ref;
  using boost::spirit::qi::_1;

  auto begin = arg.begin(),
       end = arg.end();

  int continue_count = 1;

  bool result =
    boost::spirit::qi::phrase_parse(
        begin, end,

        -int_[ref(continue_count) = _1],

        space
    );

  if (!result || begin != end) {
    display_argument_parsing_failed(displayer_);
    return;
  }

  if (continue_count == 0) {
    return;
  }

  metaprogram::direction_t direction =
    continue_count >= 0 ? metaprogram::forward : metaprogram::backwards;

  breakpoints_t::iterator breakpoint_it = breakpoints.end();
  for (int i = 0;
      i < std::abs(continue_count) && !mp->is_at_endpoint(direction); ++i)
  {
    breakpoint_it = continue_metaprogram(direction);
  }

  if (mp->is_finished()) {
    if (continue_count > 0) {
      display_metaprogram_finished(displayer_);
    }
  } else if (mp->is_at_start()) {
    if (continue_count < 0) {
      display_metaprogram_reached_the_beginning(displayer_);
    }
  } else {
    assert(breakpoint_it != breakpoints.end());
    displayer_.show_raw_text(
        "Breakpoint \"" + std::get<0>(*breakpoint_it) + "\" reached");
    display_current_frame(displayer_);
  }
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
  enum { normal, over } step_type = normal;

  bool result =
    boost::spirit::qi::phrase_parse(
        begin, end,

        -lit("over")[ref(step_type) = over] >>
        -int_[ref(step_count) = _1],

        space
    );

  if (!result || begin != end) {
    display_argument_parsing_failed(displayer_);
    return;
  }

  metaprogram::direction_t direction =
    step_count >= 0 ? metaprogram::forward : metaprogram::backwards;

  int iteration_count = std::abs(step_count);

  switch (step_type) {
    case normal:
      for (int i = 0;
          i < iteration_count && !mp->is_at_endpoint(direction); ++i)
      {
        mp->step(direction);
      }
      break;
    case over:
      {
        for (int i = 0;
            i < iteration_count && !mp->is_at_endpoint(direction); ++i)
        {
          unsigned bt_depth = mp->get_backtrace_length();
          do {
            mp->step(direction);
          } while (!mp->is_at_endpoint(direction) &&
              mp->get_backtrace_length() > bt_depth);
        }
      }
      break;
    default:
      assert(false);
      break;
  }

  if (mp->is_finished()) {
    if (step_count > 0) {
      display_metaprogram_finished(displayer_);
    }
  } else if (mp->is_at_start()) {
    if (step_count < 0) {
      display_metaprogram_reached_the_beginning(displayer_);
    }
  } else {
    display_current_frame(displayer_);
  }
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


void mdb_shell::filter_enable_reachable_from_current_line() {
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
    // Filter out edges, that is not instantiated by the entered type
    if (property.point_of_instantiation.name == internal_file_name &&
        property.point_of_instantiation.row == line_number + 1 &&
        (property.kind == instantiation_kind::template_instantiation ||
        property.kind == instantiation_kind::memoization) &&
        (!is_wrap_type(target_name) ||
         property.kind != instantiation_kind::memoization))
    {
      property.enabled = true;
      edge_stack.push(edge);
    }
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

    for (edge_descriptor out_edge : mp->get_out_edges(vertex)) {
      edge_property& property = mp->get_edge_property(out_edge);
      if (property.kind == instantiation_kind::template_instantiation ||
         property.kind == instantiation_kind::memoization)
      {
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
            instantiation_kind::non_template_type;
        }
      }
    }
  }
}

void mdb_shell::filter_similar_edges() {

  using vertex_descriptor = metaprogram::vertex_descriptor;
  using edge_descriptor = metaprogram::edge_descriptor;
  using edge_property = metaprogram::edge_property;

  auto comparator =
    mp->is_in_full_mode() ? less_than_ignore_instantiation_kind : less_than;

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

void mdb_shell::filter_metaprogram() {
  assert(mp);

  filter_disable_everything();
  filter_enable_reachable_from_current_line();
  filter_unwrap_vertices();
  filter_similar_edges();
}

void mdb_shell::command_evaluate(
    const std::string& arg,
    iface::displayer& displayer_)
{
  // Easier not to use spirit here (or probably not...)

  using boost::starts_with;

  const std::string full_flag = "-full";

  const bool has_full =
      boost::starts_with(arg, full_flag) &&
      (arg.size() == full_flag.size() || std::isspace(arg[full_flag.size()]));

  std::string type =
      boost::trim_copy(has_full ? arg.substr(full_flag.size()) : arg);

  if (type.empty()) {
    if (!mp) {
      displayer_.show_error("Nothing has been evaluated yet.");
      return;
    }
    type = mp->get_vertex_property(mp->get_root_vertex()).name;
  }

  breakpoints.clear();

  if (!run_metaprogram_with_templight(type, has_full, displayer_)) {
    return;
  }
  displayer_.show_raw_text("Metaprogram started");

  filter_metaprogram();
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
  if (
    require_empty_args(arg, displayer_)
    && require_running_metaprogram(displayer_)
  ) {
    display_backtrace(displayer_);
  }
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
    const std::string& str, bool full_mode, iface::displayer& displayer_)
{
  temporary_file templight_output_file("templight.pb");
  std::string output_path = templight_output_file.get_path();

  env.set_output_location(output_path);

  boost::optional<type> evaluation_result = run_metaprogram(str, displayer_);

  if (!evaluation_result) {
    mp = boost::none;
    return false;
  }

  mp = metaprogram::create_from_xml_file(
      output_path + ".trace.pbf", full_mode, str, *evaluation_result);
  return true;
}

boost::optional<type> mdb_shell::run_metaprogram(
    const std::string& str,
    iface::displayer& displayer_)
{
  result res = eval_tmp(env, str, conf);

  if (!res.info.empty()) {
    displayer_.show_raw_text(res.info);
  }

  if (res.has_errors()) {
    for (const std::string& e : res.errors) {
      displayer_.show_error(e);
    }
    return boost::none;
  }
  return type(res.output);
}

mdb_shell::breakpoints_t::iterator mdb_shell::continue_metaprogram(
    metaprogram::direction_t direction)
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

void mdb_shell::display_current_frame(iface::displayer& displayer_) const {
  assert(mp);
  assert(!mp->is_at_start());
  assert(!mp->is_finished());

  displayer_.show_frame(mp->get_current_frame());
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
  displayer_.show_type(mp->get_evaluation_result());
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

