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

function(disable_warnings)
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w" PARENT_SCOPE)
    message(STATUS "Turning off warnings for GCC")
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w" PARENT_SCOPE)
    message(STATUS "Turning off warnings for Clang")
  endif()
endfunction()

function(enable_warnings)
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(
      CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic"
      PARENT_SCOPE
    )
    message(STATUS "Turning on warnings for GCC")
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(
      CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic"
      PARENT_SCOPE
    )
    message(STATUS "Turning on warnings for Clang")
  endif()
endfunction()

function(use_cpp11)
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" PARENT_SCOPE)
    message(STATUS "Turning on C++11 for GCC")
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" PARENT_SCOPE)
    message(STATUS "Turning on C++11 for Clang")
  endif()
endfunction()

