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

project(boost_thread)

aux_source_directory(src SOURCES)

if (WIN32)
  aux_source_directory(src/win32 SOURCES_WIN32)
  list(APPEND SOURCES ${SOURCES_WIN32})
else()
  find_package(Threads)

  aux_source_directory(src/pthread SOURCES_PTHREAD)
  list(APPEND SOURCES ${SOURCES_PTHREAD})
endif()

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_library(boost::thread ALIAS ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PRIVATE BOOST_THREAD_BUILD_LIB)

# flag_states enum are defined in multiple TUs
set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD OFF)

target_link_libraries(
  ${PROJECT_NAME}
  
  boost::algorithm
  boost::atomic
  boost::bind
  boost::chrono
  boost::container_hash
  boost::core
  boost::date_time
  boost::exception
  boost::function
  boost::io
  boost::iterator
  boost::lexical_cast
  boost::move
  boost::mpl
  boost::optional
  boost::smart_ptr
  boost::static_assert
  boost::system
  boost::tuple
  boost::type_traits

  ${CMAKE_THREAD_LIBS_INIT}
)

disable_warnings(${PROJECT_NAME})

target_include_directories(
  ${PROJECT_NAME} SYSTEM
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)

boost_lib(PUBLIC ${PROJECT_NAME})
