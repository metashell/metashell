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
add_executable(reference_gen ${SOURCES})

enable_warnings(reference_gen)

# Metashell
target_link_libraries(reference_gen
  metashell_core_lib
  metashell_data_lib
  metashell_process_lib
  replace_part_lib
)

# Wave
target_link_libraries(reference_gen
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
target_link_libraries(reference_gen boost_program_options)

# Regex
target_link_libraries(reference_gen boost_regex)

# Just
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/just_file/include")

# yaml-cpp
target_link_libraries(reference_gen yaml_cpp_lib)

# Mpark.Variant
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/mpark_variant/include")


set(DOC_DIR "${CMAKE_SOURCE_DIR}/docs")

if(METASHELL_NO_DOC_GENERATION)
  add_custom_command(
    TARGET reference_gen
    POST_BUILD
    COMMENT "Generating documentation"
    COMMAND ${CMAKE_COMMAND} -E echo
      "Skipping documentation generation because METASHELL_NO_DOC_GENERATION is set"
  )
else()
  add_custom_command(
    TARGET reference_gen
    POST_BUILD
    COMMENT "Generating documentation"
    COMMAND reference_gen -d "${DOC_DIR}"
  )
endif()

add_test(
  NAME test_generated_doc
  COMMAND reference_gen -t -d "${DOC_DIR}"
)

