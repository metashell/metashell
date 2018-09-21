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
add_executable(metashell_unit_test ${SOURCES})

enable_warnings(metashell_unit_test)

target_link_libraries(metashell_unit_test
  metashell_core_lib
  metashell_data_lib
  metashell_process_lib
  replace_part_lib
)

# Wave
target_link_libraries(metashell_unit_test
  boost_system
  boost_thread
  ${BOOST_ATOMIC_LIB}
  boost_filesystem
  boost_wave
  ${CMAKE_THREAD_LIBS_INIT}
  ${RT_LIBRARY}
  ${PROTOBUF_LIBRARY}
  protobuf
)

# Program_options
target_link_libraries(metashell_unit_test boost_program_options)

# Regex
target_link_libraries(metashell_unit_test boost_regex)

# Gtest
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/googletest/include")
target_link_libraries(metashell_unit_test googletest)

# Gmock
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/googlemock/include")
target_link_libraries(metashell_unit_test googlemock)

# Just
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/just_environment/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/just_temp/include")

# yaml-cpp
target_link_libraries(metashell_unit_test yaml_cpp_lib)

# Mpark.Variant
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/mpark_variant/include")

add_test(metashell_unit_tests metashell_unit_test)

# Code coverage
if (ENABLE_CODE_COVERAGE)
  SETUP_TARGET_FOR_COVERAGE(
    test_coverage
    metashell_unit_test
    coverage
    "'/usr/*';'test/*';'3rd/*'"
  )
endif()

if (TEMPLIGHT_FOUND)
  #########################################################
  # Copying files next to the Metashell binary on Windows #
  #########################################################
  include(MetashellTemplight)
  
  copy_templight_next_to_binary(false)
endif ()

