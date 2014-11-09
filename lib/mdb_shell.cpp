
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

#include <cmath>

#include <boost/assign.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/range/adaptor/reversed.hpp>

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
        "Use of the `over` qualifier will jump over sub instantiations."},
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

const std::string mdb_shell::internal_file_name = "mdb-stdin";

const std::vector<color> mdb_shell::colors =
  {
    color::red,
    color::green,
    color::yellow,
    color::blue,
    color::cyan
  };

const std::string mdb_shell::wrap_prefix = "metashell::impl::wrap<";
const std::string mdb_shell::wrap_suffix = ">";

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

mdb_shell::~mdb_shell() {}

void mdb_shell::display(const colored_string& cs) const {
  display(cs, 0, cs.size());
}

std::string mdb_shell::prompt() const {
  return "(mdb) ";
}

bool mdb_shell::stopped() const {
  return is_stopped;
}

void mdb_shell::display_splash() const {
  display_info(
      "For help, type \"help\".\n"
  );
}

void mdb_shell::line_available(const std::string& line_arg) {

  try {
    using boost::algorithm::all;
    using boost::is_space;

    std::string line = line_arg;

    if (line != prev_line && !line.empty()) {
      add_history(line);
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
      display_error("Command parsing failed\n");
      last_command_repeatable = false;
      return;
    }

    mdb_command cmd;
    std::string args;
    std::tie(cmd, args) = *command_arg_pair;

    last_command_repeatable = cmd.is_repeatable();

    (this->*cmd.get_func())(args);
  } catch (const std::exception& ex) {
    display_error(std::string("Error: ") + ex.what() + "\n");
  } catch (...) {
    display_error("Unknown error\n");
  }
}
bool mdb_shell::breakpoint_match(
    metaprogram::vertex_descriptor vertex, const breakpoint_t& breakpoint)
{
  return boost::regex_search(
      mp->get_vertex_property(vertex).name, std::get<1>(breakpoint));
}

bool mdb_shell::require_empty_args(const std::string& args) const {
  if (!args.empty()) {
    display_error("This command doesn't accept arguments\n");
    return false;
  }
  return true;
}

bool mdb_shell::require_evaluated_metaprogram() const {
  if (!mp) {
    display_error("Metaprogram not evaluated yet\n");
    return false;
  }
  return true;
}

bool mdb_shell::require_running_metaprogram() const {
  if (!require_evaluated_metaprogram()) {
    return false;
  }

  if (mp->is_finished()) {
    display_metaprogram_finished();
    return false;
  }
  return true;
}

void mdb_shell::command_continue(const std::string& arg) {
  if (!require_evaluated_metaprogram()) {
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
    display_argument_parsing_failed();
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
      display_metaprogram_finished();
    }
  } else if (mp->is_at_start()) {
    if (continue_count < 0) {
      display_metaprogram_reached_the_beginning();
    }
  } else {
    assert(breakpoint_it != breakpoints.end());
    display_info(
        "Breakpoint \"" + std::get<0>(*breakpoint_it) + "\" reached\n");
    display_current_frame();
  }
}

void mdb_shell::command_step(const std::string& arg) {
  if (!require_evaluated_metaprogram()) {
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
    display_argument_parsing_failed();
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
      display_metaprogram_finished();
    }
  } else if (mp->is_at_start()) {
    if (step_count < 0) {
      display_metaprogram_reached_the_beginning();
    }
  } else {
    display_current_frame();
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

  // Clang sometimes produces equivalent instantiations events from the same
  // point. Filter out all but one of each
  for (vertex_descriptor vertex : mp->get_vertices()) {

    typedef std::tuple<file_location, instantiation_kind, vertex_descriptor>
      set_element_t;

    std::set<set_element_t> similar_edges;

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

void mdb_shell::command_evaluate(const std::string& arg) {
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
      display_error("Nothing has been evaluated yet.\n");
      return;
    }
    type = mp->get_vertex_property(mp->get_root_vertex()).name;
  }

  breakpoints.clear();

  if (!run_metaprogram_with_templight(type, has_full)) {
    return;
  }
  display_info("Metaprogram started\n");

  filter_metaprogram();
}

void mdb_shell::command_forwardtrace(const std::string& arg) {
  if (!require_running_metaprogram()) {
    return;
  }

  using boost::spirit::qi::lit;
  using boost::spirit::qi::uint_;
  using boost::spirit::ascii::space;
  using boost::spirit::qi::_1;

  namespace phx = boost::phoenix;

  auto begin = arg.begin(),
       end = arg.end();

  boost::optional<unsigned> max_depth;

  bool result =
    boost::spirit::qi::phrase_parse(
        begin, end,

        -uint_ [phx::ref(max_depth) =_1],

        space
    );

  if (!result || begin != end) {
    display_argument_parsing_failed();
    return;
  }

  display_current_forwardtrace(max_depth);
}

void mdb_shell::command_backtrace(const std::string& arg) {
  if (!require_empty_args(arg) || !require_running_metaprogram()) {
    return;
  }
  display_backtrace();
}

void mdb_shell::command_rbreak(const std::string& arg) {
  if (arg.empty()) {
    display_error("Argument expected\n");
    return;
  }
  if (!require_evaluated_metaprogram()) {
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
      display_info(
          "Breakpoint \"" + arg + "\" will never stop the execution\n");
    } else {
      display_info(
          "Breakpoint \"" + arg + "\" will stop the execution on " +
          std::to_string(match_count) +
          (match_count > 1 ? " locations" : " location") + "\n");
      breakpoints.push_back(breakpoint);
    }
  } catch (const boost::regex_error&) {
    display_error("\"" + arg + "\" is not a valid regex\n");
  }
}

void mdb_shell::command_help(const std::string& arg) {
  if (arg.empty()) {
    display_info(
        "List of available commands:\n\n");
    for (const mdb_command& cmd : command_handler.get_commands()) {
      display_info(
          cmd.get_keys().front() + " -- " +
          cmd.get_short_description() + "\n");
    }
    display_info(
        "\n"
        "Type \"help\" followed by a command name for more information.\n"
        "Command name abbreviations are allowed if unambiguous.\n"
        "A blank line as an input will repeat the last command, if it makes sense.\n");
    return;
  }

  auto command_arg_pair = command_handler.get_command_for_line(arg);
  if (!command_arg_pair) {
    display_error("Command not found\n");
    return;
  }

  using boost::algorithm::join;

  mdb_command cmd;
  std::string command_args;
  std::tie(cmd, command_args) = *command_arg_pair;

  if (!command_args.empty()) {
    display_error("Only one argument expected\n");
    return;
  }

  display_info(
      join(cmd.get_keys(), "|") + " " + cmd.get_usage() + "\n" +
      cmd.get_full_description() + "\n");
}

void mdb_shell::command_quit(const std::string& arg) {
  if (!require_empty_args(arg)) {
    return;
  }
  is_stopped = true;
}

bool mdb_shell::run_metaprogram_with_templight(
    const std::string& str, bool full_mode)
{
  temporary_file templight_xml_file("templight-%%%%-%%%%-%%%%-%%%%.xml");
  std::string xml_path = templight_xml_file.get_path().string();

  env.set_xml_location(xml_path);

  boost::optional<std::string> evaluation_result = run_metaprogram(str);

  if (!evaluation_result) {
    mp = boost::none;
    return false;
  }

  mp = metaprogram::create_from_xml_file(
      xml_path, full_mode, str, *evaluation_result);
  return true;
}

boost::optional<std::string> mdb_shell::run_metaprogram(
    const std::string& str)
{
  result res = eval_tmp_unformatted(env, str, conf, internal_file_name);

  if (!res.info.empty()) {
    display_info(res.info);
  }

  if (res.has_errors()) {
    for (const std::string& e : res.errors) {
      display_error(e + "\n");
    }
    return boost::none;
  }
  return res.output;
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

void mdb_shell::display_error(const std::string& str) const {
  display(colored_string(str, color::bright_red));
}

void mdb_shell::display_info(const std::string& str) const {
  display(str);
}

void mdb_shell::display_current_frame() const {
  assert(mp && !mp->is_at_start() && !mp->is_finished());
  display(get_frame_string(*mp->get_current_edge()) + "\n");
}

void mdb_shell::display_trace_graph(
    unsigned depth,
    const std::vector<unsigned>& depth_counter,
    bool print_mark) const
{
  assert(depth_counter.size() > depth);

  if (depth > 0) {
    //TODO respect the -H (no syntax highlight parameter)
    for (unsigned i = 1; i < depth; ++i) {
      display(colored_string(
          depth_counter[i] > 0 ? "| " : "  ",
          colors[i % colors.size()]));
    }

    color mark_color = colors[depth % colors.size()];
    if (print_mark) {
      if (depth_counter[depth] == 0) {
        display(colored_string("` ", mark_color));
      } else {
        display(colored_string("+ ", mark_color));
      }
    } else if (depth_counter[depth] > 0) {
      display(colored_string("| ", mark_color));
    } else {
      display("  ");
    }
  }
}

void mdb_shell::display_trace_line(
    metaprogram::optional_edge_descriptor edge,
    unsigned depth,
    const std::vector<unsigned>& depth_counter,
    unsigned width) const
{

  colored_string element_content = get_frame_string(edge);

  unsigned non_content_length = 2*depth;

  const unsigned pretty_print_threshold = 10;
  if (width < pretty_print_threshold ||
      non_content_length >= width - pretty_print_threshold)
  {
    // We have no chance to display the graph nicely :(
    display_trace_graph(depth, depth_counter, true);

    display(element_content);
    display("\n");
  } else {
    unsigned content_width = width - non_content_length;
    for (unsigned i = 0; i < element_content.size(); i += content_width) {
      display_trace_graph(depth, depth_counter, i == 0);
      display(element_content, i, content_width);
      display("\n");
    }
  }
}

void mdb_shell::display_trace_visit(
    metaprogram::optional_edge_descriptor root_edge,
    boost::optional<unsigned> max_depth,
    metaprogram::discovered_t& discovered,
    unsigned width) const
{

  // -----
  // Customized DFS
  //   The algorithm only checks vertices which are reachable from root_vertex
  // ----

  // This vector counts how many elements are in the to_visit
  // stack for each specific depth.
  // The purpose is to not draw pipes, when a tree element
  // doesn't have any more children.
  // The 0th element is never read.

  const metaprogram::graph_t& graph = mp->get_graph();

  std::vector<unsigned> depth_counter(1);

  typedef std::tuple<
    metaprogram::optional_edge_descriptor,
    unsigned // Depth
  > stack_element;

  // The usual stack for DFS
  std::stack<stack_element> to_visit;

  to_visit.push(std::make_tuple(root_edge, 0));
  ++depth_counter[0]; // This value is neved read

  while (!to_visit.empty()) {
    metaprogram::optional_edge_descriptor edge;
    unsigned depth;
    std::tie(edge, depth) = to_visit.top();
    to_visit.pop();

    --depth_counter[depth];

    metaprogram::vertex_descriptor vertex = [&] {
      if (edge) return mp->get_target(*edge);
      return mp->get_root_vertex();
    }();

    display_trace_line(edge, depth, depth_counter, width);

    if (discovered[vertex]) {
      continue;
    }
    if (!mp->is_in_full_mode()) {
      discovered[vertex] = true;
    }

    if (max_depth && *max_depth <= depth) {
      continue;
    }

    metaprogram::out_edge_iterator begin, end;
    std::tie(begin, end) = boost::out_edges(vertex, graph);

    typedef std::vector<metaprogram::edge_descriptor> edges_t;
    edges_t edges(begin, end);

    if (depth_counter.size() <= depth+1) {
      depth_counter.resize(depth+1+1);
    }

    // Reverse iteration, so types that got instantiated first
    // get on the top of the stack
    for (const metaprogram::edge_descriptor& out_edge :
        edges | boost::adaptors::reversed)
    {
      if (mp->get_edge_property(out_edge).enabled) {
        to_visit.push(std::make_tuple(out_edge, depth+1));

        ++depth_counter[depth+1];
      }
    }
  }
}

void mdb_shell::display_current_forwardtrace(
    boost::optional<unsigned> max_depth) const
{
  metaprogram::discovered_t discovered = mp->get_state().discovered;

  display_trace_visit(mp->get_current_edge(), max_depth, discovered, width());
}

colored_string mdb_shell::get_frame_string(
    metaprogram::optional_edge_descriptor frame) const
{
  if (!frame) {
    return highlight_syntax(
        mp->get_vertex_property(mp->get_root_vertex()).name);
  }

  colored_string result =
    highlight_syntax(mp->get_vertex_property(mp->get_target(*frame)).name);

  if (!mp->is_in_full_mode()) {
    result += " (" + to_string(mp->get_edge_property(*frame).kind) + ")";
  }

  return result;
}

void mdb_shell::display_backtrace() const {
  const metaprogram::backtrace_t& backtrace = mp->get_backtrace();

  for (unsigned i = 0; i < backtrace.size(); ++i) {
    display(colored_string("#" + std::to_string(i) + " ", color::white));
    display(get_frame_string(backtrace[i]) + "\n");
  }

  display(colored_string(
        "#" + std::to_string(backtrace.size()) + " ", color::white));
  display(highlight_syntax(
        mp->get_vertex_property(mp->get_root_vertex()).name) + "\n");
}

void mdb_shell::display_argument_parsing_failed() const {
  display_error("Argument parsing failed\n");
}

void mdb_shell::display_metaprogram_reached_the_beginning() const {
  display("Metaprogram reached the beginning\n");
}

void mdb_shell::display_metaprogram_finished() const {
  display(
      "Metaprogram finished\n" +
      highlight_syntax(mp->get_evaluation_result()) + "\n");
}

}

