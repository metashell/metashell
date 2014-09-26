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
#  TERMCAP_INCLUDEDIR   Set this if the module can not find the Termcap
#                       headers
#  TERMCAP_LIBRARYDIR   Set this if the module can not find the Termcap
#                       library
#  TERMCAP_DEBUG        Set this for verbose output
#
# This module will define the following:
#   TERMCAP_FOUND
#   TERMCAP_INCLUDE_DIR
#   TERMCAP_LIBRARY

if (NOT $ENV{TERMCAP_INCLUDEDIR} STREQUAL "" )
  set(TERMCAP_INCLUDEDIR $ENV{TERMCAP_INCLUDEDIR})
endif()

if (NOT $ENV{TERMCAP_LIBRARYDIR} STREQUAL "" )
  set(TERMCAP_LIBRARYDIR $ENV{TERMCAP_LIBRARYDIR})
endif()

# Open SuSE 64 bit
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(TERMCAP_LIBRARYDIR "${TERMCAP_LIBRARYDIR};/usr/lib64/termcap")
endif()

if (TERMCAP_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  message(STATUS "Searching for Termcap")
  message(STATUS "  TERMCAP_INCLUDEDIR = ${TERMCAP_INCLUDEDIR}")
  message(STATUS "  TERMCAP_LIBRARYDIR = ${TERMCAP_LIBRARYDIR}")
endif()

find_path(
  TERMCAP_INCLUDE_DIR
  NAMES termcap.h
  HINTS ${TERMCAP_INCLUDEDIR}
)
find_library(
  TERMCAP_LIBRARY
  NAMES termcap
  HINTS ${TERMCAP_LIBRARYDIR}
)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set TERMCAP_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
  Termcap DEFAULT_MSG TERMCAP_LIBRARY TERMCAP_INCLUDE_DIR
)

mark_as_advanced(TERMCAP_INCLUDE_DIR, TERMCAP_LIBRARY)

if (TERMCAP_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  if (TERMCAP_FOUND)
    message(STATUS "Termcap found")
    message(STATUS "  TERMCAP_INCLUDE_DIR = ${TERMCAP_INCLUDE_DIR}")
    message(STATUS "  TERMCAP_LIBRARY = ${TERMCAP_LIBRARY}")
  else()
    message(STATUS "Termcap not found")
  endif()
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
endif()


