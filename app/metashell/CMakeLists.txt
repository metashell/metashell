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

aux_source_directory(. SOURCES)
add_executable(metashell ${SOURCES})

enable_warnings()
use_cpp14()

# Metashell
target_link_libraries(metashell
  metashell_core_lib
  metashell_readline_lib
  metashell_data_lib
  metashell_process_lib
)

# Wave
target_link_libraries(metashell
  boost_system
  boost_thread
  ${BOOST_ATOMIC_LIB}
  boost_filesystem
  boost_wave
  ${CMAKE_THREAD_LIBS_INIT}
  ${RT_LIBRARY}
  ${PROTOBUF_LIBRARY}
  protobuf
)

# Readline
if (WIN32)
  target_link_libraries(metashell edit_static)
  add_definitions( -DUSE_EDITLINE )
  include_directories("${CMAKE_SOURCE_DIR}/3rd/wineditline")
else()
  if (USE_EDITLINE)
    include_directories(${EDITLINE_INCLUDE_DIR})
    target_link_libraries(metashell ${EDITLINE_LIBRARY})
    add_definitions( -DUSE_EDITLINE )
  else()
    include_directories(${READLINE_INCLUDE_DIR})
    target_link_libraries(metashell ${READLINE_LIBRARY} termcap_lib)
  endif()
endif()

# Just
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/just_console/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/just_temp/include")

# Program_options
target_link_libraries(metashell boost_program_options)

# Regex
target_link_libraries(metashell boost_regex)

# yaml-cpp
target_link_libraries(metashell yaml_cpp_lib)

# Mpark.Variant
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/mpark_variant/include")

install(TARGETS metashell DESTINATION bin COMPONENT metashell)

set_property(TARGET metashell PROPERTY INSTALL_RPATH_USE_LINK_PATH true)

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

