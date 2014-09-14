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

# Regex
target_link_libraries(metashell ${Boost_REGEX_LIBRARIES})

# Clang
if (MSVC)
  # libclang calls its import library libclang.imp instead of libclang.lib
  set(CMAKE_IMPORT_LIBRARY_SUFFIX ".imp")

  # delayload libclang
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DELAYLOAD:libclang.dll")
endif()
target_link_libraries(metashell ${CLANG_LIBRARY})
include_directories(${CLANG_INCLUDE_DIR})

if (CLANG_STATIC)
  target_link_libraries(metashell ${ZLIB_LIBRARIES})
endif()

install(TARGETS metashell DESTINATION bin COMPONENT metashell)

set_property(TARGET metashell PROPERTY INSTALL_RPATH_USE_LINK_PATH true)

#########################################
# Install a custom Clang with Metashell #
#########################################

if (NOT WIN32)
  # Clang binary
  set(MY_CLANG "${CMAKE_CURRENT_BINARY_DIR}/clang_metashell")
  configure_file("${CLANG_BINARY}" "${MY_CLANG}" COPYONLY)
  install(
    FILES "${MY_CLANG}"
    DESTINATION bin
    PERMISSIONS
      OWNER_READ OWNER_EXECUTE
      GROUP_READ GROUP_EXECUTE
      WORLD_READ WORLD_EXECUTE
    COMPONENT clang
  )

  # Clang headers
  file(GLOB_RECURSE ABS_PATHS ${CLANG_HEADERS}/*)
  
  foreach (P ${ABS_PATHS})
    file(RELATIVE_PATH REL_PATH ${CLANG_HEADERS} ${P})
    get_filename_component(DST_DIR "${REL_PATH}" PATH)

    install(
      FILES "${CLANG_HEADERS}/${REL_PATH}"
      DESTINATION "${CLANG_HEADER_INSTALL_PATH}/${DST_DIR}"
      COMPONENT clang
    )
  endforeach ()
endif ()

############################################
# Copy files next to the binary on Windows #
############################################
function(copy_file_to_binary_dir SRC_PATH DST_PATH COMPONENT)
  get_filename_component(DST_DIR "${DST_PATH}" PATH)

  configure_file(
    "${SRC_PATH}"
    "${CMAKE_CURRENT_BINARY_DIR}/Debug/${DST_PATH}"
    COPYONLY
  )
  configure_file(
    "${SRC_PATH}"
    "${CMAKE_CURRENT_BINARY_DIR}/Release/${DST_PATH}"
    COPYONLY
  )
  install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/Release/${DST_PATH}"
    DESTINATION "bin/${DST_DIR}"
    COMPONENT ${COMPONENT}
  )
endfunction(copy_file_to_binary_dir)

function(copy_all_files_to_binary_dir SRC_PATH DST_PATH COMPONENT)
  file(GLOB_RECURSE ABS_PATHS ${SRC_PATH}/*)
  
  foreach (P ${ABS_PATHS})
    file(RELATIVE_PATH REL_PATH ${SRC_PATH} ${P})

    copy_file_to_binary_dir(
      "${SRC_PATH}/${REL_PATH}"
      "${DST_PATH}/${REL_PATH}"
      ${COMPONENT}
    )
  endforeach ()
endfunction(copy_all_files_to_binary_dir)

#########################################################
# Copying files next to the Metashell binary on Windows #
#########################################################
if (WIN32)
  # Windows header files
  copy_all_files_to_binary_dir(
    "${CMAKE_SOURCE_DIR}/windows_headers"
    "windows_headers"
    windows_headers
  )

  # libclang.dll
  copy_file_to_binary_dir("${CLANG_DLL}" "clang/libclang.dll" libclang)

  # Headers used by Clang on Windows
  copy_all_files_to_binary_dir("${CLANG_HEADERS}" "clang/include" libclang)

  # clang compiler
  copy_file_to_binary_dir("${CLANG_BINARY}" "clang/clang.exe" clang)
endif ()

