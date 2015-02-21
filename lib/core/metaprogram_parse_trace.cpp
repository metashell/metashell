
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

#include <metashell/exception.hpp>
#include <metashell/metaprogram.hpp>
#include <metashell/metaprogram_builder.hpp>

#include <templight/ProtobufReader.h>

namespace metashell {

data::instantiation_kind instantiation_kind_from_protobuf(int kind) {
  switch (kind) {
    case 0:
      return data::instantiation_kind::template_instantiation;
    case 1:
      return data::instantiation_kind::default_template_argument_instantiation;
    case 2:
      return data::instantiation_kind::default_function_argument_instantiation;
    case 3:
      return data::instantiation_kind::explicit_template_argument_substitution;
    case 4:
      return data::instantiation_kind::deduced_template_argument_substitution;
    case 5:
      return data::instantiation_kind::prior_template_argument_substitution;
    case 6:
      return data::instantiation_kind::default_template_argument_checking;
    case 7:
      return data::instantiation_kind::exception_spec_instantiation;
    case 8:
      return data::instantiation_kind::memoization;
    default:
      throw exception(
          "templight xml parse failed (invalid instantiation kind)");
  }
}

metaprogram metaprogram::create_from_protobuf_stream(
    std::istream& stream,
    bool full_mode,
    const std::string& root_name,
    const data::type_or_error& evaluation_result)
{

  metaprogram_builder builder(full_mode, root_name, evaluation_result);
  templight::ProtobufReader reader;
  reader.startOnBuffer(stream);
  while (reader.LastChunk != templight::ProtobufReader::EndOfFile) {
    switch (reader.LastChunk) {
      case templight::ProtobufReader::BeginEntry:
        {
          auto begin_entry = reader.LastBeginEntry;
          builder.handle_template_begin(
              instantiation_kind_from_protobuf(begin_entry.InstantiationKind),
              begin_entry.Name,
              data::file_location(
                begin_entry.FileName, begin_entry.Line, begin_entry.Column));
          break;
        }
      case templight::ProtobufReader::EndEntry:
        builder.handle_template_end();
        break;
      case templight::ProtobufReader::EndOfFile:
      case templight::ProtobufReader::Other:
      case templight::ProtobufReader::Header:
      default:
        break;
    }
    reader.next();
  }
  return builder.get_metaprogram();
}

metaprogram metaprogram::create_from_protobuf_string(
    const std::string& string,
    bool full_mode,
    const std::string& root_name,
    const data::type_or_error& evaluation_result)
{
  std::istringstream ss(string);
  return create_from_protobuf_stream(ss, full_mode, root_name, evaluation_result);
}

}

