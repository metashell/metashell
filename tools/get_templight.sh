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

if [ -d 3rd ]
then
  cd 3rd/templight
    echo "Deleting current templight and libc++"
    rm -rf build llvm libcxx

    echo "Getting libc++"
    svn co http://llvm.org/svn/llvm-project/libcxx/trunk libcxx
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

    echo "Getting LLVM/Clang"
    svn co http://llvm.org/svn/llvm-project/llvm/trunk llvm
    cd llvm
      svn info | grep Revision | egrep -o '[0-9]*' > ../revision.txt
      cd tools
        svn co http://llvm.org/svn/llvm-project/cfe/trunk clang
      cd ..
      cd projects
        svn co http://llvm.org/svn/llvm-project/compiler-rt/trunk compiler-rt
      cd ..

      echo "Make the entire LLVM/Clang source code managed in one git repository"
      sed -i 's/.*\(projects\|tools\).*//g' .gitignore

      if [ -z "${DISABLE_TEMPLIGHT_PATCH}" ]
      then
        echo "Patching LLVM/Clang"
        cd tools/clang
          cd tools
            git clone 'https://github.com/mikael-s-persson/templight.git'
            rm -rf templight/.git
            sed -i '1s/^/add_subdirectory(templight)\n/g' CMakeLists.txt
          cd ..
          svn patch tools/templight/templight_clang_patch.diff
        cd ../..
      else
        echo "Patching LLVM/Clang is disabled"
      fi

      rm -rf .svn
      rm -rf projects/compiler-rt/.svn
      rm -rf tools/clang/.svn
    cd ..

  cd ../..
else
  echo "Please run this script from the root directory of the Metashell source code"
fi

