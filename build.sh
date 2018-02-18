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

set -e

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

if [ -z "${BUILD_TYPE}" ]
then
  BUILD_TYPE="Release"
fi

PLATFORM="$(tools/detect_platform.sh)"

# Config
if [ "${PLATFORM}" = "openbsd" ]
then
  export CC=egcc
  export CXX=eg++
fi

# Show argument & config summary
echo "Number of threads used: ${BUILD_THREADS}"
echo "Platform: ${PLATFORM}"

# Build Templight
if [ "${NO_TEMPLIGHT}" = "" ]
then
  if [ "${PLATFORM}" = "opensuse" ] || [ "${PLATFORM}" = "fedora" ]
  then
    # The default Templight include path seems to be empty on Tumbleweed
    C_INCLUDE_DIRS="-DC_INCLUDE_DIRS=$(tools/clang_default_path --gcc g++ -f shell)"
  else
    C_INCLUDE_DIRS=""
  fi

  cd 3rd
    cd templight
      mkdir -p build; cd build
        cmake \
          -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
          -DLLVM_ENABLE_TERMINFO=OFF \
          "${C_INCLUDE_DIRS}" \
          ../llvm
          make templight -j${BUILD_THREADS}
      cd ..
    cd ..
  cd ..
else
  echo "Skipping Templight build, because \$NO_TEMPLIGHT = \"${NO_TEMPLIGHT}\""
fi

mkdir -p bin; cd bin
  if [ -z "${METASHELL_NO_DOC_GENERATION}" ]
  then
    cmake ..
  else
    cmake .. -DMETASHELL_NO_DOC_GENERATION=1
  fi
  make -j${BUILD_THREADS}
  make test || (cat Testing/Temporary/LastTest.log && false)

  if [ "${NO_INSTALLER}" = "" ]
  then
    cpack
    make system_test_zip

    cd ../3rd/boost
      zip -qr ../../bin/system_test_boost.zip \
        include/boost/config.hpp \
        include/boost/config \
        include/boost/detail \
        include/boost/mpl \
        include/boost/preprocessor \
        include/boost/type_traits
    cd ../../bin
  else
    echo "Skipping installer generation, because \$NO_INSTALLER = \"${NO_INSTALLER}\""
  fi
cd ..
