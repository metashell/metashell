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

if [ "$#" -ne 1 ]
then
  echo "Usage: $0 <path to reference_gen binary>"
  exit 1
fi

REFERENCE_GEN="$1"

function process_one {
  echo "Checking \"$4\" in $3"
  "$1" "$2" | bin/app/replace_part/replace_part -i "$3" -m "$4" -o - -r - | diff "$3" -
}

. tools/template/generated_doc.sh

