
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
#include <fstream>
#include <sstream>
#include <iostream>

#include <metashell/type.hpp>
#include <metashell/exception.hpp>
#include <metashell/metaprogram.hpp>

#include "templight_messages.pb.h"

namespace metashell {

struct metaprogram_builder {

  metaprogram_builder(
      bool full_mode,
      const std::string& root_name,
      const type& evaluation_result);

  void handle_template_begin(
    instantiation_kind kind,
    const std::string& context,
    const file_location& location);

  void handle_template_end();

  const metaprogram& get_metaprogram() const;

private:
  typedef metaprogram::vertex_descriptor vertex_descriptor;
  typedef std::map<std::string, vertex_descriptor> element_vertex_map_t;

  vertex_descriptor add_vertex(const std::string& context);

  metaprogram mp;

  std::stack<vertex_descriptor> vertex_stack;

  element_vertex_map_t element_vertex_map;
};

metaprogram_builder::metaprogram_builder(
    bool full_mode,
    const std::string& root_name,
    const type& evaluation_result) :
  mp(full_mode, root_name, evaluation_result)
{}

void metaprogram_builder::handle_template_begin(
  instantiation_kind kind,
  const std::string& context,
  const file_location& point_of_instantiation)
{
  vertex_descriptor vertex = add_vertex(context);
  vertex_descriptor top_vertex =
    vertex_stack.empty() ? mp.get_root_vertex() : vertex_stack.top();

  mp.add_edge(top_vertex, vertex, kind, point_of_instantiation);
  vertex_stack.push(vertex);
}

void metaprogram_builder::handle_template_end() {
  if (vertex_stack.empty()) {
    throw exception(
        "Mismatched Templight TemplateBegin and TemplateEnd events");
  }
  vertex_stack.pop();
}

const metaprogram& metaprogram_builder::get_metaprogram() const {
  if (!vertex_stack.empty()) {
    throw exception(
        "Some Templight TemplateEnd events are missing");
  }
  return mp;
}

metaprogram_builder::vertex_descriptor metaprogram_builder::add_vertex(
    const std::string& context)
{
  element_vertex_map_t::iterator pos;
  bool inserted;

  std::tie(pos, inserted) = element_vertex_map.insert(
      std::make_pair(context, vertex_descriptor()));

  if (inserted) {
    pos->second = mp.add_vertex(context);
  }
  return pos->second;
}

typedef std::map<unsigned, std::string> file_dictionary;

file_location file_location_from_protobuf(
    const TemplightEntry::SourceLocation& source_location,
    file_dictionary& dict)
{
  if (source_location.has_file_name()) {
    dict[source_location.file_id()] = source_location.file_name();
  }

  int col = source_location.has_column() ? source_location.column() : -1;

  return file_location(
      dict[source_location.file_id()],
      source_location.line(),
      col);
}

instantiation_kind instantiation_kind_from_protobuf(
    const TemplightEntry::InstantiationKind& kind)
{
  switch (kind) {
    case TemplightEntry::TemplateInstantiation:
      return instantiation_kind::template_instantiation;
    case TemplightEntry::DefaultTemplateArgumentInstantiation:
      return instantiation_kind::default_template_argument_instantiation;
    case TemplightEntry::DefaultFunctionArgumentInstantiation:
      return instantiation_kind::default_function_argument_instantiation;
    case TemplightEntry::ExplicitTemplateArgumentSubstitution:
      return instantiation_kind::explicit_template_argument_substitution;
    case TemplightEntry::DeducedTemplateArgumentSubstitution:
      return instantiation_kind::deduced_template_argument_substitution;
    case TemplightEntry::PriorTemplateArgumentSubstitution:
      return instantiation_kind::prior_template_argument_substitution;
    case TemplightEntry::DefaultTemplateArgumentChecking:
      return instantiation_kind::default_template_argument_checking;
    case TemplightEntry::ExceptionSpecInstantiation:
      return instantiation_kind::exception_spec_instantiation;
    case TemplightEntry::Memoization:
      return instantiation_kind::memoization;
    default:
      throw exception(
          "templight xml parse failed (invalid instantiation kind)");
  }
}

//TODO this is probably very slow
std::string resolve_name(int id, const TemplightTrace& trace) {

  if (id >= trace.names_size()) {
    throw exception("id out of range");
  }

  const DictionaryEntry& entry = trace.names(id);
  int marker_idx = 0;

  std::stringstream ss;

  for (char ch : entry.marked_name()) {
    if (ch != '\0') {
      ss << ch;
    } else {
      ss << resolve_name(entry.marker_ids(marker_idx++), trace);
    }
  }

  return ss.str();
}

//TODO type instead of std::string
std::string type_from_protobuf(
    const TemplightEntry::TemplateName& name,
    const TemplightTrace& trace) //For the dictionary
{
  if (name.has_name()) {
    return name.name();
  }
  if (name.has_dict_id()) {
    return resolve_name(static_cast<int>(name.dict_id()), trace);
  }
  if (!name.has_compressed_name()) {
    throw exception("TemplateName has no name, dict_id and compressed_name");
  }
  return "?????";
}

metaprogram metaprogram::create_from_protobuf_stream(
    std::istream& stream,
    bool full_mode,
    const std::string& root_name,
    const type& evaluation_result)
{

  TemplightTraceCollection traces;

  if (!traces.ParseFromIstream(&stream)) {
    throw exception("Can't parse protobuf trace file");
  }

  if (traces.traces_size() != 1) {
    throw exception("There are more than one trace in the protobuf trace file");
  }

  const TemplightTrace& trace = traces.traces(0);

  metaprogram_builder builder(full_mode, root_name, evaluation_result);
  file_dictionary dict;

  for (int i = 0; i < trace.entries_size(); ++i) {
    const TemplightEntry& entry = trace.entries(i);
    if (entry.has_begin() && entry.has_end()) {
      throw exception("TemplightEntry has both begin and end object");
    }

    if (entry.has_begin()) {
      const TemplightEntry::Begin& begin = entry.begin();
      builder.handle_template_begin(
          instantiation_kind_from_protobuf(begin.kind()),
          type_from_protobuf(begin.name(), trace),
          file_location_from_protobuf(begin.location(), dict)
      );
    } else if (entry.has_end()) {
      builder.handle_template_end();
    } else {
      throw exception("TemplightEntry has no begin and end object");
    }
  }

  return builder.get_metaprogram();
}

metaprogram metaprogram::create_from_protobuf_file(
    const std::string& file,
    bool full_mode,
    const std::string& root_name,
    const type& evaluation_result)
{
  std::ifstream in(file);
  if (!in) {
    throw exception("Can't open templight file");
  }
  return create_from_protobuf_stream(in, full_mode, root_name, evaluation_result);
}

metaprogram metaprogram::create_from_protobuf_string(
    const std::string& string,
    bool full_mode,
    const std::string& root_name,
    const type& evaluation_result)
{
  std::istringstream ss(string);
  return create_from_protobuf_stream(ss, full_mode, root_name, evaluation_result);
}

}

