#!/bin/bash
# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

set -e

if [ ! -d cmake ]
then
  echo "Please run this script from the root directory of the Metashell source code"
  exit 1
fi

if command -v clang-format-3.7 >/dev/null 2>&1
then
  CLANG_FORMAT="clang-format-3.7"
elif command -v clang-format >/dev/null 2>&1
then
  CLANG_FORMAT="clang-format"
else
  echo "clang-format not found"
  exit 1
fi

for l in include lib test app
do
  "${CLANG_FORMAT}" -i $(find "$l" -type f -a '(' \
    -name '*.hpp' -o \
    -name '*.cpp' -o \
    -name '*.cc' -o \
    -name '*.c' -o \
    -name '*.h' \
  ')' )
done

