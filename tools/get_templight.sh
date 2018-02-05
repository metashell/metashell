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
  while getopts ':r:dh' opt
  do
    case "${opt}" in
      h)
        echo "Usage: $0 [-r <svn_directory>] [-d]"
        echo "-r <svn_directory>"
        echo "  examples for <svn_directory>: trunk, trunk@212345, tags/RELEASE_380/final"
        echo "  default value is trunk"
        echo "-d"
        echo "  disable templight patch"
        exit 1
        ;;
      r)
        SVN_DIRECTORY="${OPTARG}"
        ;;
      d)
        DISABLE_TEMPLIGHT_PATCH="disable"
        ;;
      \?)
        echo "Invalid option -${OPTARG}" >&2
        exit 1
        ;;
      :)
        echo "Missing option for -${OPTARG}" >&2
        exit 1
        ;;
    esac
  done

  if [ -z "${SVN_DIRECTORY}" ]
  then
    SVN_DIRECTORY="trunk"
  fi

  cd 3rd/templight
    echo "Deleting current templight and libc++"
    rm -rf build llvm libcxx

    echo "Getting libc++"
    svn co "http://llvm.org/svn/llvm-project/libcxx/${SVN_DIRECTORY}" libcxx
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

    echo "Getting LLVM/Clang from $SVN_DIRECTORY"
    svn co "http://llvm.org/svn/llvm-project/llvm/${SVN_DIRECTORY}" llvm

    cd llvm
      CLANG_REV_FILE=../revision.txt
      CLANG_REV_HEADER=../../../include/metashell/clang_revision.hpp

      CLANG_REV="$(svn info | grep Revision | egrep -o '[0-9]+')"
      CLANG_REV_TEXT="${SVN_DIRECTORY} (r${CLANG_REV})"

      echo "${CLANG_REV_TEXT}" > "${CLANG_REV_FILE}"

      echo "#ifndef METASHELL_CLANG_REVISION_HPP" > ${CLANG_REV_HEADER}
      echo "#define METASHELL_CLANG_REVISION_HPP" >> ${CLANG_REV_HEADER}
      echo "/*" >> ${CLANG_REV_HEADER}
      echo " * This is an automatically generated header using tools/get_templight.sh" >> ${CLANG_REV_HEADER}
      echo " */" >> ${CLANG_REV_HEADER}
      echo >> ${CLANG_REV_HEADER}
      echo "#ifdef METASHELL_CLANG_REVISION">> ${CLANG_REV_HEADER}
      echo "#error METASHELL_CLANG_REVISION already defined">> ${CLANG_REV_HEADER}
      echo "#endif" >> ${CLANG_REV_HEADER}
      echo -n "#define METASHELL_CLANG_REVISION \"${CLANG_REV_TEXT}\"" >> ${CLANG_REV_HEADER}
      echo >> ${CLANG_REV_HEADER}
      echo "#endif" >> ${CLANG_REV_HEADER}

      cd tools
        svn co "http://llvm.org/svn/llvm-project/cfe/${SVN_DIRECTORY}" clang
      cd ..
      cd projects
        svn co "http://llvm.org/svn/llvm-project/compiler-rt/${SVN_DIRECTORY}" compiler-rt
      cd ..

      echo "Make the entire LLVM/Clang source code managed in one git repository"
      sed -i '' 's/.*projects.*//g' .gitignore
      sed -i '' 's/.*tools.*//g' .gitignore

      if [ -z "${DISABLE_TEMPLIGHT_PATCH}" ]
      then
        echo "Patching LLVM/Clang"
        cd tools/clang
          cd tools
            git clone 'https://github.com/mikael-s-persson/templight.git'
            rm -rf templight/.git
            echo 'add_subdirectory(templight)' | cat - CMakeLists.txt > temp && mv temp CMakeLists.txt
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

