
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

#include <metashell/templight_environment.hpp>

namespace metashell {

templight_environment::templight_environment(
  const config& config
) : header_file_environment(config)
{
  clang_arguments().push_back("-Xclang");
  clang_arguments().push_back("-ast-dump");
  clang_arguments().push_back("-Xtemplight");
  clang_arguments().push_back("-profiler");

  // templight can't be forced to generate output file with
  // -Xtemplight -output=<file> for some reason
  // A workaround is to specify a standard output location with -o
  // then append ".trace.pbf" to the specified file (on the calling side)
  clang_arguments().push_back("-o");
  clang_arguments().push_back("TEMPLIGHT_OUTPUT_LOCATION_IS_NOT_SET");
  output_path_index = clang_arguments().size() - 1;
}

void templight_environment::set_output_location(
    const std::string& output_location)
{
  clang_arguments()[output_path_index] = output_location;
}

}
