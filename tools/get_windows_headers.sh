#!/bin/bash
# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

##################################
# Validations before downloading #
##################################

if echo .* | grep .travis.yml > /dev/null
then
  :
else
  echo Please run the script from the source root
  exit 1
fi

#########################
# Getting the arguments #
#########################

OUT_DIR="windows_headers"
TMP_DIR="tmp"
GCC_VERSION="4.8.1"
VERSION="$GCC_VERSION-4"

function get {
  PACKAGE_DIR="$1"
  PACKAGE="$2"
  PATH_TO_COPY="$3"
  VERSION="$4"
  TMP_DIR="$5"
  OUT_DIR="$6"

  FN="$PACKAGE"

  rm -rf "$TMP_DIR"
  mkdir "$TMP_DIR"
  cd "$TMP_DIR"
  curl -L -o $FN -O http://sourceforge.net/projects/mingw/files/MinGW/Base/$PACKAGE_DIR/$FN/download
  tar -xvf $FN
  cp -r $PATH_TO_COPY/* "../$OUT_DIR"
  cd ..
  rm -rf "$TMP_DIR"
}

rm -rf $TMP_DIR
rm -rf $OUT_DIR

mkdir $OUT_DIR

get mingw-rt/mingwrt-4.0.3    mingwrt-4.0.3-1-mingw32-dev.tar.lzma  include                                          "$VERSION" "$TMP_DIR" "$OUT_DIR"
get gcc/Version4/gcc-$VERSION gcc-c++-$VERSION-mingw32-dev.tar.lzma mingw32/lib/gcc/mingw32/$GCC_VERSION/include/c++ "$VERSION" "$TMP_DIR" "$OUT_DIR"
get w32api/w32api-4.0.3       w32api-4.0.3-1-mingw32-dev.tar.lzma   include                                          "$VERSION" "$TMP_DIR" "$OUT_DIR"

