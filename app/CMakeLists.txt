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

aux_source_directory(. SOURCES)
add_executable(metashell ${SOURCES})

# Metashell
target_link_libraries(metashell metashell_lib)

# Wave
target_link_libraries(metashell
  ${Boost_SYSTEM_LIBRARIES}
  ${Boost_THREAD_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARIES}
  ${Boost_WAVE_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

# Readline
include_directories(${READLINE_INCLUDE_DIR})
target_link_libraries(metashell ${READLINE_LIBRARY})

# Program_options
target_link_libraries(metashell ${Boost_PROGRAM_OPTIONS_LIBRARIES})

# Clang
target_link_libraries(metashell ${CLANG_LIBRARY})
include_directories(${CLANG_INCLUDE_DIR})

install(TARGETS metashell DESTINATION bin)

