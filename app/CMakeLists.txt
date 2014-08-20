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

# Metashell
target_link_libraries(metashell metashell_lib)

# Wave
target_link_libraries(metashell
  ${Boost_SYSTEM_LIBRARIES}
  ${Boost_THREAD_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARIES}
  ${Boost_WAVE_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

# Readline
if (WIN32)
  target_link_libraries(metashell edit_static)
  add_definitions( -DUSE_EDITLINE )
  include_directories("${CMAKE_SOURCE_DIR}/wineditline")
else()
  if (USE_EDITLINE)
    include_directories(${EDITLINE_INCLUDE_DIR})
    target_link_libraries(metashell ${EDITLINE_LIBRARY})
    add_definitions( -DUSE_EDITLINE )
  else()
    include_directories(${READLINE_INCLUDE_DIR})
    target_link_libraries(metashell ${READLINE_LIBRARY} ${TERMCAP_LIBRARY})
  endif()
endif()

# Program_options
target_link_libraries(metashell ${Boost_PROGRAM_OPTIONS_LIBRARIES})

# Clang
if (MSVC)
  # libclang calls its import library libclang.imp instead of libclang.lib
  set(CMAKE_IMPORT_LIBRARY_SUFFIX ".imp")

  # delayload libclang
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DELAYLOAD:libclang.dll")
endif()
target_link_libraries(metashell ${CLANG_LIBRARY})
include_directories(${CLANG_INCLUDE_DIR})

install(TARGETS metashell DESTINATION bin)
set_property(TARGET metashell PROPERTY INSTALL_RPATH_USE_LINK_PATH true)

########################
# Windows header files #
########################
if (WIN32)
  file(GLOB_RECURSE ABS_WINDOWS_HEADERS ${CMAKE_SOURCE_DIR}/windows_headers/*)
  
  foreach (HEADER ${ABS_WINDOWS_HEADERS})
    file(RELATIVE_PATH REL_HEADER ${CMAKE_SOURCE_DIR} ${HEADER})
    list(APPEND WINDOWS_HEADERS ${REL_HEADER})
  endforeach()
  
  foreach (HEADER ${WINDOWS_HEADERS})
    configure_file(
      "${CMAKE_SOURCE_DIR}/${HEADER}"
      "${CMAKE_CURRENT_BINARY_DIR}/Debug/${HEADER}"
      COPYONLY
    )
    configure_file(
      "${CMAKE_SOURCE_DIR}/${HEADER}"
      "${CMAKE_CURRENT_BINARY_DIR}/Release/${HEADER}"
      COPYONLY
    )
  endforeach ()
endif ()

