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

project(boost_program_options)

aux_source_directory(src SOURCES)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_library(boost::program_options ALIAS ${PROJECT_NAME})

disable_warnings(${PROJECT_NAME})

target_include_directories(
  ${PROJECT_NAME} SYSTEM
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
  ${PROJECT_NAME}
  
  boost::any
  boost::assert
  boost::bind
  boost::core
  boost::config
  boost::detail
  boost::function
  boost::iterator
  boost::lexical_cast
  boost::range
  boost::smart_ptr
  boost::throw_exception
  boost::tokenizer
)

set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON)

boost_lib(PUBLIC ${PROJECT_NAME})
