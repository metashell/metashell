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

function(templight_path OUT_VAR)
  if (WIN32)
    set(${OUT_VAR} templight/templight.exe PARENT_SCOPE)
  else()
    set(${OUT_VAR} templight_metashell PARENT_SCOPE)
  endif()
endfunction()

function(register_system_test TEST_TARGET_NAME)
  templight_path(TEMPLIGHT_PATH)

  if (WIN32)
    set(
      CLANG_FLAGS
      -fno-ms-compatibility
      -U_MSC_VER
    )
  elseif (APPLE)
    set(
      CLANG_FLAGS
      "-idirafter$<TARGET_FILE_DIR:metashell>/../include/metashell/libcxx"
      "-idirafter$<TARGET_FILE_DIR:metashell>/../include/metashell/templight"
    )
  else()
    set(
      CLANG_FLAGS
      "-idirafter$<TARGET_FILE_DIR:metashell>/../include/metashell/templight"
    )
  endif()

  if (NOT DEFINED TESTS OR TESTS MATCHES "^internal.*")
    add_test(
      NAME ${TEST_TARGET_NAME}_internal_templight
      COMMAND
        ${TEST_TARGET_NAME} "$<TARGET_FILE:metashell>" --
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/config/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/mpl/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/preprocessor/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/type_traits/include"
        --
    )

    add_test(
      NAME ${TEST_TARGET_NAME}_auto_internal_templight
      COMMAND
        ${TEST_TARGET_NAME} "$<TARGET_FILE:metashell>" --engine auto --
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/config/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/mpl/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/preprocessor/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/type_traits/include"
        --
    )
  else()
    message(STATUS "Skipping internal system tests because TESTS=${TESTS}")
  endif()

  if (NOT DEFINED TESTS OR TESTS MATCHES "^templight.*")
    add_test(
      NAME ${TEST_TARGET_NAME}_templight
      COMMAND
        ${TEST_TARGET_NAME} "$<TARGET_FILE:metashell>" --engine templight --
        "$<TARGET_FILE_DIR:metashell>/${TEMPLIGHT_PATH}"
        -std=c++14
        -ftemplate-depth=256
        -Wfatal-errors
        ${CLANG_FLAGS}
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/config/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/mpl/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/preprocessor/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/type_traits/include"
        --
    )

    add_test(
      NAME ${TEST_TARGET_NAME}_auto_templight
      COMMAND
        ${TEST_TARGET_NAME} "$<TARGET_FILE:metashell>" --engine auto --
        "$<TARGET_FILE_DIR:metashell>/${TEMPLIGHT_PATH}"
        -std=c++14
        -ftemplate-depth=256
        -Wfatal-errors
        ${CLANG_FLAGS}
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/config/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/mpl/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/preprocessor/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/type_traits/include"
        --
    )
  else()
    message(STATUS "Skipping templight system tests because TESTS=${TESTS}")
  endif()

  if (NOT DEFINED TESTS OR TESTS MATCHES "^clang.*")
    add_test(
      NAME ${TEST_TARGET_NAME}_clang
      COMMAND
        ${TEST_TARGET_NAME} "$<TARGET_FILE:metashell>" --engine clang --
        "$<TARGET_FILE_DIR:metashell>/${TEMPLIGHT_PATH}"
        -std=c++14
        -ftemplate-depth=256
        -Wfatal-errors
        ${CLANG_FLAGS}
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/config/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/mpl/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/preprocessor/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/type_traits/include"
        --
    )

    add_test(
      NAME ${TEST_TARGET_NAME}_auto_clang
      COMMAND
        ${TEST_TARGET_NAME} "$<TARGET_FILE:metashell>" --engine auto --
        "$<TARGET_FILE_DIR:metashell>/${TEMPLIGHT_PATH}"
        -std=c++14
        -ftemplate-depth=256
        -Wfatal-errors
        ${CLANG_FLAGS}
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/config/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/mpl/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/preprocessor/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/type_traits/include"
        --
    )
  else()
    message(STATUS "Skipping clang system tests because TESTS=${TESTS}")
  endif()
endfunction()

function(register_gcc_system_test TEST_TARGET_NAME)
  if (NOT DEFINED TESTS OR TESTS MATCHES "^gcc.*")
    if (GXX_FOUND)
      message(STATUS "Using ${GXX_BINARY} for gcc system test")
      add_test(
        NAME ${TEST_TARGET_NAME}_gcc
        COMMAND
          ${TEST_TARGET_NAME} "$<TARGET_FILE:metashell>" --engine gcc --
          "${GXX_BINARY}"
          -std=c++14
          "-I${CMAKE_SOURCE_DIR}/3rd/boost/config/include"
          "-I${CMAKE_SOURCE_DIR}/3rd/boost/mpl/include"
          "-I${CMAKE_SOURCE_DIR}/3rd/boost/preprocessor/include"
          "-I${CMAKE_SOURCE_DIR}/3rd/boost/type_traits/include"
          --
      )

      add_test(
        NAME ${TEST_TARGET_NAME}_auto_gcc
        COMMAND
          ${TEST_TARGET_NAME} "$<TARGET_FILE:metashell>" --engine auto --
          "${GXX_BINARY}"
          -std=c++14
          "-I${CMAKE_SOURCE_DIR}/3rd/boost/config/include"
          "-I${CMAKE_SOURCE_DIR}/3rd/boost/mpl/include"
          "-I${CMAKE_SOURCE_DIR}/3rd/boost/preprocessor/include"
          "-I${CMAKE_SOURCE_DIR}/3rd/boost/type_traits/include"
          --
      )
    else()
      message(WARNING "Skipping gcc system test (g++ exectuable not found)")
    endif()
  else()
    message(STATUS "Skipping gcc system tests because TESTS=${TESTS}")
  endif()
endfunction()

function(register_msvc_system_test TEST_TARGET_NAME)
  if (WIN32)
    if (NOT DEFINED TESTS OR TESTS MATCHES "^msvc.*")
      if (MSVC_FOUND)
        message(STATUS "cl.exe used in system tests: ${MSVC_CL_BINARY}")

        add_test(
          NAME ${TEST_TARGET_NAME}_msvc
          COMMAND
            ${TEST_TARGET_NAME} "$<TARGET_FILE:metashell>" --engine msvc --
            "${MSVC_CL_BINARY}"
            "/I${CMAKE_SOURCE_DIR}\\3rd\\boost\\config\\include"
            "/I${CMAKE_SOURCE_DIR}\\3rd\\boost\\mpl\\include"
            "/I${CMAKE_SOURCE_DIR}\\3rd\\boost\\preprocessor\\include"
            "/I${CMAKE_SOURCE_DIR}\\3rd\\boost\\type_traits\\include"
            "/EHsc"
            --
        )

        add_test(
          NAME ${TEST_TARGET_NAME}_auto_msvc
          COMMAND
            ${TEST_TARGET_NAME} "$<TARGET_FILE:metashell>" --engine auto --
            "${MSVC_CL_BINARY}"
            "/I${CMAKE_SOURCE_DIR}\\3rd\\boost\\config\\include"
            "/I${CMAKE_SOURCE_DIR}\\3rd\\boost\\mpl\\include"
            "/I${CMAKE_SOURCE_DIR}\\3rd\\boost\\preprocessor\\include"
            "/I${CMAKE_SOURCE_DIR}\\3rd\\boost\\type_traits\\include"
            "/EHsc"
            --
        )
      else()
        message(WARNING "Skipping Visual C++ system test (cl.exe not found)")
      endif()
    else()
      message(STATUS "Skipping msvc system tests because TESTS=${TESTS}")
    endif()
  endif()
endfunction()

function(register_wave_system_test TEST_TARGET_NAME)
  if (NOT DEFINED TESTS OR TESTS MATCHES "^wave.*")
    add_test(
      NAME ${TEST_TARGET_NAME}_wave
      COMMAND
        ${TEST_TARGET_NAME} "$<TARGET_FILE:metashell>" --engine wave --
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/config/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/mpl/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/preprocessor/include"
        "-I${CMAKE_SOURCE_DIR}/3rd/boost/type_traits/include"
        --
    )
  else()
    message(STATUS "Skipping wave system tests because TESTS=${TESTS}")
  endif()
endfunction()

