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

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/metaprogram.hpp>

namespace metashell
{

  class metaprogram_builder
  {
  public:
    metaprogram_builder(data::metaprogram::mode_t mode,
                        const data::cpp_code& root_name,
                        const data::file_location& root_source_location);

    void handle_template_begin(data::event_kind kind,
                               const data::type& type,
                               const data::file_location& point_of_event,
                               const data::file_location& source_location,
                               double timestamp);

    void handle_template_end(double timestamp);

    void handle_evaluation_end(data::type_or_code_or_error result_);

    const data::metaprogram& get_metaprogram() const;

  private:
    typedef data::metaprogram::vertex_descriptor vertex_descriptor;
    typedef data::metaprogram::edge_descriptor edge_descriptor;

    typedef std::tuple<data::metaprogram_node, data::file_location>
        element_key_t;
    typedef std::map<element_key_t, vertex_descriptor> element_vertex_map_t;

    vertex_descriptor add_vertex(const data::metaprogram_node& node,
                                 const data::file_location& source_location);

    data::metaprogram mp;

    std::stack<edge_descriptor> edge_stack;

    element_vertex_map_t element_vertex_map;
  };
}

#endif
