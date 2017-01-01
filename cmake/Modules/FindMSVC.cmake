# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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
#  MSVC_BINARYDIR  Set this if the module can not find the compiler
#  MSVC_DEBUG      Set this for verbose output
#
# This module will define the following:
#   MSVC_FOUND
#   MSVC_CL_BINARY

if (MSVC_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  message(STATUS "MSVC_BINARYDIR = ${MSVC_BINARYDIR}")
endif()

# The cl.exe
find_program(MSVC_CL_BINARY cl.exe HINTS ${MSVC_BINARYDIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set MSVC_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(MSVC DEFAULT_MSG MSVC_CL_BINARY)

# Done

if (MSVC_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  if (MSVC_FOUND)
    message(STATUS "MSVC found")
    message(STATUS "  MSVC_CL_BINARY = ${MSVC_CL_BINARY}")
  else()
    message(STATUS "MSVC not found")
  endif()
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
endif()

