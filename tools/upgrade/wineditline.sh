#!/bin/bash
# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2023, Abel Sinkovics (abel@sinkovics.hu)
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
  echo "Usage: $0 <wineditline version>"
  echo "Example: $0 2.206"
  exit 1
fi

VERSION="$1"
ZIP_NAME="WinEditLine-${VERSION}.zip"
BASE="wineditline-WinEditLine-${VERSION}"

cd 3rd
  mv wineditline/CMakeLists.txt backup.txt
  rm -rf wineditline

  wget "https://github.com/winlibs/wineditline/archive/refs/tags/${ZIP_NAME}"
  unzip "${ZIP_NAME}" "${BASE}/src/*"
  unzip "${ZIP_NAME}" "${BASE}/COPYING"
  mv "${BASE}/src" wineditline
  mv "${BASE}/COPYING" wineditline
  rmdir "${BASE}"
  rm "${ZIP_NAME}"

  mv backup.txt wineditline/CMakeLists.txt
cd ..
