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
#  RT_LIBRARYDIR   Set this if the module can not find the rt library
#  RT_DEBUG        Set this for verbose output
#
# This module will define the following:
#   RT_FOUND
#   RT_LIBRARY

if (NOT $ENV{RT_INCLUDEDIR} STREQUAL "" )
  set(RT_INCLUDEDIR $ENV{RT_INCLUDEDIR})
endif()

if (NOT $ENV{RT_LIBRARYDIR} STREQUAL "" )
  set(RT_LIBRARYDIR $ENV{RT_LIBRARYDIR})
endif()

if (RT_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  message(STATUS "Searching for Rt")
  message(STATUS "  RT_LIBRARYDIR = ${RT_LIBRARYDIR}")
endif()

find_library(RT_LIBRARY NAMES rt HINTS ${RT_LIBRARYDIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set RT_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Rt DEFAULT_MSG RT_LIBRARY)

mark_as_advanced(RT_LIBRARY)

if (RT_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  if (RT_FOUND)
    message(STATUS "Rt found")
    message(STATUS "  RT_LIBRARY = ${RT_LIBRARY}")
  else()
    message(STATUS "Rt not found")
  endif()
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
endif()

