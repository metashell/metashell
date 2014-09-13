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
set(MAJOR_VERSION 1)
set(MINOR_VERSION 0)
set(PATCH_VERSION 0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Version macros
add_definitions(-DMETASHELL_MAJOR=${MAJOR_VERSION})
add_definitions(-DMETASHELL_MINOR=${MINOR_VERSION})
add_definitions(-DMETASHELL_PATCH=${PATCH_VERSION})

# Windows workaround
if (WIN32)
  add_definitions( -DNOMINMAX )
endif()

# C++11
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  message(STATUS "Turning on C++11 for GCC")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  message(STATUS "Turning on C++11 for Clang")
endif()

# Warnings
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
  message(STATUS "Turning on warnings for GCC")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
  message(STATUS "Turning on warnings for Clang")
endif()

# Boost
if (WIN32)
  set(Boost_USE_STATIC_LIBS ON)
endif()
find_package(Boost COMPONENTS
  system # used by Wave
  thread # used by Wave
  filesystem # used by Wave
  wave
  program_options
  regex
)
find_package(Threads) # used by Boost.Thread

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

set(
  BOOST_VER
  "${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}"
)

# Workaround on some systems (OSX + clang + Boost 1.55)
add_definitions(-DBOOST_ATOMIC_FORCE_FALLBACK=1)

if (NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

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

# Clang
find_package(Clang)
if (CLANG_STATIC)
  find_package(ZLIB)
endif()

if (NOT DEFINED CLANG_HEADER_INSTALL_PATH)
  set(CLANG_HEADER_INSTALL_PATH "include/metashell/clang")
endif ()


# Readline
if (WIN32)
  subdirs(wineditline)
else()
  if (USE_EDITLINE)
    find_package(Editline)
  else()
    find_package(Termcap)
    find_package(Readline)
  endif()
endif()

include_directories("include")

# Unit testing
enable_testing()

# Recursing
subdirs(lib app test)

# Debian package
set(CMAKE_INSTALL_PREFIX "/usr")

if(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
  include(InstallRequiredSystemLibraries)
   
if(WIN32)
  set(CPACK_GENERATOR "NSIS")
  set(
    CPACK_NSIS_MENU_LINKS
    "bin/metashell.exe" "Metashell"
    "https://github.com/sabel83/metashell#metashell" "Metashell website"
    "http://abel.web.elte.hu/shell/metashell.html" "Metashell online demo"
  )
else()
  set(CPACK_SET_DESTDIR "on")
  set(CPACK_PACKAGING_INSTALL_PREFIX "/tmp")
  set(CPACK_GENERATOR "DEB")
endif()
   
  set(CPACK_PACKAGE_DESCRIPTION "Metashell")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "It is an interactive C++ template metaprogramming shell")
  set(CPACK_PACKAGE_VENDOR "Abel Sinkovics")
  set(CPACK_PACKAGE_CONTACT "abel@sinkovics.hu")
  set(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
  set(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
  set(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
  set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}_${CMAKE_SYSTEM_PROCESSOR}")
  set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
  seT(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")

  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-system${BOOST_VER}, libboost-thread${BOOST_VER}, libboost-filesystem${BOOST_VER}, libboost-wave${BOOST_VER}, libboost-program-options${BOOST_VER}, libboost-regex${BOOST_VER}, libclang1, libclang-dev")
   
  set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
  set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
  set(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
   
  set(CPACK_COMPONENTS_ALL metashell windows_headers clang libclang)
  set(CPACK_ALL_INSTALL_TYPES full noclang minimal)

  set(CPACK_INSTALL_TYPE_FULL_DISPLAY_NAME "Full")
  set(CPACK_INSTALL_TYPE_NOLCANG_DISPLAY_NAME "Without Clang")
  set(CPACK_INSTALL_TYPE_MINIMAL_DISPLAY_NAME "Minimal")

  set(CPACK_COMPONENT_METASHELL_DISPLAY_NAME "Metashell")
  set(CPACK_COMPONENT_METASHELL_DESCRIPTION "The Metashell binary")
  set(CPACK_COMPONENT_METASHELL_INSTALL_TYPES full noclang minimal)

  set(CPACK_COMPONENT_WINDOWS_HEADERS_DISPLAY_NAME "C++ headers")
  set(CPACK_COMPONENT_WINDOWS_HEADERS_DESCRIPTION "The MinGW standard headers")
  set(CPACK_COMPONENT_WINDOWS_HEADERS_INSTALL_TYPES full noclang)

  set(CPACK_COMPONENT_LIBCLANG_DISPLAY_NAME "Libclang")
  set(CPACK_COMPONENT_LIBCLANG_DESCRIPTION "A Clang library to use when Clang is not installed on your computer. It is recommend to install Clang for better performance in Metashell.")
  set(CPACK_COMPONENT_LIBCLANG_INSTALL_TYPES full)

  set(CPACK_COMPONENT_CLANG_DISPLAY_NAME "Clang")
  set(CPACK_COMPONENT_CLANG_DESCRIPTION "The Clang compiler")
  set(CPACK_COMPONENT_CLANG_INSTALL_TYPES full)

  include(CPack)
   
endif(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")

