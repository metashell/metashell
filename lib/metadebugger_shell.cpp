
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

#include <metashell/metadebugger_shell.hpp>
#include <metashell/highlight_syntax.hpp>
#include <metashell/metashell.hpp>
#include <metashell/temporary_file.hpp>

#include <boost/regex.hpp>
#include <boost/assign.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/home/phoenix/operator.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/range/adaptor/reversed.hpp>

namespace metashell {

const std::vector<color> metadebugger_shell::colors =
  {
    color::red,
    color::green,
    color::yellow,
    color::blue,
    color::cyan
  };

metadebugger_command_handler_map::commands_t
  metadebugger_shell::create_default_command_map() {

  metadebugger_command_handler_map::commands_t res =
    {
      {{"continue"}, repeatable, &metadebugger_shell::command_continue,
        "",
        "Continue program being debugged.",
        "The program is continued until breakpoint or end of program."},
      {{"step"}, repeatable, &metadebugger_shell::command_step,
        "[over] [n]",
        "Step the program forward.",
        "Argument n means step n times. n defaults to 1 if not specified."},
      {{"evaluate"}, non_repeatable, &metadebugger_shell::command_evaluate,
        "<type>",
        "Evaluate and start debugging a new metaprogram.",
        "Unlike metashell, evaluate doesn't use metashell::format to avoid cluttering\n"
        "the debugged metaprogram with unrelated code. If you need formatting, you can\n"
        "explicitly enter `metashell::format< <type> >::type` for the same effect."},
      {{"forwardtrace", "ft"}, non_repeatable, &metadebugger_shell::command_forwardtrace,
        "",
        "Print forwardtrace from the current point.",
        ""},
      {{"backtrace", "bt"}, non_repeatable, &metadebugger_shell::command_backtrace,
        "",
        "Print backtrace from the current point.",
        ""},
      {{"break"}, non_repeatable, &metadebugger_shell::command_break,
        "[breakpoint]",
        "Add new breakpoint.",
        ""},
      {{"help"}, non_repeatable, &metadebugger_shell::command_help,
        "[command]",
        "Show help for commands.",
        "If no [command] is specified, show a list of all avaliable commands."},
      {{"quit"} , non_repeatable, &metadebugger_shell::command_quit,
        "",
        "Quit metadebugger.",
        ""}
    };
  return res;
}

metadebugger_shell::metadebugger_shell(
    const config& conf,
    const environment& env_arg) :
  conf(conf),
  env("__mdb_internal", conf),
  command_handler(create_default_command_map()),
  mp(metaprogram::create_empty_finished()),
  is_stopped(false)
{
  env.append(env_arg.get_all());
}

metadebugger_shell::~metadebugger_shell() {}

void metadebugger_shell::display(const colored_string& cs) const {
  display(cs, 0, cs.size());
}

std::string metadebugger_shell::prompt() const {
  return "(mdb) ";
}

bool metadebugger_shell::stopped() const {
  return is_stopped;
}

void metadebugger_shell::display_splash() const {
  display_info(
      "For help, type \"help\".\n"
  );
}

void metadebugger_shell::line_available(const std::string& original_line) {

  std::string line = boost::trim_copy(original_line);

  if (line.empty()) {
    if (prev_line && !prev_line->empty()) {
      line = *prev_line;
    } else {
      return;
    }
  } else if (line != prev_line) {
    add_history(line);
    prev_line = line;
  }

  auto command_arg_pair = command_handler.get_command_for_line(line);
  if (!command_arg_pair) {
    display_error("Command parsing failed\n");
    prev_line = boost::none;
    return;
  }

  metadebugger_command cmd;
  std::string args;
  std::tie(cmd, args) = *command_arg_pair;

  if (!cmd.is_repeatable()) {
    prev_line = boost::none;
  }

  (this->*cmd.get_func())(args);
}

bool metadebugger_shell::require_empty_args(const std::string& args) const {
  if (!args.empty()) {
    display_error("This command doesn't accept arguments\n");
    return false;
  }
  return true;
}

bool metadebugger_shell::require_running_metaprogram() const {
  if (mp.is_finished()) {
    display_error("Metaprogram finished or not evaluated yet\n");
    return false;
  }
  return true;
}

void metadebugger_shell::command_continue(const std::string& arg) {
  if (!require_empty_args(arg) || !require_running_metaprogram()) {
    return;
  }
  continue_metaprogram();
}

void metadebugger_shell::command_step(const std::string& arg) {
  if (!require_running_metaprogram()) {
    return;
  }

  using boost::spirit::qi::lit;
  using boost::spirit::qi::uint_;
  using boost::spirit::ascii::space;
  using boost::phoenix::ref;
  using boost::spirit::qi::_1;

  auto begin = arg.begin(),
       end = arg.end();

  bool has_over = false;
  unsigned step_count = 1;

  bool result =
    boost::spirit::qi::phrase_parse(
        begin, end,

        -lit("over") [ref(has_over) = true] >>
        -uint_ [ref(step_count) = _1],

        space
    );

  if (!result || begin != end) {
    display_error("Argument parsing failed\n");
    return;
  }

  if (has_over) {
    display_error("Sorry, but step over is not supported yet\n");
  } else {
    for (unsigned i = 0; i < step_count && !mp.is_finished(); ++i) {
      mp.step();
    }
  }
  if (mp.is_finished()) {
    display_info("Metaprogram finished\n");
  } else {
    display_current_frame();
  }
}

void metadebugger_shell::command_evaluate(const std::string& arg) {
  if (arg.empty()) {
    display_error("Argument expected\n");
    return;
  }
  run_metaprogram_with_templight(arg);
}

void metadebugger_shell::command_forwardtrace(const std::string& arg) {
  if (!require_empty_args(arg) || !require_running_metaprogram()) {
    return;
  }
  display_current_forwardtrace();
}

void metadebugger_shell::command_backtrace(const std::string& arg) {
  if (!require_empty_args(arg) || !require_running_metaprogram()) {
    return;
  }
  display_backtrace();
}

void metadebugger_shell::command_break(const std::string& arg) {
  breakpoints.push_back(arg);
  display_info("Break point \"" + arg + "\" added\n");
}

void metadebugger_shell::command_help(const std::string& arg) {
  if (arg.empty()) {
    display_info(
        "List of available commands:\n\n");
    for (const metadebugger_command& cmd : command_handler.get_commands()) {
      display_info(
          cmd.get_keys().front() + " -- " +
          cmd.get_short_description() + "\n");
    }
    display_info(
        "\n"
        "Type \"help\" followed by a command name for more information.\n"
        "Command name abbreviations are allowed if unambiguous.\n");
    return;
  }

  auto command_arg_pair = command_handler.get_command_for_line(arg);
  if (!command_arg_pair) {
    display_error("Command not found\n");
    return;
  }

  using boost::algorithm::join;

  metadebugger_command cmd;
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

void metadebugger_shell::command_quit(const std::string& arg) {
  if (!require_empty_args(arg)) {
    return;
  }
  is_stopped = true;
}

void metadebugger_shell::run_metaprogram_with_templight(
    const std::string& str)
{
  temporary_file templight_xml_file("templight-%%%%-%%%%-%%%%-%%%%.xml");
  const std::string& xml_path = templight_xml_file.get_path().string();

  env.set_xml_location(xml_path);

  run_metaprogram(str);

  mp = metaprogram::create_from_xml_file(xml_path);
}

void metadebugger_shell::run_metaprogram(const std::string& str) {
  result res = eval_tmp_unformatted(env, str, conf, "<mdb-stdin>");

  if (!res.info.empty()) {
    display_info(res.info);
  }

  for (const std::string& e : res.errors) {
    display_error(e + "\n");
  }
  if (!res.has_errors()) {
    display(highlight_syntax(res.output) + "\n");
  }
}

void metadebugger_shell::continue_metaprogram() {
  assert(!mp.is_finished());

  while (true) {
    mp.step();
    if (mp.is_finished()) {
      display("Metaprogram finished\n");
      return;
    }
    for (const breakpoint_t& breakpoint : breakpoints) {
      const std::string current_type =
        mp.get_vertex_property(mp.get_current_frame().vertex).name;

      boost::regex break_regex(breakpoint);
      if (boost::regex_search(current_type, break_regex)) {
        display("Breakpoint reached\n");
        return;
      }
    }
  }
}

void metadebugger_shell::display_error(const std::string& str) const {
  display(colored_string(str, color::bright_red));
}

void metadebugger_shell::display_info(const std::string& str) const {
  display(str);
}

void metadebugger_shell::display_current_frame() const {
  if (mp.get_current_vertex() == mp.get_root_vertex()) {
    // The MP hasn't been stepped at least once => no frame available
    return;
  }
  display_frame(mp.get_current_frame());
}

void metadebugger_shell::display_trace_graph(
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

void metadebugger_shell::display_trace_line(
    metaprogram::vertex_descriptor vertex,
    unsigned depth,
    const std::vector<unsigned>& depth_counter,
    const boost::optional<instantiation_kind>& kind,
    unsigned width) const
{

  colored_string element_content =
    highlight_syntax(mp.get_vertex_property(vertex).name);

  if (kind) {
    element_content += " (" + to_string(*kind) + ")";
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

// Visits a single vertex and all of its children
void metadebugger_shell::display_trace_visit(
    metaprogram::vertex_descriptor root_vertex,
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

  const metaprogram::graph_t& graph = mp.get_graph();

  std::vector<unsigned> depth_counter(1);

  typedef std::tuple<
    metaprogram::vertex_descriptor,
    unsigned, // Depth
    boost::optional<instantiation_kind> > stack_element;

  // The usual stack for DFS
  std::stack<stack_element> to_visit;

  // We don't care about the instantiation_kind for the source vertex
  to_visit.push(std::make_tuple(root_vertex, 0, boost::none));
  ++depth_counter[0]; // This value is neved read

  while (!to_visit.empty()) {
    unsigned depth;
    metaprogram::vertex_descriptor vertex;
    boost::optional<instantiation_kind> kind;
    std::tie(vertex, depth, kind) = to_visit.top();
    to_visit.pop();

    --depth_counter[depth];

    display_trace_line(vertex, depth, depth_counter, kind, width);

    if (discovered[vertex]) {
      continue;
    }
    discovered[vertex] = true;

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
      instantiation_kind next_kind = mp.get_edge_property(edge).kind;

      to_visit.push(
        std::make_tuple(boost::target(edge, graph), depth+1, next_kind));

      ++depth_counter[depth+1];
    }
  }
}

void metadebugger_shell::display_current_forwardtrace() const {
  metaprogram::discovered_t discovered = mp.get_state().discovered;

  display_trace_visit(mp.get_current_vertex(), discovered, width());
}

void metadebugger_shell::display_frame(const metaprogram::frame_t& frame) const {
  // No kind for <root> vertex
  if (frame.vertex == mp.get_root_vertex()) {
    display(mp.get_vertex_property(frame.vertex).name + "\n");
  } else {
    display(
        highlight_syntax(mp.get_vertex_property(frame.vertex).name) +
        " (" + to_string(mp.get_edge_property(frame.parent_edge).kind) + ")\n"
    );
  }
}

void metadebugger_shell::display_backtrace() const {
  const metaprogram::backtrace_t& backtrace = mp.get_backtrace();

  unsigned i = 0;
  for (const metaprogram::frame_t& frame :
      backtrace | boost::adaptors::reversed)
  {
    display("#" + std::to_string(i) + " ");
    display_frame(frame);
    ++i;
  }
}

}

