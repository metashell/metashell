
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
#include <metashell/metaprogram_parse_trace.hpp>
#include <metashell/protobuf_trace.hpp>
#include <metashell/yaml_trace.hpp>

#include <sstream>

namespace metashell
{
  data::metaprogram create_metaprogram_from_protobuf_stream(
      std::istream& stream,
      data::metaprogram::mode_t mode,
      const data::cpp_code& root_name,
      const data::file_location& root_source_location,
      const data::type_or_code_or_error& evaluation_result)
  {
    protobuf_trace trace(stream, evaluation_result);

    return metaprogram_builder(trace, mode, root_name, root_source_location)
        .get_metaprogram();
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
    yaml_trace ytrace(trace, evaluation_result);

    return metaprogram_builder(ytrace, mode, root_name, root_source_location)
        .get_metaprogram();
  }
}
