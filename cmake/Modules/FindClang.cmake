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
#  CLANG_INCLUDEDIR   Set this if the module can not find the Readline
#                     headers
#  CLANG_LIBRARYDIR   Set this if the module can not find the Readline
#                     library
#  CLANG_DEBUG        Set this for verbose output
#
# This module will define the following:
#   CLANG_FOUND
#   CLANG_INCLUDE_DIR
#   CLANG_LIBRARY

if (NOT $ENV{CLANG_INCLUDEDIR} STREQUAL "" )
  set(CLANG_INCLUDEDIR $ENV{CLANG_INCLUDEDIR})
endif()

if (NOT $ENV{CLANG_LIBRARYDIR} STREQUAL "" )
  set(CLANG_LIBRARYDIR $ENV{CLANG_LIBRARYDIR})
endif()

if (CLANG_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  message(STATUS "Searching for libclang")
  message(STATUS "  CLANG_INCLUDEDIR = ${CLANG_INCLUDEDIR}")
  message(STATUS "  CLANG_LIBRARYDIR = ${CLANG_LIBRARYDIR}")
endif()

find_path(CLANG_INCLUDE_DIR NAMES clang-c/Index.h HINTS ${CLANG_INCLUDEDIR})
find_library(CLANG_LIBRARY NAMES clang HINTS ${CLANG_LIBRARYDIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set CLANG_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
  CLANG DEFAULT_MSG CLANG_LIBRARY CLANG_INCLUDE_DIR
)

mark_as_advanced(CLANG_INCLUDE_DIR, CLANG_LIBRARY)

if (CLANG_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  if (CLANG_FOUND)
    message(STATUS "libclang found")
    message(STATUS "  CLANG_INCLUDE_DIR = ${CLANG_INCLUDE_DIR}")
    message(STATUS "  CLANG_LIBRARY = ${CLANG_LIBRARY}")
  else()
    message(STATUS "libclang not found")
  endif()
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
endif()

