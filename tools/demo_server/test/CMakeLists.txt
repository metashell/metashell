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

function(register_demo_server_test TEST_SCRIPT)
  if (UBUNTU_LINUX)
    add_test(
      NAME demo_server_${TEST_SCRIPT}
      COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_SCRIPT}
    )
  endif()
endfunction()

if (NOT DEFINED TESTS OR TESTS STREQUAL "unit")
  register_demo_server_test(test_git_clone)
  register_demo_server_test(test_deploy)
  register_demo_server_test(test_run)
  register_demo_server_test(test_config)
else()
  message(STATUS "Skipping demo server tests because TESTS=${TESTS}")
endif()
