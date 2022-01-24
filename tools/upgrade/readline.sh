#!/bin/bash
# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2022, Abel Sinkovics (abel@sinkovics.hu)
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

if [ $# -ne 1 ]
then
  echo "Usage: $0 <readline version>"
  echo "Example: $0 8.0"
  exit 1
fi

VERSION="$1"
GZ_NAME="readline-${VERSION}.tar.gz"

cd 3rd
  wget "https://ftp.gnu.org/gnu/readline/${GZ_NAME}"
  tar -xvzf "${GZ_NAME}"
  rm "${GZ_NAME}"

  TEMP="readline-${VERSION}"
  cd readline
    rm -rf COPYING include src

    mv "../${TEMP}/COPYING" .

    mkdir -p include/readline
    mv ../${TEMP}/*.h include/readline
    touch include/readline/config.h

    mkdir src
    mv ../${TEMP}/*.c src
  cd ..
  rm -rf "${TEMP}"
cd ..
