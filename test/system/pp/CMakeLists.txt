# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

project(metashell_pp_system_test)

include(MetashellSystemTest)
find_package(GCC)
find_package(MSVC)

aux_source_directory(. SOURCES)
add_executable(${PROJECT_NAME} ${SOURCES})

enable_warnings(${PROJECT_NAME})

target_link_libraries(
  ${PROJECT_NAME}

  boost::algorithm
  boost::filesystem
  boost::range

  googletest

  just::lines
  just::temp

  metashell::pattern
  metashell::system_test
)

if (NOT DEFINED TESTS OR TESTS MATCHES ".*_pp$")
  register_system_test(${PROJECT_NAME})
  register_gcc_system_test(${PROJECT_NAME})
  register_msvc_system_test(${PROJECT_NAME})
  register_wave_system_test(${PROJECT_NAME})
else()
  message(STATUS "Skipping pp system tests because TESTS=${TESTS}")
endif()
