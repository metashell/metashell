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

project(reference_gen)

aux_source_directory(. SOURCES)
add_executable(${PROJECT_NAME} ${SOURCES})

enable_warnings(${PROJECT_NAME})

target_link_libraries(
  ${PROJECT_NAME}

  boost::algorithm
  boost::filesystem
  boost::range

  just::file

  metashell::core
  metashell::data
  metashell::defaults
  metashell::mdb
  metashell::replace_part
)

set(DOC_DIR "${CMAKE_SOURCE_DIR}/docs")

if(METASHELL_NO_DOC_GENERATION)
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMENT "Generating documentation"
    COMMAND ${CMAKE_COMMAND} -E echo
      "Skipping documentation generation because METASHELL_NO_DOC_GENERATION is set"
  )
else()
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMENT "Generating documentation"
    COMMAND ${PROJECT_NAME} -d "${DOC_DIR}"
  )
endif()

if (NOT DEFINED TESTS OR TESTS STREQUAL "unit")
  add_test(
    NAME test_generated_doc
    COMMAND ${PROJECT_NAME} -t -d "${DOC_DIR}"
  )
else()
  message(STATUS "Skipping generated doc tests because TESTS=${TESTS}")
endif()
