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

include(MetashellSystemTest)

find_package(GCC)
find_package(MSVC)

add_subdirectory(self)

set(SYSTEM_TEST_TARGETS )
set(SYSTEM_TEST_FILES )
foreach(T core pp mdb pdb)
  add_subdirectory(${T})

  set(TARGET metashell_${T}_system_test)

  list(APPEND SYSTEM_TEST_TARGETS ${TARGET})
  list(APPEND SYSTEM_TEST_FILES "$<TARGET_FILE:${TARGET}>")
endforeach(T)

set(
  SYSTEM_TEST_ARCHIVE
  "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}_system_test_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}${METASHELL_PACKAGE_PLATFORM}_${CMAKE_SYSTEM_PROCESSOR}.tar.gz"
)

add_custom_target(
  system_test_zip DEPENDS ${SYSTEM_TEST_TARGETS}
  COMMAND
    ${CMAKE_COMMAND} -E
    tar "cfz" "${SYSTEM_TEST_ARCHIVE}" ${SYSTEM_TEST_FILES}
)

