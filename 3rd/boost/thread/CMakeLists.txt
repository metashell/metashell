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

aux_source_directory(src SOURCES)

add_definitions(-DBOOST_THREAD_BUILD_LIB)

if (WIN32)
  aux_source_directory(src/win32 SOURCES_WIN32)
  list(APPEND SOURCES ${SOURCES_WIN32})
else()
  aux_source_directory(src/pthread SOURCES_PTHREAD)
  list(APPEND SOURCES ${SOURCES_PTHREAD})
endif()

add_library(boost_thread STATIC ${SOURCES})
target_link_libraries(boost_thread ${CMAKE_THREAD_LIBS_INIT})

disable_warnings(boost_thread)

