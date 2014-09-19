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

