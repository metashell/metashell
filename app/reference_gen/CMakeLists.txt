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

enable_warnings()
use_cpp11()

# Metashell
target_link_libraries(reference_gen
  metashell_core_lib
  metashell_data_lib
  metashell_process_lib
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


function(register_doc_test MD_FILE MARKER ARG)
  set(INPUT ${CMAKE_SOURCE_DIR}/docs/${MD_FILE})
  set(TEMP ${CMAKE_BINARY_DIR}/ref_${MARKER}.md)
  set(EXPECTED ${CMAKE_BINARY_DIR}/expected_${MARKER}.md)

  if (WIN32)
    set(MARKER_ARG "^<!-- ${MARKER} --^>")
  else()
    set(MARKER_ARG '<!-- ${MARKER} -->')
  endif()

  add_custom_command(
    TARGET reference_gen
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo
      "Generating expected documentation for '<!-- ${MARKER} -->' in ${MD_FILE}"
    COMMAND reference_gen ${ARG} -o "${TEMP}"
    COMMAND replace_part
      -i "${INPUT}"
      -m ${MARKER_ARG}
      -r "${TEMP}"
      -o "${EXPECTED}"
  )
  
  add_test(
    NAME test_generated_doc_${MARKER}
    COMMAND ${CMAKE_COMMAND} -E compare_files "${EXPECTED}" "${INPUT}"
  )
endfunction()


register_doc_test(reference/pragmas.md      pragma_info    --show_pragma_help)
register_doc_test(reference/mdb_commands.md mdb_info       --show_mdb_help)
register_doc_test(reference/pdb_commands.md pdb_info       --show_pdb_help)
register_doc_test(reference/engines.md      feature_info   --show_feature_help)
register_doc_test(reference/engines.md      engine_info    --show_engine_help)
register_doc_test(reference/engines.md      feature_matrix --show_engine_features)

