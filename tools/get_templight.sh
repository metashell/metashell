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

if [ -d templight ]
then
  cd templight
    # Get the patch revision
    PATCH=$(ls patch | sort | tail -1)
    REV=$(echo "$PATCH" | egrep -o '[0-9]*')

    echo "Getting LLVM/Clang revision ${REV}"
    svn co -r "${REV}" http://llvm.org/svn/llvm-project/llvm/trunk llvm
    cd llvm/tools
      svn co -r "${REV}" http://llvm.org/svn/llvm-project/cfe/trunk clang
    cd ../..
    cd llvm/projects
      svn co -r "${REV}" http://llvm.org/svn/llvm-project/compiler-rt/trunk compiler-rt
    cd ../..

    echo "Patching LLVM/Clang"
    cd llvm/tools/clang
      patch -p0 -i "../../../patch/${PATCH}"
    cd ../../..

    echo "Building LLVM/Clang"
    mkdir build
    cd build
      ../llvm/configure --enable-optimized
      make -j5
    cd ..

    echo "Zipping the patched LLVM/Clang"
    zip -9 -r '--exclude=*.svn*'  "templight-${REV}.zip" llvm
  cd ..
else
  echo "Please run this script from the root directory of the Metashell source code"
fi

