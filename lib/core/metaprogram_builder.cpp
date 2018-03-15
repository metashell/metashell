
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

#include <metashell/metaprogram_builder.hpp>

#include <metashell/exception.hpp>

namespace metashell
{
  metaprogram_builder::metaprogram_builder(
      data::metaprogram::mode_t mode,
      const data::cpp_code& root_name,
      const data::file_location& root_source_location)
    : mp(mode,
         root_name,
         root_source_location,
         data::type_or_code_or_error(
             "Internal Metashell error: metaprogram not finished yet"))
  {
  }

  void metaprogram_builder::handle_event(const data::event_data& details)
  {
    mpark::visit(
        [this](const auto& det) { this->handle_event_impl(det); }, details);
  }

  const data::metaprogram& metaprogram_builder::get_metaprogram() const
  {
    if (!edge_stack.empty())
    {
      throw exception("Some Templight TemplateEnd events are missing");
    }
    return mp;
  }

  metaprogram_builder::vertex_descriptor
  metaprogram_builder::add_vertex(const data::metaprogram_node& node,
                                  const data::file_location& source_location)
  {
    element_vertex_map_t::iterator pos;
    bool inserted;

    std::tie(pos, inserted) = element_vertex_map.insert(std::make_pair(
        std::make_tuple(node, source_location), vertex_descriptor()));

    if (inserted)
    {
      pos->second = mp.add_vertex(node, source_location);
    }
    return pos->second;
  }

  void metaprogram_builder::pop_edge(data::event_kind end_kind,
                                     double timestamp)
  {
    if (edge_stack.empty())
    {
      throw exception("Mismatched begin and " + to_string(end_kind) +
                      " events");
    }
    auto& ep = mp.get_edge_property(edge_stack.top());
    ep.time_taken = timestamp - ep.begin_timestamp;

    edge_stack.pop();
  }
}
