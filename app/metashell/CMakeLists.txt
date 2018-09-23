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

  boost::atomic
  boost::filesystem
  boost::program_options
  boost::regex
  boost::system
  boost::thread
  boost::wave

  just::console
  just::temp

  metashell::core
  metashell::process
  metashell::readline

  mpark::variant

  protobuf

  yaml_cpp

  ${CMAKE_THREAD_LIBS_INIT}
  ${RT_LIBRARY}
  ${PROTOBUF_LIBRARY}
)

# Readline
if (WIN32)
  target_link_libraries(${PROJECT_NAME} wineditline)
  target_compile_definitions(${PROJECT_NAME} PRIVATE USE_EDITLINE)
elseif (USE_EDITLINE)
  include_directories(${EDITLINE_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} ${EDITLINE_LIBRARY})
  target_compile_definitions(${PROJECT_NAME} PRIVATE USE_EDITLINE)
else()
  include_directories(${READLINE_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} ${READLINE_LIBRARY} termcap)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION bin COMPONENT metashell)

set_property(TARGET ${PROJECT_NAME} PROPERTY INSTALL_RPATH_USE_LINK_PATH true)

if (TEMPLIGHT_FOUND)
  #############################################
  # Install a custom Templight with Metashell #
  #############################################
  
  include(MetashellTemplight)
  
  copy_templight_next_to_binary(true)
  
  if (WIN32)
    # Windows header files
    copy_all_files_to_binary_dir(
      "${CMAKE_SOURCE_DIR}/windows_headers"
      "windows_headers"
      true
      windows_headers
    )
  endif ()
endif ()

