#ifndef METASHELL_TEMPLIGHT_TRACE_HPP
#define METASHELL_TEMPLIGHT_TRACE_HPP

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

#include <stack>
#include <tuple>
#include <string>
#include <vector>
#include <iostream>

#include <boost/optional.hpp>

#include <metashell/metaprogram.hpp>

#include <just/console.hpp>

namespace metashell {

//Fwd:
class metadebugger_shell;

class templight_trace {
public:

  void print_graph(std::ostream& os = std::cout) const;
  void print_graphviz(std::ostream& os = std::cout) const;

  void set_metaprogram(const metaprogram& new_mp);
  metaprogram& get_metaprogram();
  const metaprogram& get_metaprogram() const;

private:

  typedef metaprogram::edge_descriptor edge_descriptor;
  typedef metaprogram::vertex_descriptor vertex_descriptor;
  typedef metaprogram::const_vertex_property_map_t const_vertex_property_map_t;
  typedef metaprogram::const_edge_property_map_t const_edge_property_map_t;
  typedef metaprogram::template_vertex_property template_vertex_property;
  typedef metaprogram::template_vertex_property_tag template_vertex_property_tag;
  typedef metaprogram::template_edge_property_tag template_edge_property_tag;

  metaprogram mp;
};

std::ostream& operator<<(std::ostream& os, const templight_trace& trace);

}

#endif
