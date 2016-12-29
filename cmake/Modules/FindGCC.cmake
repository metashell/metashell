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
#  GCC_BINARYDIR  Set this if the module can not find the compiler
#  GCC_DEBUG      Set this for verbose output
#
# This module will define the following:
#   GCC_FOUND
#   GCC_BINARY
#   GXX_FOUND
#   GXX_BINARY

if (GCC_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  message(STATUS "GCC_BINARYDIR = ${GCC_BINARYDIR}")
endif()

if (APPLE)
  # The default gcc/g++ binary on OS X is symlinked to clang
  set(GCC_NAMES gcc-6 gcc-5 gcc-4 gcc-3)
  set(GXX_NAMES g++-6 g++-5 g++-4 g++-3)
else()
  set(GCC_NAMES gcc gcc-6 gcc-5 gcc-4 gcc-3)
  set(GXX_NAMES g++ g++-6 g++-5 g++-4 g++-3)
endif()

find_program(
  GCC_BINARY
  NAMES ${GCC_NAMES}
  HINTS ${GCC_BINARYDIR}
)
find_program(
  GXX_BINARY
  NAMES ${GXX_NAMES}
  HINTS ${GCC_BINARYDIR}
)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set GCC_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(GCC DEFAULT_MSG GCC_BINARY)
find_package_handle_standard_args(GXX DEFAULT_MSG GXX_BINARY)

# Done

if (GCC_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  if (GCC_FOUND)
    message(STATUS "gcc found")
    message(STATUS "  GCC_BINARY = ${GCC_BINARY}")
  else()
    message(STATUS "gcc not found")
  endif()
  if (GXX_FOUND)
    message(STATUS "g++ found")
    message(STATUS "  GXX_BINARY = ${GXX_BINARY}")
  else()
    message(STATUS "g++ not found")
  endif()
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
endif()

