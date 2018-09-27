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

if [ $# -ne 1 ]
then
  echo "Usage: $0 <just component>"
  echo "Example: $0 test"
  exit 1
fi

COMPONENT="$1"
COMPONENT_DIR="../3rd/just/${COMPONENT}"

rm -rf "${COMPONENT_DIR}"
mkdir -p "${COMPONENT_DIR}/include/just"

cd "${COMPONENT_DIR}/include/just"
  wget "https://raw.githubusercontent.com/sabel83/just/master/include/just/${COMPONENT}.hpp"
cd ../../..

