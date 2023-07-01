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

project(boost_atomic)

if (WIN32)
  add_library(${PROJECT_NAME} INTERFACE)
  set(LIB_TYPE INTERFACE)
else()
  aux_source_directory(src SOURCES)
  list(REMOVE_ITEM SOURCES "src/wait_on_address.cpp")
  add_library(${PROJECT_NAME} STATIC ${SOURCES})
  set(LIB_TYPE PUBLIC)

  target_compile_options(${PROJECT_NAME} PUBLIC "-msse4.1")
endif()

add_library(boost::atomic ALIAS ${PROJECT_NAME})

disable_warnings(${PROJECT_NAME})

target_include_directories(
  ${PROJECT_NAME} SYSTEM
  ${LIB_TYPE} ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(
  ${PROJECT_NAME}
  ${LIB_TYPE} ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(
  ${PROJECT_NAME}
  
  ${LIB_TYPE}
    boost::align
    boost::assert
    boost::config
    boost::predef
    boost::preprocessor
    boost::static_assert
    boost::type_traits
)

boost_lib(${LIB_TYPE} ${PROJECT_NAME})
