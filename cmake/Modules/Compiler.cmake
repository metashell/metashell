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

function(disable_warnings TARGET)
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${TARGET} PRIVATE -w)
    message(STATUS "Turning off warnings for GCC")
  elseif("x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xClang")
    target_compile_options(${TARGET} PRIVATE -w)
    message(STATUS "Turning off warnings for Clang")
  endif()
endfunction()

function(enable_warnings TARGET)
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${TARGET} PRIVATE -Wall -Wextra)
    message(STATUS "Turning on warnings for GCC")
  elseif("x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xClang")
    target_compile_options(${TARGET} PRIVATE -Wall -Wextra -pedantic)
    message(STATUS "Turning on warnings for Clang")
  endif()
endfunction()

