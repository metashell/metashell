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

# Options for the module
#  EDITLINE_INCLUDEDIR   Set this if the module can not find the Editline
#                        headers
#  EDITLINE_LIBRARYDIR   Set this if the module can not find the Editline
#                        library
#  EDITLINE_DEBUG        Set this for verbose output
#
# This module will define the following:
#   EDITLINE_FOUND
#   EDITLINE_INCLUDE_DIR
#   EDITLINE_LIBRARY

if (NOT $ENV{EDITLINE_INCLUDEDIR} STREQUAL "" )
  set(EDITLINE_INCLUDEDIR $ENV{EDITLINE_INCLUDEDIR})
endif()

if (NOT $ENV{EDITLINE_LIBRARYDIR} STREQUAL "" )
  set(EDITLINE_LIBRARYDIR $ENV{EDITLINE_LIBRARYDIR})
endif()

if (EDITLINE_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  message(STATUS "Searching for Editline")
  message(STATUS "  EDITLINE_INCLUDEDIR = ${EDITLINE_INCLUDEDIR}")
  message(STATUS "  EDITLINE_LIBRARYDIR = ${EDITLINE_LIBRARYDIR}")
endif()

find_path(
  EDITLINE_INCLUDE_DIR
  NAMES editline/readline.h
  HINTS ${EDITLINE_INCLUDEDIR}
)
find_library(
  EDITLINE_LIBRARY
  NAMES edit
  HINTS ${EDITLINE_LIBRARYDIR}
)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set EDITLINE_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
  Editline DEFAULT_MSG EDITLINE_LIBRARY EDITLINE_INCLUDE_DIR
)

mark_as_advanced(EDITLINE_INCLUDE_DIR, EDITLINE_LIBRARY)

if (EDITLINE_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  if (EDITLINE_FOUND)
    message(STATUS "Editline found")
    message(STATUS "  EDITLINE_INCLUDE_DIR = ${EDITLINE_INCLUDE_DIR}")
    message(STATUS "  EDITLINE_LIBRARY = ${EDITLINE_LIBRARY}")
  else()
    message(STATUS "Editline not found")
  endif()
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
endif()


