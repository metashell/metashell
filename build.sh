#!/bin/bash
#
# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

if [ ! -d cmake ]
then
  echo "Please run this script from the root directory of the Metashell source code"
  exit 1
fi

# Arguments
if [ -z "${BUILD_THREADS}" ]
then
  BUILD_THREADS=1
fi

# Detect config
if \
  [ -e /etc/redhat-release ] \
  || [ -e /etc/fedora-release ] \
  || [ -e /etc/SuSE-release ]
then
  PACKAGE_FORMAT=RPM
else
  PACKAGE_FORMAT=DEB
fi

# Show argument & config summary
echo "Number of threads used: ${BUILD_THREADS}"
echo "Package format for the installer: ${PACKAGE_FORMAT}"

# Build Clang
cd templight
  mkdir build; cd build
    cmake ../llvm -DLIBCLANG_BUILD_STATIC=ON \
      && make clang -j${BUILD_THREADS} \
      && make libclang -j${BUILD_THREADS} \
      && make libclang_static -j${BUILD_THREADS}
  cd ..
cd ..

# Build & package Metashell
mkdir bin; cd bin
  cmake .. \
    && make -j${BUILD_THREADS} \
    && make test \
    && cpack -G "${PACKAGE_FORMAT}"
cd ..

