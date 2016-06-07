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

if [ ! -f compile_commands.json ]
then
  echo "Please run this script from the bin directory (after running cmake)"
  exit 1
fi

if [ -x "../3rd/clang/bin/clang-tidy" ]
then
  CLANG_TIDY="../3rd/clang/bin/clang-tidy"
elif command -v clang-tidy-3.8 >/dev/null 2>&1
then
  CLANG_TIDY="clang-tidy-3.8"
elif command -v clang-tidy >/dev/null 2>&1
then
  CLANG_TIDY="clang-tidy"
elif [ -x "/usr/local/opt/llvm/bin/clang-tidy" ]
then
  # Homebrew installs it here on OS X. This directry shouldn't be added to PATH,
  # so the absolute path is used.
  CLANG_TIDY="/usr/local/opt/llvm/bin/clang-tidy"
else
  echo "clang-tidy not found"
  exit 1
fi

"${CLANG_TIDY}" \
  -p=compile_commands.json \
  $( \
    egrep '"file"' compile_commands.json \
      | sed 's/^.*"file"[ \t]*:[ \t]*"//' \
      | sed 's/"$//' \
      | egrep -v 3rd \
  )

