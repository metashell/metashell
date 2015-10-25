#ifndef METASHELL_METAPROGRAM_BUILDER_HPP
#define METASHELL_METAPROGRAM_BUILDER_HPP

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
#include <string>

#include <metashell/metaprogram.hpp>

namespace metashell {

class metaprogram_builder {
public:

  metaprogram_builder(
      metaprogram::mode_t mode,
      const std::string& root_name,
      const data::file_location& root_source_location,
      const data::type_or_error& evaluation_result);

  void handle_template_begin(
    data::instantiation_kind kind,
    const std::string& name,
    const data::file_location& point_of_instantiation,
    const data::file_location& source_location,
    double timestamp);

  void handle_template_end(double timestamp);

  const metaprogram& get_metaprogram() const;

private:
  typedef metaprogram::vertex_descriptor vertex_descriptor;
  typedef metaprogram::edge_descriptor edge_descriptor;

  typedef std::tuple<std::string, data::file_location> element_key_t;
  typedef std::map<element_key_t, vertex_descriptor> element_vertex_map_t;

  vertex_descriptor add_vertex(
    const std::string& name,
    const data::file_location& source_location);

  metaprogram mp;

  std::stack<edge_descriptor> edge_stack;

  element_vertex_map_t element_vertex_map;
};

}

#endif
