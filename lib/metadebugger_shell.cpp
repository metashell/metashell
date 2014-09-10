
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
#include <metashell/metashell.hpp>
#include <metashell/temporary_file.hpp>

#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <boost/assign.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/home/phoenix/operator.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/range/adaptor/reversed.hpp>

// a min macro is defined in minwindef.h (Visual C++ 2013)
#ifdef min
#  undef min
#endif

// a max macro is defined in minwindef.h (Visual C++ 2013)
#ifdef max
#  undef max
#endif

namespace metashell {

const std::vector<just::console::color> metadebugger_shell::colors =
  {
    just::console::color::red,
    just::console::color::green,
    just::console::color::yellow,
    just::console::color::blue,
    just::console::color::magenta,
    just::console::color::cyan
  };

metadebugger_command_handler_map::command_map_t
  metadebugger_shell::create_default_command_map() {

  metadebugger_command_handler_map::command_map_t res =
    {
      {"continue", repeatable, &metadebugger_shell::command_continue,
        "Continue program being debugged, until breakpoint or end of program."},
      {"step", repeatable, &metadebugger_shell::command_step,
        "Step the program one instantiation. Usage: step [over] [count]"},
      {"evaluate", non_repeatable, &metadebugger_shell::command_evaluate,
        "Evaluate and start debugging new metaprogram."},
      {"forwardtrace", non_repeatable, &metadebugger_shell::command_forwardtrace,
        "Print forwardtrace from the current point."},
      {"ft", non_repeatable, &metadebugger_shell::command_forwardtrace,
        "Alias for forwardtrace."},
      {"backtrace", non_repeatable, &metadebugger_shell::command_backtrace,
        "Print backtrace from the current point."},
      {"bt", non_repeatable, &metadebugger_shell::command_backtrace,
        "Alias for backtrace."},
      {"break", non_repeatable, &metadebugger_shell::command_break,
        "Add new breakpoint. Usage: break [breakpoint]"},
      {"help", non_repeatable, &metadebugger_shell::command_help,
        "Show help for commands. Usage: help [command]"}
    };
  return res;
}

metadebugger_shell::metadebugger_shell(
    const config& conf,
    environment& env) :
  conf(conf),
  env(env),
  command_handler(create_default_command_map()),
  mp(metaprogram::create_empty_finished()),
  is_stopped(false)
{}

metadebugger_shell::~metadebugger_shell() {}

std::string metadebugger_shell::prompt() const {
  return "(mdb) ";
}

bool metadebugger_shell::stopped() const {
  return is_stopped;
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
  auto command_arg_pair = command_handler.get_command_for_line(arg);
  if (!command_arg_pair) {
    display_error("Command not found\n");
    return;
  }

  metadebugger_command cmd;
  std::string command_args;
  std::tie(cmd, command_args) = *command_arg_pair;

  if (!command_args.empty()) {
    display_error("Only one argument expected\n");
    return;
  }

  display_info(cmd.get_key() + ": " + cmd.get_description() + "\n");
}

void metadebugger_shell::run_metaprogram_with_templight(
    const std::string& str)
{
  temporary_file templight_xml_file("templight-%%%%-%%%%-%%%%-%%%%.xml");

  std::vector<std::string>& clang_args = env.clang_arguments();

  clang_args.push_back("-templight");
  clang_args.push_back("-templight-output");
  clang_args.push_back(templight_xml_file.get_path().string());
  clang_args.push_back("-templight-format");
  clang_args.push_back("xml");

  run_metaprogram(str);

  //TODO move this to a destructor. run_metaprogram might throw
  clang_args.erase(clang_args.end() - 5, clang_args.end());

  mp = metaprogram::create_from_xml_file(
      templight_xml_file.get_path().string());
}

void metadebugger_shell::run_metaprogram(const std::string& str) {
  result res = eval_tmp(env, str, conf, "<mdb-stdin>");

  if (!res.info.empty()) {
    display_info(res.info);
  }

  for (const std::string& e : res.errors) {
    display_error(e + "\n");
  }
  if (!res.has_errors()) {
    display_info(res.output + "\n"); //TODO syntax hightlighted
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
  display(str, just::console::color::bright_red);
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

metadebugger_shell::string_range metadebugger_shell::find_type_emphasize(
    const std::string& type) const
{

  #define MSH_R_NAMESPACE_OR_TYPE       \
    "("                                 \
      "([_a-zA-Z][_a-zA-Z0-9]*)"    "|" \
      "(\\(anonymous namespace\\))" "|" \
      "(<anonymous>)"               "|" \
      "(<anonymous struct>)"        "|" \
      "(<anonymous class>)"         "|" \
      "(<anonymous union>)"             \
    ")"

  boost::regex reg(
      "^(::)?(" MSH_R_NAMESPACE_OR_TYPE "::)*" MSH_R_NAMESPACE_OR_TYPE);

  #undef MSH_R_NAMESPACE_OR_TYPE

  boost::smatch match;
  if (!boost::regex_search(type.begin(), type.end(), match, reg)) {
    return string_range(type.end(), type.end());
  }

  return string_range(match[10].first, match[10].second);
}

void metadebugger_shell::display_range(
    std::string::const_iterator begin,
    std::string::const_iterator end,
    optional_color c) const
{
  if (begin < end) {
    display(std::string(begin, end), c);
  }
}

void metadebugger_shell::display_trace_content(
    string_range range,
    string_range emphasize) const
{
  assert(range.first <= range.second);
  assert(emphasize.first <= emphasize.second);

  //TODO avoid copying

  display_range(
      range.first,
      std::min(range.second, emphasize.first),
      boost::none);

  display_range(
      std::max(range.first, emphasize.first),
      std::min(range.second, emphasize.second),
      just::console::color::white);

  display_range(
      std::max(emphasize.second, range.first),
      range.second,
      boost::none);
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
      display(
          depth_counter[i] > 0 ? "| " : "  ",
          colors[i % colors.size()]);
    }

    just::console::color mark_color = colors[depth % colors.size()];
    if (print_mark) {
      if (depth_counter[depth] == 0) {
        display("` ", mark_color);
      } else {
        display("+ ", mark_color);
      }
    } else if (depth_counter[depth] > 0) {
      display("| ", mark_color);
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

  const std::string type = mp.get_vertex_property(vertex).name;

  std::stringstream element_content_ss;
  element_content_ss << type;

  if (kind) {
    element_content_ss << " (" << *kind << ")";
  }

  std::string element_content = element_content_ss.str();

  string_range emphasize = find_type_emphasize(type);

  // Realign the iterators from 'type' to 'element_content'
  emphasize.first = element_content.begin() + (emphasize.first - type.begin());
  emphasize.second = element_content.begin() + (emphasize.second - type.begin());

  unsigned non_content_length = 2*depth;

  if (width < 10 || non_content_length >= width - 10) {
    // We have no chance to display the graph nicely :(
    display_trace_graph(depth, depth_counter, true);

    display_trace_content(
      string_range(element_content.begin(), element_content.end()),
      emphasize);
    display("\n");
  } else {
    unsigned content_width = width - non_content_length;
    for (unsigned i = 0; i < element_content.size(); i += content_width) {
      display_trace_graph(depth, depth_counter, i == 0);
      display_trace_content(
        string_range(
          element_content.begin() + i,
          i + content_width < element_content.size() ?
            element_content.begin() + (i + content_width) :
            element_content.end()
        ),
        emphasize
      );
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
    display((boost::format("%1% (%2%)\n") %
          mp.get_vertex_property(frame.vertex).name %
          mp.get_edge_property(frame.parent_edge).kind).str());
  }
}

void metadebugger_shell::display_backtrace() const {
  const metaprogram::backtrace_t& backtrace = mp.get_backtrace();

  unsigned i = 0;
  for (const metaprogram::frame_t& frame :
      backtrace | boost::adaptors::reversed)
  {
    display((boost::format("#%1% ") % i).str());
    display_frame(frame);
    ++i;
  }
}

}

