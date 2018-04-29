
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

#include <metashell/metaprogram_parse_trace.hpp>
#include <metashell/protobuf_trace.hpp>
#include <metashell/yaml_trace.hpp>

#include <metashell/filter_events.hpp>

#include <metashell/data/metaprogram.hpp>

#include <sstream>

namespace metashell
{
  data::metaprogram create_metaprogram_from_protobuf_stream(
      std::istream& stream,
      data::metaprogram::mode_t mode,
      const data::cpp_code& root_name,
      const data::type_or_code_or_error& evaluation_result,
      boost::optional<data::file_location> from_line)
  {
    auto trace = filter_events(protobuf_trace(stream, evaluation_result),
                               std::move(from_line),
                               mode != data::metaprogram::mode_t::full);

    return data::metaprogram(trace, mode, root_name);
  }

  data::metaprogram create_metaprogram_from_protobuf_string(
      const std::string& string,
      data::metaprogram::mode_t mode,
      const data::cpp_code& root_name,
      const data::type_or_code_or_error& evaluation_result,
      boost::optional<data::file_location> from_line)
  {
    std::istringstream ss(string);
    return create_metaprogram_from_protobuf_stream(
        ss, mode, root_name, evaluation_result, std::move(from_line));
  }

  data::metaprogram create_metaprogram_from_yaml_trace(
      const std::string& trace,
      data::metaprogram::mode_t mode,
      const data::cpp_code& root_name,
      const data::type_or_code_or_error& evaluation_result,
      boost::optional<data::file_location> from_line)
  {
    auto ytrace = filter_events(yaml_trace(trace, evaluation_result),
                                std::move(from_line),
                                mode != data::metaprogram::mode_t::full);

    return data::metaprogram(ytrace, mode, root_name);
  }
}
