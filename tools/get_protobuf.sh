#!/bin/bash
# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2015, Andras Kucsma (andras.kucsma@gmail.com)
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

if [ ! -d 3rd ]
then
  echo "Please run this script from the root directory of the Metashell source code"
  exit 1
fi

cd 3rd
  if [ -d protobuf ]
  then
    echo "Deleting current protobuf version"
    rm -rf protobuf
  fi

  mkdir -p protobuf
  cd protobuf
    echo "Downloading protobuf 2.6.0"
    wget 'https://protobuf.googlecode.com/svn/rc/protobuf-2.6.0.tar.gz'
    tar xvzf protobuf-2.6.0.tar.gz

    echo "Installing protobuf"
    cd protobuf-2.6.0
      ./configure --prefix="$(readlink -f ..)" && \
      make && \
      make install
    cd ..
    rm -rf protobuf-2.6.0
    rm -f protobuf-2.6.0.tar.gz
  cd ..
cd ..

