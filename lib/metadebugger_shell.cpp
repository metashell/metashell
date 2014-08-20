
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

#include <metashell/metashell.hpp>

#include <metashell/metadebugger_shell.hpp>

#include <metashell/temporary_file.hpp>

#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <boost/assign.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/reversed.hpp>

namespace metashell {

const std::vector<just::console::color> metadebugger_shell::colors =
  boost::assign::list_of
    (just::console::color::red)
    (just::console::color::green)
    (just::console::color::yellow)
    (just::console::color::blue)
    (just::console::color::magenta)
    (just::console::color::cyan);

metadebugger_shell::metadebugger_shell(
    const config& conf,
    environment& env) :
  conf(conf),
  env(env),
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
    if (!prev_line.empty()) {
      line = prev_line;
    } else {
      return;
    }
  } else if (line != prev_line) {
    add_history(line);
    prev_line = line;
  }
  if (line == "ft" || line == "forwardtrace") {
    if (require_running_metaprogram()) {
      display_current_forward_trace();
    }
  } else if (line == "bt" || line == "backtrace") {
    if (require_running_metaprogram()) {
      display_back_trace();
    }
  } else if (boost::starts_with(line, "eval ")) {
    run_metaprogram_with_templight(
        line.substr(5, std::string::npos));
  } else if (line == "step") {
    if (require_running_metaprogram()) {
        if (!mp.step_metaprogram()) {
          display_current_frame();
        } else {
          display("Metaprogram finished\n",
              just::console::color::green);
        }
    }
  } else if (boost::starts_with(line, "break ")) {
    breakpoint_t breakpoint = line.substr(6, std::string::npos);
    display("Break point \"" + breakpoint + "\" added\n");
    breakpoints.push_back(breakpoint);
  } else if (line == "continue") {
    if (require_running_metaprogram()) {
      continue_metaprogram();
    }
  } else {
    display("Unknown command: \"" + line + "\"\n",
        just::console::color::red);
  }
}

bool metadebugger_shell::require_running_metaprogram() const {
  if (mp.is_metaprogram_finished()) {
    display("Metaprogram finished\n", just::console::color::red);
    return false;
  }
  return true;
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
    display(res.info);
  }

  for (const std::string& e : res.errors) {
    display(e + "\n", just::console::color::red);
  }
  if (!res.has_errors()) {
    display(res.output + "\n");
  }
}

void metadebugger_shell::continue_metaprogram() {
  //TODO check if metaprogram started

  while (true) {
    if (mp.step_metaprogram()) {
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

void metadebugger_shell::display_current_frame() const {
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
      display("+ ", mark_color);
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

  if (discovered[root_vertex]) {
    return;
  }
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

    if (!discovered[vertex]) {
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
}

void metadebugger_shell::display_forward_trace(
    const std::string& root_type) const
{
  boost::optional<metaprogram::vertex_descriptor> opt_vertex =
    mp.find_vertex(root_type);

  if (!opt_vertex) {
    display("Type \"" + root_type + "\" not found", just::console::color::red);
    return;
  }

  unsigned shell_width = width();

  metaprogram::discovered_t discovered(mp.get_num_vertices());

  display_trace_visit(*opt_vertex, discovered, shell_width);
}

void metadebugger_shell::display_current_forward_trace() const {
  metaprogram::discovered_t discovered = mp.get_state().discovered;

  metaprogram::vertex_descriptor vertex = mp.get_current_frame().vertex;
  discovered[vertex] = false;

  unsigned shell_width = width();

  display_trace_visit(vertex, discovered, shell_width);
}

void metadebugger_shell::display_frame(const metaprogram::frame& frame) const {
  // No kind for <root> vertex
  if (frame.vertex == mp.get_root_vertex()) {
    display(mp.get_vertex_property(frame.vertex).name + "\n");
  } else {
    display((boost::format("%1% (%2%)\n") %
          mp.get_vertex_property(frame.vertex).name %
          mp.get_edge_property(frame.parent_edge).kind).str());
  }
}

void metadebugger_shell::display_back_trace() const {
  const metaprogram::back_trace_t& back_trace = mp.get_back_trace();

  unsigned i = 0;
  for (const metaprogram::frame& frame :
      back_trace | boost::adaptors::reversed)
  {
    display((boost::format("#%1% ") % i).str());
    display_frame(frame);
    ++i;
  }
}

}

