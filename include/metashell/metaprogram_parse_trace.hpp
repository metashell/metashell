#ifndef METASHELL_METAPROGRAM_PARSE_TRACE_HPP
#define METASHELL_METAPROGRAM_PARSE_TRACE_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/file_location.hpp>
#include <metashell/data/metaprogram.hpp>
#include <metashell/data/type_or_error.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  data::metaprogram create_metaprogram_from_protobuf_stream(
      std::istream& stream,
      data::metaprogram::mode_t mode,
      const std::string& root_name,
      const data::file_location& root_source_location,
      const data::type_or_error& evaluation_result);

  data::metaprogram create_metaprogram_from_protobuf_string(
      const std::string& string,
      data::metaprogram::mode_t mode,
      const std::string& root_name,
      const data::file_location& root_source_location,
      const data::type_or_error& evaluation_result);
}

#endif
