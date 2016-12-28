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

function(register_system_test TEST_TARGET_NAME)
  if (WIN32)
    set(TEMPLIGHT_PATH templight/templight.exe)
    set(
      CLANG_FLAGS
      -fno-ms-compatibility
      -U_MSC_VER
      "-I$<TARGET_FILE_DIR:metashell>/windows_headers"
      "-I$<TARGET_FILE_DIR:metashell>/windows_headers/mingw32"
      "-I$<TARGET_FILE_DIR:metashell>/templight/include"
    )
  else()
    set(TEMPLIGHT_PATH templight_metashell)
    if (APPLE)
      set(
        CLANG_FLAGS
        "-I$<TARGET_FILE_DIR:metashell>/../include/metashell/templight"
        "-I$<TARGET_FILE_DIR:metashell>/../include/metashell/libcxx"
      )
    else()
      set(
        CLANG_FLAGS
        "-I$<TARGET_FILE_DIR:metashell>/../include/metashell/templight"
      )
    endif()
  endif()

  add_test(
    NAME ${TEST_TARGET_NAME}_templight
    COMMAND
      ${TEST_TARGET_NAME} "$<TARGET_FILE:metashell>" --
      "-I${CMAKE_SOURCE_DIR}/3rd/boost/include"
      --
  )

  add_test(
    NAME ${TEST_TARGET_NAME}_clang
    COMMAND
      ${TEST_TARGET_NAME} "$<TARGET_FILE:metashell>" --engine clang --
      "$<TARGET_FILE_DIR:metashell>/${TEMPLIGHT_PATH}"
      -std=c++0x
      -ftemplate-depth=256
      -Wfatal-errors
      ${CLANG_FLAGS}
      "-I${CMAKE_SOURCE_DIR}/3rd/boost/include"
      --
  )
endfunction()

function(register_gcc_system_test TEST_TARGET_NAME)
  if (APPLE)
    find_file(GXX_PATH NAMES g++-6 g++-5)
    if (NOT GXX_PATH)
      message(WARNING "Skipping gcc system test (g++ exectuable not found)")
    endif()
  elseif (NOT WIN32)
    set(GXX_PATH "/usr/bin/g++")
  endif()

  if (GXX_PATH)
    message(STATUS "Using ${GXX_PATH} for gcc system test")
    add_test(
      NAME ${TEST_TARGET_NAME}_gcc
      COMMAND
        ${TEST_TARGET_NAME} "$<TARGET_FILE:metashell>" --engine gcc --
        ${GXX_PATH}
        -std=c++0x
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/include"
        --
    )
  endif()
endfunction()

