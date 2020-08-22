# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

project(termcap)

aux_source_directory(src SOURCES)
add_library(${PROJECT_NAME} STATIC ${SOURCES})

disable_warnings(${PROJECT_NAME})

# termcap has some TU local static definitions (memory_out, xmalloc, etc)
set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD OFF)

target_include_directories(
  ${PROJECT_NAME} SYSTEM
  INTERFACE ${PROJECT_SOURCE_DIR}/include
  PRIVATE ${PROJECT_SOURCE_DIR}/custom_include
)

target_compile_definitions(
  ${PROJECT_NAME}
  
  PRIVATE
    STDC_HEADERS
    HAVE_CONFIG_H
)

