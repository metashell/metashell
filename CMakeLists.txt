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

cmake_minimum_required(VERSION 2.6)

project(metashell)
set(CMAKE_PROJECT_NAME metashell)
set(MAJOR_VERSION 4)
set(MINOR_VERSION 0)
set(PATCH_VERSION 0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Version macros
add_definitions(-DMETASHELL_MAJOR=${MAJOR_VERSION})
add_definitions(-DMETASHELL_MINOR=${MINOR_VERSION})
add_definitions(-DMETASHELL_PATCH=${PATCH_VERSION})

include(Platform)

# Windows workaround
if (WIN32)
  add_definitions( -DNOMINMAX )
endif()

# Warnings, C++11, etc
include(Compiler)

# Workaround on some systems (OSX + clang + Boost 1.55)
add_definitions(-DBOOST_ATOMIC_FORCE_FALLBACK=1)

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
set(TEMPLIGHT_BUILD "${CMAKE_SOURCE_DIR}/3rd/templight/build")

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

# Boost
if (WIN32)
  add_definitions(-DBOOST_ALL_NO_LIB=1)
else ()
  set(BOOST_ATOMIC_LIB boost_atomic)
  if (OPEN_BSD)
    add_definitions(-DBOOST_HAS_STDINT_H)
  elseif(NOT APPLE)
    find_package(Rt)
  endif()
endif ()

# Pthreads
if (NOT WIN32)
  find_package(Threads)
endif()

# Readline
if (NOT WIN32)
  if (USE_EDITLINE)
    find_package(Editline)
  else()
    find_package(Readline)
  endif()
endif()

include_directories("include")
include_directories(SYSTEM "3rd/boost/include")
include_directories(SYSTEM "3rd/rapidjson/include")
include_directories(SYSTEM "3rd/protobuf/include")
include_directories(SYSTEM "3rd/termcap/include")
include_directories(SYSTEM "3rd/yaml-cpp/include")

# Unit testing
enable_testing()

# Recursing
subdirs(lib app test 3rd tools)

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

  set(CPACK_COMPONENTS_ALL metashell windows_headers templight)
  set(CPACK_ALL_INSTALL_TYPES full notemplight minimal)

  set(CPACK_INSTALL_TYPE_FULL_DISPLAY_NAME "Full")
  set(CPACK_INSTALL_TYPE_NOTEMPLIGHT_DISPLAY_NAME "Without Templight")
  set(CPACK_INSTALL_TYPE_MINIMAL_DISPLAY_NAME "Minimal")

  set(CPACK_COMPONENT_METASHELL_DISPLAY_NAME "Metashell")
  set(CPACK_COMPONENT_METASHELL_DESCRIPTION "The Metashell binary")
  set(CPACK_COMPONENT_METASHELL_INSTALL_TYPES full notemplight minimal)

  set(CPACK_COMPONENT_WINDOWS_HEADERS_DISPLAY_NAME "C++ headers")
  set(CPACK_COMPONENT_WINDOWS_HEADERS_DESCRIPTION "The MinGW standard headers")
  set(CPACK_COMPONENT_WINDOWS_HEADERS_INSTALL_TYPES full notemplight)

  set(CPACK_COMPONENT_TEMPLIGHT_DISPLAY_NAME "Templight")
  set(CPACK_COMPONENT_TEMPLIGHT_DESCRIPTION "Templight")
  set(CPACK_COMPONENT_TEMPLIGHT_INSTALL_TYPES full)

  include(CPack)

endif(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")

