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

if [ -x 3rd/clang/bin/clang-format ]
then
  echo 3rd/clang/bin/clang-format
else
  for suffix in "-3.7" ""
  do
    if command -v "clang-format${suffix}" >/dev/null 2>&1
    then
      echo "clang-format${suffix}"
      exit 0
    fi
  done

  echo "clang-format not found" 1>&2
fi

