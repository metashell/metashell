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

set -e

if [ -d templight ]
then
  cd templight
    # Get the patch revision
    PATCH=$(ls patch | sort | tail -1)
    REV=$(echo "$PATCH" | egrep -o '[0-9]*')

    echo "Deleting current templight and libc++"
    rm -rf build llvm libcxx

    echo "Getting libc++ revision ${REV}"
    svn co -r "${REV}" http://llvm.org/svn/llvm-project/libcxx/trunk libcxx
    echo "Removing the unused parts of the libcxx source code"
    rm -rf \
      libcxx/.svn \
      libcxx/cmake \
      libcxx/CMakeLists.txt \
      libcxx/lib \
      libcxx/Makefile \
      libcxx/src \
      libcxx/test \
      libcxx/www

    echo "Getting LLVM/Clang revision ${REV}"
    svn co -r "${REV}" http://llvm.org/svn/llvm-project/llvm/trunk llvm
    cd llvm/tools
      svn co -r "${REV}" http://llvm.org/svn/llvm-project/cfe/trunk clang
    cd ../..
    cd llvm/projects
      svn co -r "${REV}" http://llvm.org/svn/llvm-project/compiler-rt/trunk compiler-rt
    cd ../..

    echo "Make the entire LLVM/Clang source code managed in one git repository"
    cat llvm/.gitignore | grep -v projects | grep -v tools > tmp
    rm llvm/.gitignore
    mv tmp llvm/.gitignore

    rm -rf llvm/.svn
    rm -rf llvm/projects/compiler-rt/.svn
    rm -rf llvm/tools/clang/.svn

    if [ -z "${DISABLE_TEMPLIGHT_PATCH}" ]
    then
      echo "Patching LLVM/Clang"
      cd llvm/tools/clang
        patch -p0 -i "../../../patch/${PATCH}"
      cd ../../..
    else
      echo "Patching LLVM/Clang is disabled"
    fi
    cd llvm/tools/clang/tools
      git clone 'https://github.com/mikael-s-persson/templight.git'
      sed -i '1iadd_subdirectory(templight)' CMakeLists.txt
    cd ../../../..
  cd ..
  echo "Running protoc"
  3rd/protobuf/bin/protoc --cpp_out=lib \
    -I=templight/llvm/tools/clang/tools/templight \
    templight/llvm/tools/clang/tools/templight/templight_messages.proto
else
  echo "Please run this script from the root directory of the Metashell source code"
fi

