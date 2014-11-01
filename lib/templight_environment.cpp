
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
  clang_arguments().push_back("-templight");
  clang_arguments().push_back("-templight-format");
  clang_arguments().push_back("xml");
  clang_arguments().push_back("-templight-output");
  clang_arguments().push_back("TEMPLIGHT_XML_LOCATION_IS_NOT_SET");
  xml_path_index = clang_arguments().size() - 1;
}

void templight_environment::set_xml_location(const std::string& xml_location) {
  clang_arguments()[xml_path_index] = xml_location;
}

}
