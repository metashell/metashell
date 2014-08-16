
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

#include <map>
#include <deque>
#include <utility>
#include <iostream>
#include <cassert>
#include <algorithm>

#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/graph/graphviz.hpp>

#include <metashell/templight_trace.hpp>
#include <metashell/metadebugger_shell.hpp>

namespace metashell {

const std::vector<just::console::color> templight_trace::colors =
  boost::assign::list_of
    (just::console::color::red)
    (just::console::color::green)
    (just::console::color::yellow)
    (just::console::color::blue)
    (just::console::color::magenta)
    (just::console::color::cyan);



void templight_trace::print_graph(std::ostream& os) const {

  const_vertex_property_map_t vertex_map =
      boost::get(template_vertex_property_tag(), mp.get_graph());

  const_edge_property_map_t edge_map =
      boost::get(template_edge_property_tag(), mp.get_graph());

  os << "Verticies:\n";
  BOOST_FOREACH(vertex_descriptor vertex, boost::vertices(mp.get_graph())) {
    const template_vertex_property& vertex_property =
      boost::get(vertex_map, vertex);

    os << vertex << " : " <<
      vertex_property.name <<
      " instantiated from " <<
      vertex_property.point_of_instantiation << '\n';
  }

  os << "Edges:\n";
  BOOST_FOREACH(const edge_descriptor& edge, boost::edges(mp.get_graph())) {
    os << boost::get(vertex_map, source(edge, mp.get_graph())).name <<
      " ---" << boost::get(edge_map, edge).kind << "---> " <<
      boost::get(vertex_map, target(edge, mp.get_graph())).name << '\n';
  }
}

void templight_trace::print_graphviz(std::ostream& os) const {

  boost::write_graphviz(
      os, mp.get_graph(),
      [this](std::ostream& os, vertex_descriptor vertex) {
        os << "[label=\"" <<
          boost::get(template_vertex_property_tag(), mp.get_graph(), vertex).name <<
          "\"]";
      },
      [this](std::ostream& os, edge_descriptor edge) {
        os << "[label=\""<<
          boost::get(template_edge_property_tag(), mp.get_graph(), edge).kind <<
          "\"]";
      }
  );
}

templight_trace::string_range templight_trace::find_type_emphasize(
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

void templight_trace::print_trace_graph(
    const metadebugger_shell& sh,
    unsigned depth,
    const std::vector<unsigned>& depth_counter,
    bool print_mark) const
{
  assert(depth_counter.size() > depth);

  if (depth > 0) {
    //TODO respect the -H (no syntax highlight parameter)
    for (unsigned i = 1; i < depth; ++i) {
      sh.display(
          depth_counter[i] > 0 ? "| " : "  ",
          colors[i % colors.size()]);
    }

    just::console::color mark_color = colors[depth % colors.size()];
    if (print_mark) {
      sh.display("+ ", mark_color);
    } else if (depth_counter[depth] > 0) {
      sh.display("| ", mark_color);
    } else {
      sh.display("  ");
    }
  }
}

namespace {

void print_range(
    const metadebugger_shell& sh,
    std::string::const_iterator begin,
    std::string::const_iterator end,
    metadebugger_shell::optional_color c)
{
  if (begin < end) {
    sh.display(std::string(begin, end), c);
  }
}

}

void templight_trace::print_trace_content(
    const metadebugger_shell& sh,
    string_range range,
    string_range emphasize) const
{
  assert(range.first <= range.second);
  assert(emphasize.first <= emphasize.second);

  //TODO avoid copying

  print_range(
      sh,
      range.first,
      std::min(range.second, emphasize.first),
      boost::none);

  print_range(
      sh,
      std::max(range.first, emphasize.first),
      std::min(range.second, emphasize.second),
      just::console::color::white);

  print_range(
      sh,
      std::max(emphasize.second, range.first),
      range.second,
      boost::none);
}

void templight_trace::print_trace_line(
    const metadebugger_shell& sh,
    vertex_descriptor vertex,
    unsigned depth,
    const std::vector<unsigned>& depth_counter,
    const boost::optional<instantiation_kind>& kind,
    unsigned width) const
{

  const std::string type =
    boost::get(template_vertex_property_tag(), mp.get_graph(), vertex).name;

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
    print_trace_graph(sh, depth, depth_counter, true);

    print_trace_content(
      sh,
      string_range(element_content.begin(), element_content.end()),
      emphasize);
    sh.display("\n");
  } else {
    unsigned content_width = width - non_content_length;
    for (unsigned i = 0; i < element_content.size(); i += content_width) {
      print_trace_graph(sh, depth, depth_counter, i == 0);
      print_trace_content(
        sh,
        string_range(
          element_content.begin() + i,
          i + content_width < element_content.size() ?
            element_content.begin() + (i + content_width) :
            element_content.end()
        ),
        emphasize
      );
      sh.display("\n");
    }
  }
}

// Visits a single vertex and all of its children
template<class EdgeIterator, class GetEdges, class EdgeDirection>
void templight_trace::print_trace_visit(
    const metadebugger_shell& sh,
    vertex_descriptor root_vertex,
    discovered_t& discovered,
    GetEdges get_edges, EdgeDirection edge_direction,
    unsigned width) const
{

  assert(discovered.size() == boost::num_vertices(mp.get_graph()));

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
    vertex_descriptor,
    unsigned, // Depth
    boost::optional<instantiation_kind> > stack_element;

  // The usual stack for DFS
  std::stack<stack_element> to_visit;

  // We don't care about the instantiation_kind for the source vertex
  to_visit.push(std::make_tuple(root_vertex, 0, boost::none));
  ++depth_counter[0]; // This value is neved read

  while (!to_visit.empty()) {
    unsigned depth;
    vertex_descriptor vertex;
    boost::optional<instantiation_kind> kind;
    std::tie(vertex, depth, kind) = to_visit.top();
    to_visit.pop();

    --depth_counter[depth];

    print_trace_line(sh, vertex, depth, depth_counter, kind, width);

    if (!discovered[vertex]) {
      discovered[vertex] = true;

      EdgeIterator begin, end;
      std::tie(begin, end) = get_edges(vertex);

      typedef std::vector<edge_descriptor> edges_t;
      edges_t edges(begin, end);

      if (depth_counter.size() <= depth+1) {
        depth_counter.resize(depth+1+1);
      }

      // Reverse iteration, so types that got instantiated first
      // get on the top of the stack
      BOOST_REVERSE_FOREACH(const edge_descriptor& edge, edges) {
        instantiation_kind next_kind =
          boost::get(template_edge_property_tag(), mp.get_graph(), edge).kind;

        to_visit.push(
          std::make_tuple(edge_direction(edge), depth+1, next_kind));

        ++depth_counter[depth+1];
      }
    }
  }
}

void templight_trace::print_forwardtrace(
    const metadebugger_shell& sh,
    const std::string& type) const
{

  boost::optional<vertex_descriptor> opt_vertex =
    mp.find_vertex(type);

  if (!opt_vertex) {
    sh.display("type \"" + type + "\" not found", just::console::color::red);
    return;
  }

  unsigned width = sh.width();

  discovered_t discovered(boost::num_vertices(mp.get_graph()));

  print_trace_visit<out_edge_iterator>(
      sh,
      *opt_vertex,
      discovered,
      [this](vertex_descriptor v) { return boost::out_edges(v, mp.get_graph()); },
      [this](edge_descriptor e) { return boost::target(e, mp.get_graph()); },
      width);
}

void templight_trace::print_full_forwardtrace(const metadebugger_shell& sh) const {

  assert(boost::num_vertices(mp.get_graph()) > 0);

  unsigned width = sh.width();

  discovered_t discovered(boost::num_vertices(mp.get_graph()));

  print_trace_visit<out_edge_iterator>(
      sh,
      // 0 is always the <root> vertex, and every vertex is reachable from root
      0,
      discovered,
      [this](vertex_descriptor v) { return boost::out_edges(v, mp.get_graph()); },
      [this](edge_descriptor e) { return boost::target(e, mp.get_graph()); },
      width);
}

void templight_trace::print_backtrace(
    const metadebugger_shell& sh,
    const std::string& type) const
{

  boost::optional<vertex_descriptor> opt_vertex =
    mp.find_vertex(type);

  if (!opt_vertex) {
    sh.display("type \"" + type + "\" not found", just::console::color::red);
    return;
  }

  unsigned width = sh.width();

  discovered_t discovered(boost::num_vertices(mp.get_graph()));

  print_trace_visit<in_edge_iterator>(
      sh,
      *opt_vertex,
      discovered,
      [this](vertex_descriptor v) { return boost::in_edges(v, mp.get_graph()); },
      [this](edge_descriptor e) { return boost::source(e, mp.get_graph()); },
      width);
}

struct templight_trace::only_has_discovered_out_edge_predicate {
  only_has_discovered_out_edge_predicate(
    const metaprogram::graph_t& graph,
    const discovered_t& discovered) :
      graph(graph), discovered(discovered) {}

  bool operator()(vertex_descriptor vertex) const {
    if (discovered[vertex]) {
      return false;
    }
    BOOST_FOREACH(edge_descriptor edge, boost::out_edges(vertex, graph)) {
      if (!discovered[boost::target(edge, graph)]) {
        return false;
      }
    }
    return true;
  }
private:
  const metaprogram::graph_t& graph;
  const discovered_t& discovered;
};

void templight_trace::print_full_backtrace(const metadebugger_shell& sh) const {

  assert(boost::num_vertices(mp.get_graph()) > 0);

  discovered_t discovered(boost::num_vertices(mp.get_graph()));

  unsigned width = sh.width();

  // TODO this needs some more work:
  // -try to go with the deepest route first
  // -try to find a natural way to produce bt
  // -o(V^2) algorithm
  while (true) {
    // Since the graph is DAG, there is always a vertex which
    // has only discovered out_edges
    vertex_iterator begin, end, it;
    std::tie(begin, end) = boost::vertices(mp.get_graph());

    it = std::find_if(begin, end,
        only_has_discovered_out_edge_predicate(mp.get_graph(), discovered));

    if (it == end) {
      break;
    }
    print_trace_visit<in_edge_iterator>(
        sh,
        *it,
        discovered,
        [this](vertex_descriptor v) { return boost::in_edges(v, mp.get_graph()); },
        [this](edge_descriptor e) { return boost::source(e, mp.get_graph()); },
        width);
  }
}

void templight_trace::print_current_frame(const metadebugger_shell& sh) const {
  if (mp.get_state().vertex_stack.empty()) {
    sh.display("Stack is empty\n", just::console::color::red);
    return;
  }
  vertex_descriptor current_vertex;
  instantiation_kind kind;
  std::tie(current_vertex, kind) = mp.get_state().vertex_stack.top();

  // No kind for <root> vertex
  if (current_vertex == 0) {
    sh.display(boost::get(
        template_vertex_property_tag(),
        mp.get_graph(),
        current_vertex).name + "\n");
  } else {
    sh.display((
        boost::format("%1% (%2%)\n") %
          boost::get(
            template_vertex_property_tag(), mp.get_graph(), current_vertex).name %
          kind
        ).str());
  }
}

void templight_trace::set_metaprogram(const metaprogram& new_mp) {
  mp = new_mp;
}

metaprogram& templight_trace::get_metaprogram() {
  return mp;
}

const metaprogram& templight_trace::get_metaprogram() const {
  return mp;
}

std::ostream& operator<<(std::ostream& os, const templight_trace& trace) {
  trace.print_graph(os);
  return os;
}

}

