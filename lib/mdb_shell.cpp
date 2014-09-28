
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

#if defined __clang__
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include <boost/assign.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/range/adaptor/reversed.hpp>

#if defined __clang__
#  pragma GCC diagnostic error "-Wunused-parameter"
#endif

namespace metashell {

const mdb_command_handler_map mdb_shell::command_handler =
  mdb_command_handler_map(
    {
      {{"continue"}, repeatable, &mdb_shell::command_continue,
        "[n]",
        "Continue program being debugged.",
        "The program is continued until the nth breakpoint or the end of the program\n"
        "is reached. n defaults to 1 if not specified.\n"
        "Negative n means continue the program backwards."},
      {{"step"}, repeatable, &mdb_shell::command_step,
        "[n]",
        "Step the program.",
        "Argument n means step n times. n defaults to 1 if not specified.\n"
        "Negative n means step the program backwards."},
      {{"evaluate"}, non_repeatable, &mdb_shell::command_evaluate,
        "<type>",
        "Evaluate and start debugging a new metaprogram.",
        "Unlike metashell, evaluate doesn't use metashell::format to avoid cluttering\n"
        "the debugged metaprogram with unrelated code. If you need formatting, you can\n"
        "explicitly enter `metashell::format< <type> >::type` for the same effect."},
      {{"forwardtrace", "ft"}, non_repeatable, &mdb_shell::command_forwardtrace,
        "[full] [n]",
        "Print forwardtrace from the current point.",
        "Use of the full qualifier will expand Memoizations even if that instantiation\n"
        "path has been visited before.\n\n"
        "The n specifier limits the depth of the trace. If n is not specified, then the\n"
        "trace depth is unlimited."},
      {{"backtrace", "bt"}, non_repeatable, &mdb_shell::command_backtrace,
        "",
        "Print backtrace from the current point.",
        ""},
      {{"rbreak"}, non_repeatable, &mdb_shell::command_rbreak,
        "<regex>",
        "Add breakpoint for all types matching `<regex>`.",
        ""},
      {{"help"}, non_repeatable, &mdb_shell::command_help,
        "[command]",
        "Show help for commands.",
        "If no [command] is specified, show a list of all available commands."},
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


mdb_shell::mdb_shell(
    const config& conf,
    const environment& env_arg) :
  conf(conf),
  env("__mdb_internal", conf)
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

  if (continue_count > 0) {
    for (int i = 0; i < continue_count && !mp->is_finished(); ++i) {
      continue_metaprogram();
    }
  } else {
    for (int i = 0; i < -continue_count && !mp->is_at_start(); ++i) {
      continue_back_metaprogram();
    }
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
    display_info("Breakpoint reached\n");
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

  bool result =
    boost::spirit::qi::phrase_parse(
        begin, end,

        -int_[ref(step_count) = _1],

        space
    );

  if (!result || begin != end) {
    display_argument_parsing_failed();
    return;
  }

  if (step_count >= 0) {
    for (int i = 0; i < step_count && !mp->is_finished(); ++i) {
      mp->step();
    }
  } else {
    for (int i = 0; i < -step_count && !mp->is_at_start(); ++i) {
      mp->step_back();
    }
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

void mdb_shell::command_evaluate(const std::string& arg) {
  using boost::starts_with;
  using boost::ends_with;
  using boost::trim_copy;

  if (arg.empty()) {
    mp = boost::none;
    display_error("Argument expected\n");
    return;
  }

  if (!run_metaprogram_with_templight(arg)) {
    return;
  }
  display_info("Metaprogram started\n");

  std::string env_buffer = env.get();
  int line_number = std::count(env_buffer.begin(), env_buffer.end(), '\n');

  static const std::string wrap_prefix = "metashell::impl::wrap<";
  static const std::string wrap_suffix = ">";

  // TODO this check could be made more strict,
  // since we now whats inside wrap<...> (mp->get_evaluation_result)
  auto is_wrap_type = [](const std::string& type) {
    return starts_with(type, wrap_prefix) && ends_with(type, wrap_suffix);
  };

  mp->disable_edges_if(
    [&](const metaprogram::edge_descriptor& edge) {
      const metaprogram::edge_property& edge_property =
        mp->get_edge_property(edge);
      const std::string& target_name =
        mp->get_vertex_property(mp->get_target(edge)).name;
      return
        mp->get_source(edge) == mp->get_root_vertex() &&
        (
          edge_property.point_of_instantiation.name != internal_file_name ||
          edge_property.point_of_instantiation.row != line_number + 2 ||
          (is_wrap_type(target_name) && edge_property.kind == memoization)
        );
    }
  );
  for (metaprogram::vertex_descriptor vertex : mp->get_vertices()) {
    std::string& name = mp->get_vertex_property(vertex).name;
    if (is_wrap_type(name)) {
      name = trim_copy(name.substr(
          wrap_prefix.size(),
          name.size() - wrap_prefix.size() - wrap_suffix.size()));
      if (!is_template_type(name)) {
        for (metaprogram::edge_descriptor in_edge : mp->get_in_edges(vertex)) {
          mp->get_edge_property(in_edge).kind = non_template_type;
        }
      }
    }
  }
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

  bool has_full = false;
  boost::optional<unsigned> max_depth;

  bool result =
    boost::spirit::qi::phrase_parse(
        begin, end,

        -lit("full") [phx::ref(has_full) = true] >>
        -uint_ [phx::ref(max_depth) =_1],

        space
    );

  if (!result || begin != end) {
    display_argument_parsing_failed();
    return;
  }

  if (has_full) {
    display_current_full_forwardtrace(max_depth);
  } else {
    display_current_forwardtrace(max_depth);
  }
}

void mdb_shell::command_backtrace(const std::string& arg) {
  if (!require_empty_args(arg) || !require_running_metaprogram()) {
    return;
  }
  display_backtrace();
}

void mdb_shell::command_rbreak(const std::string& arg) {
  try {
    breakpoints.push_back(boost::regex(arg));
    display_info("Break point \"" + arg + "\" added\n");
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
    const std::string& str)
{
  temporary_file templight_xml_file("templight-%%%%-%%%%-%%%%-%%%%.xml");
  std::string xml_path = templight_xml_file.get_path().string();

  env.set_xml_location(xml_path);

  boost::optional<std::string> evaluation_result = run_metaprogram(str);

  if (!evaluation_result) {
    mp = boost::none;
    return false;
  }

  mp = metaprogram::create_from_xml_file(xml_path, str, *evaluation_result);
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

// TODO continue_metaprogram and continue_back_metaprogram need to be merged
// into a single function
void mdb_shell::continue_metaprogram() {
  assert(!mp->is_finished());

  while (true) {
    mp->step();
    if (mp->is_finished()) {
      return;
    }
    for (const breakpoint_t& breakpoint : breakpoints) {
      const std::string current_type =
        mp->get_vertex_property(
            mp->get_target(mp->get_current_frame())).name;

      if (boost::regex_search(current_type, breakpoint)) {
        return;
      }
    }
  }
}

void mdb_shell::continue_back_metaprogram() {
  assert(!mp->is_at_start());

  while (true) {
    mp->step_back();
    if (mp->is_at_start()) {
      return;
    }
    for (const breakpoint_t& breakpoint : breakpoints) {
      const std::string current_type =
        mp->get_vertex_property(
            mp->get_target(mp->get_current_frame())).name;

      if (boost::regex_search(current_type, breakpoint)) {
        return;
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
  display_frame(mp->get_current_frame());
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
    metaprogram::vertex_descriptor vertex,
    unsigned depth,
    const std::vector<unsigned>& depth_counter,
    const boost::optional<metaprogram::edge_property>& property,
    unsigned width) const
{

  colored_string element_content =
    highlight_syntax(mp->get_vertex_property(vertex).name);

  if (property) {
    element_content += " (" + to_string(property->kind) + ")";
  }

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

    auto property = [&]() -> boost::optional<metaprogram::edge_property> {
      if (edge) return mp->get_edge_property(*edge);
      return boost::none;
    }();

    display_trace_line(vertex, depth, depth_counter, property, width);

    if (discovered[vertex]) {
      continue;
    }
    discovered[vertex] = true;

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
    for (const metaprogram::edge_descriptor& edge :
        edges | boost::adaptors::reversed)
    {
      if (mp->get_edge_property(edge).enabled) {
        to_visit.push(std::make_tuple(edge, depth+1));

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

void mdb_shell::display_current_full_forwardtrace(
    boost::optional<unsigned> max_depth) const
{
  metaprogram::discovered_t discovered(mp->get_state().discovered.size());

  display_trace_visit(mp->get_current_edge(), max_depth, discovered, width());
}

void mdb_shell::display_frame(const metaprogram::edge_descriptor& frame) const {
  display(
      highlight_syntax(
        mp->get_vertex_property(mp->get_target(frame)).name) +
      " (" + to_string(mp->get_edge_property(frame).kind) + ")\n"
  );
}

void mdb_shell::display_backtrace() const {
  const metaprogram::backtrace_t& backtrace = mp->get_backtrace();

  for (unsigned i = 0; i < backtrace.size(); ++i) {
    display(colored_string("#" + std::to_string(i) + " ", color::white));
    display_frame(backtrace[i]);
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

