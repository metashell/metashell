# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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
add_executable(metashell_pdb_system_test ${SOURCES})

enable_warnings(metashell_pdb_system_test)

target_link_libraries(metashell_pdb_system_test
  metashell_system_test_lib
  metashell_process_lib
  metashell_data_lib
  boost_regex)

# Gtest
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/googletest/include")
target_link_libraries(metashell_pdb_system_test googletest)

# Just
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/just_temp/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/just_lines/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/just_environment/include")

# Boost
target_link_libraries(metashell_pdb_system_test
  boost_filesystem
  boost_system
  boost_thread
  ${BOOST_ATOMIC_LIB}
)

register_wave_system_test(metashell_pdb_system_test)

