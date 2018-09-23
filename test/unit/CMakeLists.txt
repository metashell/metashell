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

project(metashell_unit_test)

aux_source_directory(. SOURCES)
add_executable(${PROJECT_NAME} ${SOURCES})

enable_warnings(${PROJECT_NAME})

target_link_libraries(
  ${PROJECT_NAME}

  boost::atomic
  boost::filesystem
  boost::program_options
  boost::regex
  boost::system
  boost::thread
  boost::wave

  googletest
  googlemock

  just::environment
  just::temp

  metashell::core
  metashell::process
  metashell::replace_part

  mpark::variant

  protobuf
  yaml_cpp

  ${CMAKE_THREAD_LIBS_INIT}
  ${RT_LIBRARY}
  ${PROTOBUF_LIBRARY}
)

add_test(metashell_unit_tests ${PROJECT_NAME})

# Code coverage
if (ENABLE_CODE_COVERAGE)
  SETUP_TARGET_FOR_COVERAGE(
    test_coverage
    ${PROJECT_NAME}
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

