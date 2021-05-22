# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

function(copy_file_to_binary_dir
  SRC_PATH
  DST_PATH
  INSTALL_COPIED_FILES
  COMPONENT
)
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
  if (INSTALL_COPIED_FILES)
    install(
      FILES
      "${CMAKE_CURRENT_BINARY_DIR}/Release/${DST_PATH}"
      DESTINATION "bin/${DST_DIR}"
      COMPONENT ${COMPONENT}
    )
  endif()
endfunction(copy_file_to_binary_dir)

function(copy_all_files_to_binary_dir
  SRC_PATH
  DST_PATH
  INSTALL_COPIED_FILES
  COMPONENT
)
  file(GLOB_RECURSE ABS_PATHS ${SRC_PATH}/*)
  
  foreach (P ${ABS_PATHS})
    file(RELATIVE_PATH REL_PATH ${SRC_PATH} ${P})

    copy_file_to_binary_dir(
      "${SRC_PATH}/${REL_PATH}"
      "${DST_PATH}/${REL_PATH}"
      ${INSTALL_COPIED_FILES}
      ${COMPONENT}
    )
  endforeach ()
endfunction(copy_all_files_to_binary_dir)

function(copy_all_files_to_include SRC_PATH INSTALL_PATH INSTALL_COPIED_FILES)
  file(GLOB_RECURSE ABS_PATHS ${SRC_PATH}/*)
  
  foreach (P ${ABS_PATHS})
    file(RELATIVE_PATH REL_PATH ${SRC_PATH} ${P})
    get_filename_component(DST_DIR "${REL_PATH}" PATH)

    # Make a copy for running from the build's output dir
    configure_file(
      "${P}"
      "${CMAKE_CURRENT_BINARY_DIR}/../${INSTALL_PATH}/${REL_PATH}"
      COPYONLY
    )

    # Install the headers on the target system
    if (INSTALL_COPIED_FILES)
      install(
        FILES "${SRC_PATH}/${REL_PATH}"
        DESTINATION "${INSTALL_PATH}/${DST_DIR}"
        COMPONENT templight
      )
    endif()
  endforeach ()
endfunction()

function(copy_templight_next_to_binary INSTALL_COPIED_FILES)
  if (WIN32)
    # Headers used by Templight on Windows
    copy_all_files_to_binary_dir(
      "${TEMPLIGHT_HEADERS}"
      "templight/include"
      true
      templight
    )
  
    # templight
    copy_file_to_binary_dir(
      "${TEMPLIGHT_BINARY}"
      "templight/templight.exe"
      true
      templight
    )
  else()
    # Templight binary
    set(MY_TEMPLIGHT "${CMAKE_CURRENT_BINARY_DIR}/templight_metashell")
    configure_file("${TEMPLIGHT_BINARY}" "${MY_TEMPLIGHT}" COPYONLY)
    if (INSTALL_COPIED_FILES)
      install(
        FILES "${MY_TEMPLIGHT}"
        DESTINATION bin
        PERMISSIONS
          OWNER_READ OWNER_EXECUTE OWNER_WRITE
          GROUP_READ GROUP_EXECUTE
          WORLD_READ WORLD_EXECUTE
        COMPONENT templight
      )
    endif()

    # Templight headers
    copy_all_files_to_include(
      "${TEMPLIGHT_HEADERS}"
      "${TEMPLIGHT_HEADER_INSTALL_PATH}"
      ${INSTALL_COPIED_FILES}
    )

    if (APPLE)
      # Libcxx headers
      copy_all_files_to_include(
        "${CMAKE_SOURCE_DIR}/3rd/templight/libcxx/include"
        "${LIBCXX_HEADER_INSTALL_PATH}"
        ${INSTALL_COPIED_FILES}
      )
    endif()
  endif()
endfunction(copy_templight_next_to_binary)

