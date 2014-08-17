
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
    display_forward_trace("<root>");
  } else if (boost::starts_with(line, "eval ")) {
    run_metaprogram_with_templight(
        line.substr(5, std::string::npos));
  } else if (line == "start") {
    //TODO ask user if he wants to restart
    mp.start_metaprogram();
  } else if (line == "step") {
    if (mp.is_metaprogram_started()) {
        if (!mp.step_metaprogram()) {
          display_current_frame();
        } else {
          display("Metaprogram finished\n",
              just::console::color::green);
        }
    } else {
      display("Metaprogram not started yet\n",
          just::console::color::red);
    }
  } else {
    display("Unknown command: \"" + line + "\"\n",
        just::console::color::red);
  }

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

void metadebugger_shell::display_current_frame() const {
  metaprogram::frame frame = mp.get_current_frame();
  // No kind for <root> vertex
  if (frame.vertex == 0) {
    display(frame.type_name + "\n");
  } else {
    display((boost::format("%1% (%2%)\n") % frame.type_name % frame.kind).str());
  }
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

void metadebugger_shell::print_range(
    std::string::const_iterator begin,
    std::string::const_iterator end,
    optional_color c) const
{
  if (begin < end) {
    display(std::string(begin, end), c);
  }
}

void metadebugger_shell::print_trace_content(
    string_range range,
    string_range emphasize) const
{
  assert(range.first <= range.second);
  assert(emphasize.first <= emphasize.second);

  //TODO avoid copying

  print_range(
      range.first,
      std::min(range.second, emphasize.first),
      boost::none);

  print_range(
      std::max(range.first, emphasize.first),
      std::min(range.second, emphasize.second),
      just::console::color::white);

  print_range(
      std::max(emphasize.second, range.first),
      range.second,
      boost::none);
}

void metadebugger_shell::print_trace_graph(
    const metaprogram::graph_t& graph,
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

void metadebugger_shell::print_trace_line(
    const metaprogram::graph_t& graph,
    metaprogram::vertex_descriptor vertex,
    unsigned depth,
    const std::vector<unsigned>& depth_counter,
    const boost::optional<instantiation_kind>& kind,
    unsigned width) const
{

  const std::string type =
    boost::get(metaprogram::template_vertex_property_tag(), graph, vertex).name;

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
    print_trace_graph(graph, depth, depth_counter, true);

    print_trace_content(
      string_range(element_content.begin(), element_content.end()),
      emphasize);
    display("\n");
  } else {
    unsigned content_width = width - non_content_length;
    for (unsigned i = 0; i < element_content.size(); i += content_width) {
      print_trace_graph(graph, depth, depth_counter, i == 0);
      print_trace_content(
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
void metadebugger_shell::print_trace_visit(
    const metaprogram::graph_t& graph,
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

    print_trace_line(graph, vertex, depth, depth_counter, kind, width);

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
        instantiation_kind next_kind =
          boost::get(metaprogram::template_edge_property_tag(),
              graph, edge).kind;

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
  const metaprogram::graph_t& graph = mp.get_graph();

  boost::optional<metaprogram::vertex_descriptor> opt_vertex =
    mp.find_vertex(root_type);

  if (!opt_vertex) {
    display("Type \"" + root_type + "\" not found", just::console::color::red);
    return;
  }

  unsigned shell_width = width();

  metaprogram::discovered_t discovered(boost::num_vertices(graph));

  print_trace_visit(graph, *opt_vertex, discovered, shell_width);
}

}

