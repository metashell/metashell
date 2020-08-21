# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

project(metashell_core)

aux_source_directory(src SOURCES)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_library(metashell::core ALIAS ${PROJECT_NAME})

enable_warnings(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON)

target_link_libraries(
  ${PROJECT_NAME}

  boost::algorithm
  boost::container
  boost::filesystem
  boost::range
  boost::utility
  boost::wave

  just::environment
  just::file
  just::lines

  metashell::data
  metashell::iface

  mindent
  rapidjson
)
