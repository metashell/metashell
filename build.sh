#!/bin/sh
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

# Detect platform
if [ -e /etc/redhat-release ] || [ -e /etc/fedora-release ]
then
  PLATFORM=fedora
elif [ -e /etc/SuSE-release ]
then
  PLATFORM=opensuse
elif [ "$(uname)" = "Darwin" ]
then
  PLATFORM=osx
  PLATFORM_VERSION=$(sw_vers -productVersion)
elif [ `cat /etc/lsb-release 2>/dev/null | grep DISTRIB_ID` = "DISTRIB_ID=Ubuntu" ]
then
  PLATFORM=ubuntu
elif [ -e /etc/debian_version ]
then
  PLATFORM=debian
elif [ `uname` = "FreeBSD" ]
then
  PLATFORM=freebsd
elif [ `uname` = "OpenBSD" ]
then
  PLATFORM=openbsd
else
  PLATFORM=unknown
fi

# Config
if [ "${PLATFORM}" = "openbsd" ]
then
  export CC=egcc
  export CXX=eg++
fi

# Show argument & config summary
echo "Number of threads used: ${BUILD_THREADS}"
echo "Platform: ${PLATFORM}"

# Build Clang
cd 3rd
  cd templight
    mkdir build; cd build
      cmake ../llvm -DLIBCLANG_BUILD_STATIC=ON -DCMAKE_BUILD_TYPE=Release \
        && make clang libclang libclang_static templight -j${BUILD_THREADS}
    cd ..
  cd ..
cd ..

# Build & package Metashell
if [ "${PLATFORM}" = "fedora" ] || [ "${PLATFORM}" = "opensuse" ]
then
  tools/clang_default_path --gcc=g++ > lib/core/extra_sysinclude.hpp
elif [ "${PLATFORM}" = "openbsd" ]
then
  python2.7 tools/clang_default_path --gcc=eg++ > lib/core/extra_sysinclude.hpp
elif [ "${PLATFORM}" = "osx" ] && [ "${PLATFORM_VERSION}" = "10.10" ]
then
  tools/clang_default_path --gcc=clang > lib/core/extra_sysinclude.hpp
fi

mkdir bin; cd bin
  cmake .. \
    && make -j${BUILD_THREADS} \
    && make test \
    && cpack
cd ..

