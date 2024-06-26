# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

project(metashell_core_unit_test)

aux_source_directory(. SOURCES)
add_executable(${PROJECT_NAME} ${SOURCES})

enable_warnings(${PROJECT_NAME})

# Need to remove using `namespace metashell` from test code
set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD OFF)

target_link_libraries(
  ${PROJECT_NAME}

  googletest

  just::temp

  metashell::core
  metashell::engine::constant
  metashell::mock
)

add_test(metashell_core_unit_test ${PROJECT_NAME})

# Code coverage
if (ENABLE_CODE_COVERAGE)
  SETUP_TARGET_FOR_COVERAGE(
    test_coverage
    ${PROJECT_NAME}
    coverage
    "'/usr/*';'test/*';'3rd/*'"
  )
endif()

