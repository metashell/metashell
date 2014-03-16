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
add_executable(metashell_test ${SOURCES})

target_link_libraries(metashell_test metashell_lib)

# Wave
target_link_libraries(metashell_test
  ${Boost_SYSTEM_LIBRARIES}
  ${Boost_THREAD_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARIES}
  ${Boost_WAVE_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

# Program_options
target_link_libraries(metashell_test ${Boost_PROGRAM_OPTIONS_LIBRARIES})

add_test(metashell_unit_tests metashell_test)

