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
add_executable(replace_part ${SOURCES})

enable_warnings()
use_cpp11()

# Boost
target_link_libraries(replace_part
  boost_system
  boost_filesystem
  boost_program_options
)

# Just
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/just_file/include")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/3rd/just_lines/include")


