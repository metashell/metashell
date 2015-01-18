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
add_library(metashell_core_lib STATIC ${SOURCES})

enable_warnings()
use_cpp11()

# Readline
if (WIN32)
  add_definitions( -DUSE_EDITLINE )
  include_directories("${CMAKE_SOURCE_DIR}/3rd/wineditline")
else()
  if (USE_EDITLINE)
    include_directories(${EDITLINE_INCLUDE_DIR})
    add_definitions( -DUSE_EDITLINE )
  else()
    include_directories(${READLINE_INCLUDE_DIR})
  endif()
endif()

# Clang
include_directories(${CLANG_INCLUDE_DIR})
if (MSVC)
  # libclang calls its import library libclang.imp instead of libclang.lib
  set(CMAKE_IMPORT_LIBRARY_SUFFIX ".imp")
endif()
target_link_libraries(metashell_core_lib ${CLANG_LIBRARY})

# mIndent
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/mindent/include")

# Just
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/just_process/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/just_console/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/just_environment/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/just_temp/include")

