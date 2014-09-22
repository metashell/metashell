
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
#include <string>
#include <sstream>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <metashell/metaprogram.hpp>

#include "exception.hpp"

namespace metashell {

struct metaprogram_builder {

  metaprogram_builder();

  void handle_template_begin(
    instantiation_kind kind,
    const std::string& context,
    const file_location& location,
    double timestamp,
    unsigned long long memory_usage);

  void handle_template_end(
    instantiation_kind kind,
    double timestamp,
    unsigned long long memory_usage);

  const metaprogram& get_metaprogram() const;

private:
  typedef metaprogram::vertex_descriptor vertex_descriptor;
  typedef std::map<std::string, vertex_descriptor> element_vertex_map_t;

  vertex_descriptor add_vertex(const std::string& context);

  metaprogram trace;

  std::stack<vertex_descriptor> vertex_stack;

  element_vertex_map_t element_vertex_map;
};

metaprogram_builder::metaprogram_builder() {
  // Add root vertex
  vertex_stack.push(trace.get_root_vertex());
}

void metaprogram_builder::handle_template_begin(
  instantiation_kind kind,
  const std::string& context,
  const file_location& point_of_instantiation,
  double /* timestamp */,
  unsigned long long /* memory_usage */)
{
  vertex_descriptor vertex = add_vertex(context);
  if (!vertex_stack.empty()) {
    vertex_descriptor top_vertex = vertex_stack.top();
    trace.add_edge(top_vertex, vertex, kind, point_of_instantiation);
  }
  vertex_stack.push(vertex);
}

void metaprogram_builder::handle_template_end(
  instantiation_kind /* kind */,
  double /* timestamp */,
  unsigned long long /* memory_usage */)
{
  //TODO this check is not enough.
  //The root vertex should always be in the stack
  if (vertex_stack.empty()) {
    throw exception("Bad templight trace format");
  }
  vertex_stack.pop();
}

const metaprogram& metaprogram_builder::get_metaprogram() const {
  return trace;
}

metaprogram_builder::vertex_descriptor metaprogram_builder::add_vertex(
    const std::string& context)
{
  element_vertex_map_t::iterator pos;
  bool inserted;

  std::tie(pos, inserted) = element_vertex_map.insert(
      std::make_pair(context, vertex_descriptor()));

  if (inserted) {
    pos->second = trace.add_vertex(context);
  }
  return pos->second;
}

file_location file_location_from_string(const std::string& str) {
  std::vector<std::string> parts;
  boost::algorithm::split(parts, str, boost::algorithm::is_any_of("|"));

  if (parts.size() != 3) {
    throw exception("templight xml parse failed (invalid file location)");
  }
  return file_location(
      parts[0],
      boost::lexical_cast<int>(parts[1]),
      boost::lexical_cast<int>(parts[2]));
}

instantiation_kind instantiation_kind_from_string(const std::string& str) {
  //TODO boost::flat_map
  const static std::map<std::string, instantiation_kind> lookup = {
     {"TemplateInstantiation", template_instantiation},
     {"DefaultTemplateArgumentInstantiation", default_template_argument_instantiation},
     {"DefaultFunctionArgumentInstantiation", default_function_argument_instantiation},
     {"ExplicitTemplateArgumentSubstitution", explicit_template_argument_substitution},
     {"DeducedTemplateArgumentSubstitution", deduced_template_argument_substitution},
     {"PriorTemplateArgumentSubstitution", prior_template_argument_substitution},
     {"DefaultTemplateArgumentChecking", default_template_argument_checking},
     {"ExceptionSpecInstantiation", exception_spec_instantiation},
     {"Memoization", memoization}
  };

  auto it = lookup.find(str);
  if (it == lookup.end()) {
    throw exception("templight xml parse failed (invalid instantiation kind)");
  }
  return it->second;
}

metaprogram metaprogram::create_from_xml_stream(std::istream& stream) {
  typedef boost::property_tree::ptree ptree;

  ptree pt;
  read_xml(stream, pt);

  metaprogram_builder builder;

  for (const ptree::value_type& pt_event :
      boost::make_iterator_range(pt.get_child("Trace")))
  {
    if (pt_event.first == "TemplateBegin") {
      builder.handle_template_begin(
          instantiation_kind_from_string(
            pt_event.second.get<std::string>("Kind")),
          pt_event.second.get<std::string>("Context.<xmlattr>.context"),
          file_location_from_string(
            pt_event.second.get<std::string>("PointOfInstantiation")),
          pt_event.second.get<double>("TimeStamp.<xmlattr>.time"),
          pt_event.second.get<unsigned long long>("MemoryUsage.<xmlattr>.bytes"));
    } else if (pt_event.first == "TemplateEnd") {
      builder.handle_template_end(
          instantiation_kind_from_string(
            pt_event.second.get<std::string>("Kind")),
          pt_event.second.get<double>("TimeStamp.<xmlattr>.time"),
          pt_event.second.get<unsigned long long>("MemoryUsage.<xmlattr>.bytes"));
    } else {
      throw exception("Unknown templight xml node \"" + pt_event.first + "\"");
    }
  }
  return builder.get_metaprogram();
}

metaprogram metaprogram::create_from_xml_file(const std::string& file) {
  std::ifstream in(file);
  if (!in) {
    throw exception("Can't open templight file");
  }
  return create_from_xml_stream(in);
}

metaprogram metaprogram::create_from_xml_string(const std::string& string) {
  std::istringstream ss(string);
  return create_from_xml_stream(ss);
}

}

