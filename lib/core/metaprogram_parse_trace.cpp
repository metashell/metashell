
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

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include <metashell/data/metaprogram.hpp>

#include <metashell/exception.hpp>
#include <metashell/metaprogram_builder.hpp>

#include <templight/ProtobufReader.h>

#include <yaml-cpp/yaml.h>

#include <cassert>

namespace
{
  metashell::data::event_kind instantiation_kind_from_protobuf(int kind)
  {
    using metashell::data::event_kind;

    switch (kind)
    {
    case 0:
      return event_kind::template_instantiation;
    case 1:
      return event_kind::default_template_argument_instantiation;
    case 2:
      return event_kind::default_function_argument_instantiation;
    case 3:
      return event_kind::explicit_template_argument_substitution;
    case 4:
      return event_kind::deduced_template_argument_substitution;
    case 5:
      return event_kind::prior_template_argument_substitution;
    case 6:
      return event_kind::default_template_argument_checking;
    case 7:
      return event_kind::exception_spec_instantiation;
    case 8:
      return event_kind::declaring_special_member;
    case 9:
      return event_kind::defining_synthesized_function;
    case 10:
      return event_kind::memoization;
    default:
      throw metashell::exception(
          "templight xml parse failed (invalid instantiation kind " +
          std::to_string(kind) + ")");
    }
  }

  boost::optional<metashell::data::event_kind>
  instantiation_kind_from_yaml_dump(const std::string& s_)
  {
    using metashell::data::event_kind;

    if (s_ == "TemplateInstantiation")
    {
      return event_kind::template_instantiation;
    }
    else if (s_ == "DefaultTemplateArgumentInstantiation")
    {
      return event_kind::default_template_argument_instantiation;
    }
    else if (s_ == "DefaultFunctionArgumentInstantiation")
    {
      return event_kind::default_function_argument_instantiation;
    }
    else if (s_ == "ExplicitTemplateArgumentSubstitution")
    {
      return event_kind::explicit_template_argument_substitution;
    }
    else if (s_ == "DeducedTemplateArgumentSubstitution")
    {
      return event_kind::deduced_template_argument_substitution;
    }
    else if (s_ == "PriorTemplateArgumentSubstitution")
    {
      return event_kind::prior_template_argument_substitution;
    }
    else if (s_ == "DefaultTemplateArgumentChecking")
    {
      return event_kind::default_template_argument_checking;
    }
    else if (s_ == "ExceptionSpecInstantiation")
    {
      return event_kind::exception_spec_instantiation;
    }
    else if (s_ == "DeclaringSpecialMember")
    {
      return event_kind::declaring_special_member;
    }
    else if (s_ == "DefiningSynthesizedFunction")
    {
      return event_kind::defining_synthesized_function;
    }
    else if (s_ == "Memoization")
    {
      return event_kind::memoization;
    }
    else
    {
      return boost::none;
    }
  }
}

namespace metashell
{
  namespace
  {
    template <data::event_kind Kind>
    void handle_template_begin(metaprogram_builder& builder,
                               const data::type& type,
                               const data::file_location& point_of_event,
                               const data::file_location& source_location,
                               double timestamp)
    {
      builder.handle_event(data::event_details<Kind>{
          type, point_of_event, source_location, timestamp});
    }

    void handle_template_begin(metaprogram_builder& builder,
                               data::event_kind kind,
                               const data::type& type,
                               const data::file_location& point_of_event,
                               const data::file_location& source_location,
                               double timestamp)
    {
      switch (kind)
      {
      case data::event_kind::template_instantiation:
        handle_template_begin<data::event_kind::template_instantiation>(
            builder, type, point_of_event, source_location, timestamp);
        break;
      case data::event_kind::default_template_argument_instantiation:
        handle_template_begin<
            data::event_kind::default_template_argument_instantiation>(
            builder, type, point_of_event, source_location, timestamp);
        break;
      case data::event_kind::default_function_argument_instantiation:
        handle_template_begin<
            data::event_kind::default_function_argument_instantiation>(
            builder, type, point_of_event, source_location, timestamp);
        break;
      case data::event_kind::explicit_template_argument_substitution:
        handle_template_begin<
            data::event_kind::explicit_template_argument_substitution>(
            builder, type, point_of_event, source_location, timestamp);
        break;
      case data::event_kind::deduced_template_argument_substitution:
        handle_template_begin<
            data::event_kind::deduced_template_argument_substitution>(
            builder, type, point_of_event, source_location, timestamp);
        break;
      case data::event_kind::prior_template_argument_substitution:
        handle_template_begin<
            data::event_kind::prior_template_argument_substitution>(
            builder, type, point_of_event, source_location, timestamp);
        break;
      case data::event_kind::default_template_argument_checking:
        handle_template_begin<
            data::event_kind::default_template_argument_checking>(
            builder, type, point_of_event, source_location, timestamp);
        break;
      case data::event_kind::exception_spec_instantiation:
        handle_template_begin<data::event_kind::exception_spec_instantiation>(
            builder, type, point_of_event, source_location, timestamp);
        break;
      case data::event_kind::declaring_special_member:
        handle_template_begin<data::event_kind::declaring_special_member>(
            builder, type, point_of_event, source_location, timestamp);
        break;
      case data::event_kind::defining_synthesized_function:
        handle_template_begin<data::event_kind::defining_synthesized_function>(
            builder, type, point_of_event, source_location, timestamp);
        break;
      case data::event_kind::memoization:
        handle_template_begin<data::event_kind::memoization>(
            builder, type, point_of_event, source_location, timestamp);
        break;
      default:
        assert(!"Invalid event_kind");
        break;
      }
    }
  }

  data::metaprogram create_metaprogram_from_protobuf_stream(
      std::istream& stream,
      data::metaprogram::mode_t mode,
      const data::cpp_code& root_name,
      const data::file_location& root_source_location,
      const data::type_or_code_or_error& evaluation_result)
  {
    metaprogram_builder builder(mode, root_name, root_source_location);

    templight::ProtobufReader reader;
    reader.startOnBuffer(stream);

    while (reader.LastChunk != templight::ProtobufReader::EndOfFile)
    {
      switch (reader.LastChunk)
      {
      case templight::ProtobufReader::BeginEntry:
      {
        auto begin_entry = reader.LastBeginEntry;
        handle_template_begin(
            builder,
            instantiation_kind_from_protobuf(begin_entry.InstantiationKind),
            data::type(begin_entry.Name),
            data::file_location(
                begin_entry.FileName, begin_entry.Line, begin_entry.Column),
            data::file_location(begin_entry.TempOri_FileName,
                                begin_entry.TempOri_Line,
                                begin_entry.TempOri_Column),
            begin_entry.TimeStamp);
        break;
      }
      case templight::ProtobufReader::EndEntry:
      {
        builder.handle_event(
            data::event_details<data::event_kind::template_end>{
                reader.LastEndEntry.TimeStamp});
        break;
      }
      case templight::ProtobufReader::EndOfFile:
      case templight::ProtobufReader::Other:
      case templight::ProtobufReader::Header:
      default:
        break;
      }
      reader.next();
    }
    builder.handle_event(data::event_details<data::event_kind::evaluation_end>{
        evaluation_result});
    return builder.get_metaprogram();
  }

  data::metaprogram create_metaprogram_from_protobuf_string(
      const std::string& string,
      data::metaprogram::mode_t mode,
      const data::cpp_code& root_name,
      const data::file_location& root_source_location,
      const data::type_or_code_or_error& evaluation_result)
  {
    std::istringstream ss(string);
    return create_metaprogram_from_protobuf_stream(
        ss, mode, root_name, root_source_location, evaluation_result);
  }

  data::metaprogram create_metaprogram_from_yaml_trace(
      const std::string& trace,
      data::metaprogram::mode_t mode,
      const data::cpp_code& root_name,
      const data::file_location& root_source_location,
      const data::type_or_code_or_error& evaluation_result)
  {
    metaprogram_builder builder(mode, root_name, root_source_location);

    for (const YAML::Node& node : YAML::LoadAll(trace))
    {
      if (const auto kind =
              instantiation_kind_from_yaml_dump(node["kind"].as<std::string>()))
      {
        const std::string event = node["event"].as<std::string>();
        if (event == "Begin")
        {
          handle_template_begin(
              builder, *kind, data::type(node["name"].as<std::string>()),
              data::file_location::parse(node["poi"].as<std::string>()),
              data::file_location::parse(node["orig"].as<std::string>()), 0);
        }
        else if (event == "End")
        {
          builder.handle_event(
              data::event_details<data::event_kind::template_end>{0});
        }
      }
    }

    builder.handle_event(data::event_details<data::event_kind::evaluation_end>{
        evaluation_result});
    return builder.get_metaprogram();
  }
}
