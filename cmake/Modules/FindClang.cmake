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
#  CLANG_BINARYDIR    Set this if the module can not find the clang binary
#  CLANG_DEBUG        Set this for verbose output
#  CLANG_STATIC       Link staticly against libClang (not supported on Windows)
#
# This module will define the following:
#   CLANG_FOUND
#   CLANG_INCLUDE_DIR
#   CLANG_LIBRARY
#   CLANG_DLL (only on Windows)
#   CLANG_HEADERS (the path to the headers used by clang)
#   CLANG_BINARY

if (NOT $ENV{CLANG_INCLUDEDIR} STREQUAL "" )
  set(CLANG_INCLUDEDIR $ENV{CLANG_INCLUDEDIR})
endif()

if (NOT $ENV{CLANG_LIBRARYDIR} STREQUAL "" )
  set(CLANG_LIBRARYDIR $ENV{CLANG_LIBRARYDIR})
endif()

# Find clang on some Ubuntu versions
foreach(V 3.5 3.4 3.3 3.2)
  set(CLANG_INCLUDEDIR "${CLANG_INCLUDEDIR};/usr/lib/llvm-${V}/include")
  set(CLANG_LIBRARYDIR "${CLANG_LIBRARYDIR};/usr/lib/llvm-${V}/lib")
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
  if (CLANG_STATIC)
    message(FATAL_ERROR "CLANG_STATIC is not supported on Windows")
  endif()
  # The import library of clang is called libclang.imp instead of libclang.lib
  find_file(CLANG_LIBRARY NAMES libclang.imp HINTS ${CLANG_LIBRARYDIR})
  find_file(CLANG_DLL NAMES libclang.dll HINTS ${CLANG_LIBRARYDIR})

else()
  if (CLANG_STATIC)
    if (CLANG_DEBUG)
      message(STATUS "Link against libClang staticly")
    endif()
    set(N 1)
    foreach(L
      libclang.a
      libclangFrontend.a
      libclangParse.a
      libclangSema.a
      libclangAnalysis.a
      libclangAST.a
      libclangEdit.a
      libclangLex.a
      libclangBasic.a
      libclangDriver.a
      libclangIndex.a
      libclangFormat.a
      libclangRewrite.a
      libclangSerialization.a
      libclangTooling.a
      libclangToolingCore.a
      libLLVMBitReader.a
      libLLVMTransformUtils.a
      libLLVMCore.a
      libLLVMMC.a
      libLLVMMCParser.a
      libLLVMOption.a
      libLLVMSupport.a
      dl
      tinfo
    )
      if (CLANG_DEBUG)
        message(STATUS "CLANG: searching ${L}")
      endif()
      find_library(CLANG_LIBRARY_${N} NAMES ${L} HINTS ${CLANG_LIBRARYDIR})
      if (CLANG_LIBRARY_${N})
        if (CLANG_DEBUG)
          message(STATUS "CLANG: found ${CLANG_LIBRARY_${N}}")
        endif()
        list(APPEND CLANG_LIBRARY ${CLANG_LIBRARY_${N}})
      else()
        if (CLANG_DEBUG)
          message(STATUS "CLANG: not found ${CLANG_LIBRARY_${N}}")
        endif()
      endif()
      math(EXPR N "${N} + 1")
    endforeach()
  else()
    find_library(CLANG_LIBRARY NAMES clang HINTS ${CLANG_LIBRARYDIR})
  endif()
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set CLANG_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
  CLANG DEFAULT_MSG CLANG_LIBRARY CLANG_INCLUDE_DIR
)

# The clang binary
if (WIN32)
  set(CLANG_BINARYDIR "${CLANG_BINARYDIR};${CLANG_LIBRARYDIR}")
endif ()
find_program(CLANG_BINARY clang HINTS ${CLANG_BINARYDIR})

# The standard Clang header files
file(WRITE "${PROJECT_BINARY_DIR}/empty.hpp" "")
execute_process(
  COMMAND ${CLANG_BINARY} -v -xc++ "${PROJECT_BINARY_DIR}/empty.hpp"
  ERROR_VARIABLE CLANG_OUTPUT
)
string(REGEX MATCHALL "[^\r\n]*[\r\n]" LINES "${CLANG_OUTPUT}")
foreach (L ${LINES})
  if ("${L}" MATCHES "^[ ][^\"]*$")
    string(STRIP "${L}" SL)
    list(APPEND CLANG_INCLUDE_PATH ${SL})
  endif()
endforeach()

find_path(CLANG_HEADERS NAMES altivec.h HINTS ${CLANG_INCLUDE_PATH})

# Done

mark_as_advanced(CLANG_INCLUDE_DIR CLANG_LIBRARY CLANG_DLL)

if (CLANG_DEBUG)
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
  if (CLANG_FOUND)
    message(STATUS "libclang found")
    message(STATUS "  CLANG_INCLUDE_DIR = ${CLANG_INCLUDE_DIR}")
    message(STATUS "  CLANG_LIBRARY = ${CLANG_LIBRARY}")
    message(STATUS "  CLANG_BINARY = ${CLANG_BINARY}")
    message(STATUS "  CLANG_HEADERS = ${CLANG_HEADERS}")
    if (WIN32)
      message(STATUS "  CLANG_DLL = ${CLANG_DLL}")
    endif ()
  else()
    message(STATUS "libclang not found")
  endif()
  message(STATUS "[${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}]")
endif()

