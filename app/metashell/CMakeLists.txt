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

project(metashell)

aux_source_directory(. SOURCES)
add_executable(${PROJECT_NAME} ${SOURCES})

enable_warnings(${PROJECT_NAME})

target_link_libraries(
  ${PROJECT_NAME}

  boost::algorithm
  boost::filesystem
  boost::range
  boost::utility

  just::temp

  metashell::core
  metashell::data
  metashell::defaults
  metashell::iface
  metashell::main_shell
  metashell::readline
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin COMPONENT metashell)

set_property(TARGET ${PROJECT_NAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH true)

if (TEMPLIGHT_FOUND)
  #############################################
  # Install a custom Templight with Metashell #
  #############################################
  
  include(MetashellTemplight)
  
  copy_templight_next_to_binary(true)
endif ()

