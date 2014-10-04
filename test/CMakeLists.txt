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

aux_source_directory(. SOURCES)
add_executable(metashell_test ${SOURCES})

enable_warnings()
use_cpp11()

target_link_libraries(metashell_test metashell_lib)

# Boost for the unit tests
add_definitions( -DBOOST_INCLUDE_PATH=\"${CMAKE_SOURCE_DIR}/boost/include\" )

# Wave
target_link_libraries(metashell_test
  boost_system
  boost_thread
  ${BOOST_ATOMIC_LIB}
  boost_filesystem
  boost_wave
  ${CMAKE_THREAD_LIBS_INIT}
  ${RT_LIBRARY}
)

# Program_options
target_link_libraries(metashell_test boost_program_options)

# Regex
target_link_libraries(metashell_test boost_regex)

# Readline
if (WIN32)
  target_link_libraries(metashell_test edit_static)
  add_definitions( -DUSE_EDITLINE )
  include_directories("${CMAKE_SOURCE_DIR}/wineditline")
else()
  if (USE_EDITLINE)
    target_link_libraries(metashell_test ${EDITLINE_LIBRARY})
  else()
    target_link_libraries(metashell_test ${READLINE_LIBRARY} ${TERMCAP_LIBRARY})
  endif()
endif()

# Clang
include_directories(${CLANG_INCLUDE_DIR})
if (MSVC)
  # libclang calls its import library libclang.imp instead of libclang.lib
  set(CMAKE_IMPORT_LIBRARY_SUFFIX ".imp")

  # delayload libclang
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DELAYLOAD:libclang.dll")

  # PATH of the standard headers
  add_definitions(-DWINDOWS_HEADERS="${CMAKE_SOURCE_DIR}/windows_headers")
endif()
target_link_libraries(metashell_test ${CLANG_LIBRARY})

if (CLANG_STATIC)
  target_link_libraries(metashell_test ${ZLIB_LIBRARIES})
endif()

add_test(metashell_unit_tests metashell_test)

# Code coverage
if (ENABLE_CODE_COVERAGE)
  SETUP_TARGET_FOR_COVERAGE(
    test_coverage
    metashell_test
    coverage
    "'/usr/*';'test/*';'include/just/*'"
  )
endif()

#########################################################
# Copying files next to the Metashell binary on Windows #
#########################################################
include(MetashellClang)

copy_clang_next_to_binary(false)

