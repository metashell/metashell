# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

project(config_tester)

aux_source_directory(. SOURCES)
add_executable(${PROJECT_NAME} ${SOURCES})

enable_warnings(${PROJECT_NAME})

target_link_libraries(
  ${PROJECT_NAME}

  metashell::system_test
)

if ((NOT DEFINED TESTS OR TESTS STREQUAL "internal") AND NOT WIN32)
  add_test(
    NAME config_test
    COMMAND
      ${PROJECT_NAME} "$<TARGET_FILE:metashell>"
      --preprocessor
      --load_compile_commands "${CMAKE_BINARY_DIR}/compile_commands.json"
  )
endif()
