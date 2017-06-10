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
add_executable(metashell_system_test_self_test ${SOURCES})

enable_warnings()
use_cpp11()

target_link_libraries(metashell_system_test_self_test
  metashell_system_test_lib
)

# Gtest
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/googletest/include")
target_link_libraries(metashell_system_test_self_test googletest)

# Boost
target_link_libraries(metashell_system_test_self_test
  boost_filesystem
  boost_system
  boost_thread
  boost_regex
  ${BOOST_ATOMIC_LIB}
)

add_test(metashell_system_test_self_test metashell_system_test_self_test)

# Code coverage
if (ENABLE_CODE_COVERAGE)
  SETUP_TARGET_FOR_COVERAGE(
    test_coverage
    metashell_system_test_self_test
    coverage
    "'/usr/*';'test/*';'3rd/*'"
  )
endif()
