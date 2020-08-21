# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

project(metashell_auto_engine_system_test)

include(MetashellSystemTest)

aux_source_directory(. SOURCES)
add_executable(${PROJECT_NAME} ${SOURCES})

enable_warnings(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON)

target_link_libraries(
  ${PROJECT_NAME}

  boost::filesystem

  metashell::process
)

templight_path(TEMPLIGHT_PATH)

add_test(
  NAME
    metashell_auto_engine_system_test

  COMMAND
    ${PROJECT_NAME} --metashell "$<TARGET_FILE:metashell>"
    --clang "$<TARGET_FILE:app_stub_clang>"
    --clang "$<TARGET_FILE:app_stub_apple_clang>"
    --templight "$<TARGET_FILE_DIR:metashell>/${TEMPLIGHT_PATH}"
    --gcc "$<TARGET_FILE:app_stub_gcc>"
    --msvc "$<TARGET_FILE:app_stub_cl_en>"
    --msvc "$<TARGET_FILE:app_stub_cl_de>"
)
