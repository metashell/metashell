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

project(boost_regex)

aux_source_directory(src SOURCES)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_library(boost::regex ALIAS ${PROJECT_NAME})

disable_warnings(${PROJECT_NAME})

target_include_directories(
  ${PROJECT_NAME} SYSTEM
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)

if("x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xClang")
  target_compile_options(${PROJECT_NAME} PRIVATE -Wno-deprecated-register)
endif()

target_link_libraries(
  ${PROJECT_NAME}
  boost::assert
  boost::config
  boost::container_hash
  boost::integer
  boost::mpl
  boost::predef
  boost::smart_ptr
  boost::static_assert
  boost::throw_exception
  boost::type_traits
)

