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

cmake_minimum_required(VERSION 3.1)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
  cmake_policy(VERSION 3.12)
endif()

set(SYSTEM_TESTS internal templight clang gcc msvc wave)
set(VALID_TESTS ${SYSTEM_TESTS} unit auto)
if (DEFINED TESTS AND NOT (TESTS IN_LIST VALID_TESTS))
  message(
    FATAL_ERROR
    "Invalid TESTS value: ${TESTS}."
    " It should be one of the following: ${VALID_TESTS}"
  )
endif()

project(metashell_project)

set(MAJOR_VERSION 4)
set(MINOR_VERSION 0)
set(PATCH_VERSION 0)

if (MSVC)
  add_compile_options("/std:c++latest")
else()
  set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(Platform)
include(MetashellBoost)

# Warnings, C++11, etc
include(Compiler)

# Sanitizers
if (ENABLE_SANITIZERS)
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Enabling GCC's address sanitizer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(STATUS "Enabling Clang's address sanitizer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  endif()
endif()

# Code coverage
if (ENABLE_CODE_COVERAGE)
  include(CodeCoverage)

  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage")
  endif()
endif()

# Profiling
if (ENABLE_PROFILING)
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
  endif()
endif()

# The "internal" Templight build
set(TEMPLIGHT_BUILD "${CMAKE_BINARY_DIR}/../templight")

set(
  TEMPLIGHT_INCLUDE_PATH
  "${TEMPLIGHT_INCLUDE_PATH};${CMAKE_SOURCE_DIR}/3rd/templight/llvm/tools/clang/lib/Headers"
)
set(
  TEMPLIGHT_INCLUDEDIR
  "${TEMPLIGHT_INCLUDEDIR};${CMAKE_SOURCE_DIR}/3rd/templight/llvm/tools/clang/include"
)
set(
  TEMPLIGHT_BINARYDIR
  "${TEMPLIGHT_BINARYDIR};${TEMPLIGHT_BUILD}/Release+Asserts/bin"
)
set(
  TEMPLIGHT_BINARYDIR
  "${TEMPLIGHT_BINARYDIR};${TEMPLIGHT_BUILD}/Release/bin"
)
set(
  TEMPLIGHT_BINARYDIR
  "${TEMPLIGHT_BINARYDIR};${TEMPLIGHT_BUILD}/bin"
)

# Templight
find_package(Templight)

if (NOT DEFINED TEMPLIGHT_HEADER_INSTALL_PATH)
  set(TEMPLIGHT_HEADER_INSTALL_PATH "include/metashell/templight")
endif ()

if (NOT DEFINED LIBCXX_HEADER_INSTALL_PATH)
  set(LIBCXX_HEADER_INSTALL_PATH "include/metashell/libcxx")
endif ()

enable_testing()

add_subdirectory(app/metashell)
add_subdirectory(app/reference_gen)
add_subdirectory(app/stub/apple_clang)
add_subdirectory(app/stub/clang)
add_subdirectory(app/stub/cl_de)
add_subdirectory(app/stub/cl_en)
add_subdirectory(app/stub/gcc)
add_subdirectory(app/config_tester)

if (NOT DEFINED TESTS OR TESTS STREQUAL "auto")
  add_subdirectory(test/system/auto_engine)
else()
  message(STATUS "Skipping auto system tests because TESTS=${TESTS}")
endif()

if (NOT DEFINED TESTS OR TESTS IN_LIST SYSTEM_TESTS)
  add_subdirectory(test/system/core)
  add_subdirectory(test/system/mdb)
  add_subdirectory(test/system/pdb)
  add_subdirectory(test/system/pp)
else()
  message(STATUS "Skipping system tests because TESTS=${TESTS}")
endif()

if (NOT DEFINED TESTS OR TESTS STREQUAL "unit")
  add_subdirectory(test/unit/core)
  add_subdirectory(test/unit/data)
  add_subdirectory(test/unit/engine/clang)
  add_subdirectory(test/unit/engine/templight)
  add_subdirectory(test/unit/engine/vc)
  add_subdirectory(test/unit/main_shell)
  add_subdirectory(test/unit/mdb)
  add_subdirectory(test/unit/pragma)
  add_subdirectory(test/unit/process)
  add_subdirectory(test/unit/replace_part)
  add_subdirectory(test/unit/system_test)
else()
  message(STATUS "Skipping unit tests because TESTS=${TESTS}")
endif()

add_subdirectory(lib/compiler_stub)
add_subdirectory(lib/core)
add_subdirectory(lib/data)
add_subdirectory(lib/defaults)
add_subdirectory(lib/engine/clang)
add_subdirectory(lib/engine/constant)
add_subdirectory(lib/engine/gcc)
add_subdirectory(lib/engine/null)
add_subdirectory(lib/engine/templight)
add_subdirectory(lib/engine/vc)
add_subdirectory(lib/engine/wave)
add_subdirectory(lib/iface)
add_subdirectory(lib/main_shell)
add_subdirectory(lib/mdb)
add_subdirectory(lib/mock)
add_subdirectory(lib/pattern)
add_subdirectory(lib/pragma)
add_subdirectory(lib/process)
add_subdirectory(lib/readline)
add_subdirectory(lib/replace_part)
add_subdirectory(lib/system_test)

add_subdirectory(3rd/boost/algorithm)
add_subdirectory(3rd/boost/align)
add_subdirectory(3rd/boost/any)
add_subdirectory(3rd/boost/array)
add_subdirectory(3rd/boost/assert)
add_subdirectory(3rd/boost/assign)
add_subdirectory(3rd/boost/atomic)
add_subdirectory(3rd/boost/bind)
add_subdirectory(3rd/boost/chrono)
add_subdirectory(3rd/boost/concept_check)
add_subdirectory(3rd/boost/config)
add_subdirectory(3rd/boost/container)
add_subdirectory(3rd/boost/container_hash)
add_subdirectory(3rd/boost/conversion)
add_subdirectory(3rd/boost/core)
add_subdirectory(3rd/boost/date_time)
add_subdirectory(3rd/boost/detail)
add_subdirectory(3rd/boost/exception)
add_subdirectory(3rd/boost/filesystem)
add_subdirectory(3rd/boost/foreach)
add_subdirectory(3rd/boost/function)
add_subdirectory(3rd/boost/functional)
add_subdirectory(3rd/boost/function_types)
add_subdirectory(3rd/boost/fusion)
add_subdirectory(3rd/boost/graph)
add_subdirectory(3rd/boost/integer)
add_subdirectory(3rd/boost/intrusive)
add_subdirectory(3rd/boost/io)
add_subdirectory(3rd/boost/iostreams)
add_subdirectory(3rd/boost/iterator)
add_subdirectory(3rd/boost/lexical_cast)
add_subdirectory(3rd/boost/math)
add_subdirectory(3rd/boost/move)
add_subdirectory(3rd/boost/mpl)
add_subdirectory(3rd/boost/multi_index)
add_subdirectory(3rd/boost/numeric_conversion)
add_subdirectory(3rd/boost/optional)
add_subdirectory(3rd/boost/phoenix)
add_subdirectory(3rd/boost/pool)
add_subdirectory(3rd/boost/predef)
add_subdirectory(3rd/boost/preprocessor)
add_subdirectory(3rd/boost/property_map)
add_subdirectory(3rd/boost/proto)
add_subdirectory(3rd/boost/range)
add_subdirectory(3rd/boost/ratio)
add_subdirectory(3rd/boost/regex)
add_subdirectory(3rd/boost/serialization)
add_subdirectory(3rd/boost/smart_ptr)
add_subdirectory(3rd/boost/spirit)
add_subdirectory(3rd/boost/static_assert)
add_subdirectory(3rd/boost/system)
add_subdirectory(3rd/boost/thread)
add_subdirectory(3rd/boost/throw_exception)
add_subdirectory(3rd/boost/tokenizer)
add_subdirectory(3rd/boost/tuple)
add_subdirectory(3rd/boost/type_index)
add_subdirectory(3rd/boost/typeof)
add_subdirectory(3rd/boost/type_traits)
add_subdirectory(3rd/boost/unordered)
add_subdirectory(3rd/boost/utility)
add_subdirectory(3rd/boost/variant)
add_subdirectory(3rd/boost/wave)
add_subdirectory(3rd/boost/winapi)

add_subdirectory(3rd/googlemock)
add_subdirectory(3rd/googletest)

add_subdirectory(3rd/just/console)
add_subdirectory(3rd/just/environment)
add_subdirectory(3rd/just/file)
add_subdirectory(3rd/just/lines)
add_subdirectory(3rd/just/temp)

add_subdirectory(3rd/mindent)
add_subdirectory(3rd/mpark_variant)
add_subdirectory(3rd/protobuf)
add_subdirectory(3rd/rapidjson)
add_subdirectory(3rd/yaml-cpp)

if (WIN32)
  add_subdirectory(3rd/wineditline)
else()
  add_subdirectory(3rd/termcap)
endif()

target_compile_definitions(
  metashell_core
  
  PUBLIC
    METASHELL_MAJOR=${MAJOR_VERSION}
    METASHELL_MINOR=${MINOR_VERSION}
    METASHELL_PATCH=${PATCH_VERSION}
)

if (NOT DEFINED TESTS OR TESTS IN_LIST SYSTEM_TESTS)
  set(SYSTEM_TEST_TARGETS )
  set(SYSTEM_TEST_FILES )
  foreach(T core pp mdb pdb)
    set(TARGET metashell_${T}_system_test)

    list(APPEND SYSTEM_TEST_TARGETS ${TARGET})
    list(APPEND SYSTEM_TEST_FILES "$<TARGET_FILE:${TARGET}>")
  endforeach(T)

  set(
    SYSTEM_TEST_ARCHIVE
    "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}_system_test_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}${METASHELL_PACKAGE_PLATFORM}_${CMAKE_SYSTEM_PROCESSOR}.tar.gz"
  )

  add_custom_target(
    system_test_zip DEPENDS ${SYSTEM_TEST_TARGETS}
    COMMAND
      ${CMAKE_COMMAND} -E
      tar "cfz" "${SYSTEM_TEST_ARCHIVE}" ${SYSTEM_TEST_FILES}
  )
else()
  add_custom_target(
    system_test_zip
    COMMAND echo "Skipping system_test_zip because TESTS=${TESTS}"
  )

  message(
    STATUS
    "Skipping system test archive generation because TESTS=${TESTS}"
  )
endif()

# Recursing
subdirs(tools)

# Debian package
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  if (APPLE)
    # This argument is needed, so the generated archive will not have
    # /usr/local prefix for all files
    set(CMAKE_INSTALL_PREFIX ".")
  else()
    set(CMAKE_INSTALL_PREFIX "/usr")
  endif()
endif()

if(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
  include(InstallRequiredSystemLibraries)

  if(WIN32)
    set(
      CPACK_NSIS_MENU_LINKS
      "bin/metashell.exe" "Metashell"
      "http://metashell.org" "Metashell website"
      "http://metashell.org/about/demo" "Metashell online demo"
    )
  endif()

  if (DEFINED PLATFORM_CPACK_GENERATOR)
    set(CPACK_GENERATOR "${PLATFORM_CPACK_GENERATOR}")
  endif ()

  if (PLATFORM_NAME AND NOT WIN32)
    set(METASHELL_PACKAGE_PLATFORM "_${PLATFORM_NAME}${PLATFORM_VERSION}")
  else()
    set(METASHELL_PACKAGE_PLATFORM "")
  endif()

  set(CPACK_PACKAGE_DESCRIPTION "Metashell")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "It is an interactive C++ template metaprogramming shell")
  set(CPACK_PACKAGE_VENDOR "Abel Sinkovics")
  set(CPACK_PACKAGE_CONTACT "Abel Sinkovics <abel@sinkovics.hu>")
  set(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
  set(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
  set(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
  set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}${METASHELL_PACKAGE_PLATFORM}_${CMAKE_SYSTEM_PROCESSOR}")
  set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
  seT(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")

  set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
  set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
  set(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})

  if (SUSE_LINUX)
    set(CPACK_RPM_PACKAGE_REQUIRES "libstdc++-devel, glibc-devel")
  elseif (FEDORA_LINUX)
    set(
      CPACK_RPM_PACKAGE_REQUIRES
      "libstdc++-devel, glibc-headers, glibc-devel"
    )
  elseif (UBUNTU_LINUX)
    if ("${PLATFORM_VERSION}" STREQUAL "14.04")
      set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++-4.8-dev")
    elseif (
      ("${PLATFORM_VERSION}" STREQUAL "14.10")
      OR ("${PLATFORM_VERSION}" STREQUAL "15.04")
    )
      set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++-4.9-dev")
    elseif ("${PLATFORM_VERSION}" STREQUAL "16.04")
      set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++-5-dev")
    elseif ("${PLATFORM_VERSION}" STREQUAL "18.04")
      set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++-7-dev")
    else()
      message(
        WARNING
        "Unsupported Ubuntu version. Package dependencies will be invalid."
      )
    endif()
    set(CPACK_SET_DESTDIR "on")
    set(CPACK_PACKAGING_INSTALL_PREFIX "/tmp")
  elseif (DEBIAN_LINUX)
    if (
      ("${PLATFORM_VERSION}" STREQUAL "7.6")
      OR ("${PLATFORM_VERSION}" STREQUAL "7.7")
    )
      set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++6-4.7-dev")
    elseif (
      ("${PLATFORM_VERSION}" STREQUAL "8.1")
      OR ("${PLATFORM_VERSION}" STREQUAL "8.5")
    )
      set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++-4.9-dev")
    elseif (
      ("${PLATFORM_VERSION}" STREQUAL "9.5")
    )
      set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++-6-dev")
    else()
      message(
        WARNING
        "Unsupported Debian version. Package dependencies will be invalid."
      )
    endif()
    set(CPACK_SET_DESTDIR "on")
    set(CPACK_PACKAGING_INSTALL_PREFIX "/tmp")
  endif ()

  set(CPACK_COMPONENTS_ALL metashell templight)
  set(CPACK_ALL_INSTALL_TYPES full notemplight minimal)

  set(CPACK_INSTALL_TYPE_FULL_DISPLAY_NAME "Full")
  set(CPACK_INSTALL_TYPE_NOTEMPLIGHT_DISPLAY_NAME "Without Templight")
  set(CPACK_INSTALL_TYPE_MINIMAL_DISPLAY_NAME "Minimal")

  set(CPACK_COMPONENT_METASHELL_DISPLAY_NAME "Metashell")
  set(CPACK_COMPONENT_METASHELL_DESCRIPTION "The Metashell binary")
  set(CPACK_COMPONENT_METASHELL_INSTALL_TYPES full notemplight minimal)

  set(CPACK_COMPONENT_TEMPLIGHT_DISPLAY_NAME "Templight")
  set(CPACK_COMPONENT_TEMPLIGHT_DESCRIPTION "Templight")
  set(CPACK_COMPONENT_TEMPLIGHT_INSTALL_TYPES full)

  include(CPack)

endif(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")

