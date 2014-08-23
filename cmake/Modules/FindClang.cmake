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
#  CLANG_INCLUDEDIR   Set this if the module can not find the Clang
#                     headers
#  CLANG_LIBRARYDIR   Set this if the module can not find the Clang
#                     library
#  CLANG_DEBUG        Set this for verbose output
#
# This module will define the following:
#   CLANG_FOUND
#   CLANG_INCLUDE_DIR
#   CLANG_LIBRARY
#   CLANG_DLL (only on Windows)
#   CLANG_HEADERS (the path to the headers used by clang. Only on Windows)

if (NOT $ENV{CLANG_INCLUDEDIR} STREQUAL "" )
  set(CLANG_INCLUDEDIR $ENV{CLANG_INCLUDEDIR})
endif()

if (NOT $ENV{CLANG_LIBRARYDIR} STREQUAL "" )
  set(CLANG_LIBRARYDIR $ENV{CLANG_LIBRARYDIR})
endif()

# Find clang-c on Ubuntu 13.10
foreach(V 3.4 3.3 3.2)
  set(CLANG_INCLUDEDIR "${CLANG_INCLUDEDIR};/usr/lib/llvm-${V}/include")
endforeach()

# Find clang-c on Windows
if (WIN32)
  set(
    CLANG_INCLUDEDIR
    "${CLANG_INCLUDEDIR};C:/Program Files (x86)/LLVM/include"
  )

  set(
    CLANG_LIBRARYDIR
    "${CLANG_LIBRARYDIR};C:/Program Files (x86)/LLVM/lib"
  )
  set(
    CLANG_LIBRARYDIR
    "${CLANG_LIBRARYDIR};C:/Program Files (x86)/LLVM/bin"
  )
endif()

if (CLANG_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  message(STATUS "Searching for libclang")
  message(STATUS "  CLANG_INCLUDEDIR = ${CLANG_INCLUDEDIR}")
endif()

find_path(CLANG_INCLUDE_DIR NAMES clang-c/Index.h HINTS ${CLANG_INCLUDEDIR})

set(CLANG_LIBRARYDIR "${CLANG_LIBRARYDIR};${CLANG_INCLUDE_DIR}/../lib")

if (CLANG_DEBUG)
  message(STATUS "  CLANG_LIBRARYDIR = ${CLANG_LIBRARYDIR}")
endif()

if (WIN32)
  # The import library of clang is called libclang.imp instead of libclang.lib
  find_file(CLANG_LIBRARY NAMES libclang.imp HINTS ${CLANG_LIBRARYDIR})
  find_file(CLANG_DLL NAMES libclang.dll HINTS ${CLANG_LIBRARYDIR})

else()
  find_library(CLANG_LIBRARY NAMES clang HINTS ${CLANG_LIBRARYDIR})
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set CLANG_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
  CLANG DEFAULT_MSG CLANG_LIBRARY CLANG_INCLUDE_DIR
)

# The standard headers on Windows
if (WIN32)
  set(CLANG_HEADER_ROOT ${CLANG_INCLUDE_DIR}/../lib/clang)
  file(
    GLOB
    CLANG_HEADER_DIRS
    RELATIVE ${CLANG_HEADER_ROOT}
    ${CLANG_HEADER_ROOT}/*
  )
  list(SORT CLANG_HEADER_DIRS)
  list(REVERSE CLANG_HEADER_DIRS)
  list(GET CLANG_HEADER_DIRS 0 CLANG_HEADER_GREATEST_VERSION)

  set(
    CLANG_HEADERS
    "${CLANG_HEADER_ROOT}/${CLANG_HEADER_GREATEST_VERSION}/include"
  )
endif()

mark_as_advanced(CLANG_INCLUDE_DIR, CLANG_LIBRARY, CLANG_DLL)

if (CLANG_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  if (CLANG_FOUND)
    message(STATUS "libclang found")
    message(STATUS "  CLANG_INCLUDE_DIR = ${CLANG_INCLUDE_DIR}")
    message(STATUS "  CLANG_LIBRARY = ${CLANG_LIBRARY}")
    if (WIN32)
      message(STATUS "  CLANG_DLL = ${CLANG_DLL}")
      message(STATUS "  CLANG_HEADERS = ${CLANG_HEADERS}")
    endif ()
  else()
    message(STATUS "libclang not found")
  endif()
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
endif()

