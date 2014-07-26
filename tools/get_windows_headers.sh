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

if [ $# -ne 1 ]
then
  echo "Script to download the Windows headers"
  echo "for Metashell providing the standard library."
  echo 
  echo "Usage: $0 <Cygwin mirror to use>"
  echo "Example: $0 http://ftp.fsn.hu/pub/cygwin"
  echo
  exit 1
fi

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

MIRROR="$1"
OUT_DIR="windows_headers"
TMP_DIR="tmp"

function get {
  DIR="$1"
  PACKAGE="$2"
  PATH_TO_COPY="$3"
  COPY_TO="$4"
  TMP_DIR="$5"

  rm -rf "$TMP_DIR/download"
  mkdir -p "$TMP_DIR/download"
  mkdir -p "$TMP_DIR/extract"
  cd "$TMP_DIR/download"
  wget $MIRROR/x86/release/$DIR/$PACKAGE
  tar xf $PACKAGE
  mkdir "../extract/$COPY_TO"
  cp -r $PATH_TO_COPY/* "../extract/$COPY_TO"
  cd ../..
  rm -rf "$TMP_DIR/download"
}

function copy_subtree {
  FROM="$1"
  TO="$2"
  TMP_DIR="$3"

  mkdir $TMP_DIR/extract/$TO
  cp -r $TMP_DIR/extract/$FROM/* $TMP_DIR/extract/$TO
}

rm -rf $TMP_DIR
rm -rf $OUT_DIR

get mingw/mingw-runtime            mingw-runtime-4.0-1.tar.bz2       usr/i686-pc-mingw32/sys-root/mingw/include     d1 $TMP_DIR
get mingw/mingw-gcc/mingw-gcc-core mingw-gcc-core-4.7.3-1.tar.bz2    usr/lib/gcc/i686-pc-mingw32/4.7.3/include      d2 $TMP_DIR
get mingw/mingw-gcc/mingw-gcc-g++  mingw-gcc-g++-4.7.3-1.tar.bz2     usr/lib/gcc/i686-pc-mingw32/4.7.3/include/c++  d3 $TMP_DIR
copy_subtree                                                         d3/i686-pc-mingw32                             d4 $TMP_DIR
get mingw/mingw-w32api             mingw-w32api-4.0-1.tar.bz2        usr/i686-pc-mingw32/sys-root/mingw/include     d5 $TMP_DIR

mkdir $OUT_DIR
for i in 5 4 3 2 1
do
  cp -r $TMP_DIR/extract/d$i/* $OUT_DIR
done
rm -rf $TMP_DIR


