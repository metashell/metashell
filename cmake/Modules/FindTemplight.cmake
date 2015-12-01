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

# Options for the module
#  TEMPLIGHT_BINARYDIR  Set this if the module can not find the templight binary
#  TEMPLIGHT_DEBUG      Set this for verbose output
#
# This module will define the following:
#   TEMPLIGHT_FOUND
#   TEMPLIGHT_HEADERS (the path to the headers used by templight)
#   TEMPLIGHT_BINARY

if (WIN32)
  set(
    TEMPLIGHT_BINARYDIR
    "${TEMPLIGHT_BINARYDIR};C:/Program Files (x86)/LLVM/lib"
  )
  set(
    TEMPLIGHT_BINARYDIR
    "${TEMPLIGHT_BINARYDIR};C:/Program Files (x86)/LLVM/bin"
  )
endif()

if (TEMPLIGHT_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  message(STATUS "TEMPLIGHT_BINARYDIR = ${TEMPLIGHT_BINARYDIR}")
endif()

# The templight binary
find_program(TEMPLIGHT_BINARY templight HINTS ${TEMPLIGHT_BINARYDIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set TEMPLIGHT_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(TEMPLIGHT DEFAULT_MSG TEMPLIGHT_BINARY)

# The standard Clang header files
file(WRITE "${PROJECT_BINARY_DIR}/empty.hpp" "")
execute_process(
  COMMAND ${TEMPLIGHT_BINARY} -v -xc++ "${PROJECT_BINARY_DIR}/empty.hpp" -c
  ERROR_VARIABLE TEMPLIGHT_OUTPUT
)
string(REGEX MATCHALL "[^\r\n]*[\r\n]" LINES "${TEMPLIGHT_OUTPUT}")
foreach (L ${LINES})
  if ("${L}" MATCHES "^[ ][^\"]*$")
    string(STRIP "${L}" SL)
    list(APPEND TEMPLIGHT_INCLUDE_PATH ${SL})
  endif()
endforeach()

find_path(TEMPLIGHT_HEADERS NAMES altivec.h HINTS ${TEMPLIGHT_INCLUDE_PATH})

# Done

if (TEMPLIGHT_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  if (TEMPLIGHT_FOUND)
    message(STATUS "templight found")
    message(STATUS "  TEMPLIGHT_BINARY = ${TEMPLIGHT_BINARY}")
    message(STATUS "  TEMPLIGHT_HEADERS = ${TEMPLIGHT_HEADERS}")
  else()
    message(STATUS "templight not found")
  endif()
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
endif()

