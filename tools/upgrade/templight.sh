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
  while getopts ':t:dh' opt
  do
    case "${opt}" in
      h)
        echo "Usage: $0 [-t <tag>] [-d]"
        echo "-t <tag>"
        echo "  examples for <tag>: master, llvmorg-11.0.0"
        echo "  default value is master"
        echo "-d"
        echo "  disable templight patch"
        exit 1
        ;;
      t)
        GIT_TAG="${OPTARG}"
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

  if [ -z "${GIT_TAG}" ]
  then
    echo
    echo "Please provide a \"-t <tag>\" argument."
    echo "For example: -t llvmorg-11.0.0"
    echo
    exit 1
  fi

  cd 3rd/templight
    echo "Deleting current templight and libc++"
    rm -rf * .??*

    echo "Downloading the source"
    git clone https://github.com/llvm/llvm-project.git .
    git checkout "${GIT_TAG}"
    rm -rf .git
    echo "${GIT_TAG}" > revision.txt

    CLANG_REV_HEADER=../../lib/core/include/metashell/core/clang_revision.hpp

    echo "#ifndef METASHELL_CLANG_REVISION_HPP" > ${CLANG_REV_HEADER}
    echo "#define METASHELL_CLANG_REVISION_HPP" >> ${CLANG_REV_HEADER}
    echo "/*" >> ${CLANG_REV_HEADER}
    echo " * This is an automatically generated header using tools/get_templight.sh" >> ${CLANG_REV_HEADER}
    echo " */" >> ${CLANG_REV_HEADER}
    echo >> ${CLANG_REV_HEADER}
    echo "#ifdef METASHELL_CLANG_REVISION">> ${CLANG_REV_HEADER}
    echo "#error METASHELL_CLANG_REVISION already defined">> ${CLANG_REV_HEADER}
    echo "#endif" >> ${CLANG_REV_HEADER}
    echo -n "#define METASHELL_CLANG_REVISION \"${GIT_TAG}\"" >> ${CLANG_REV_HEADER}
    echo >> ${CLANG_REV_HEADER}
    echo "#endif" >> ${CLANG_REV_HEADER}

    if [ -z "${DISABLE_TEMPLIGHT_PATCH}" ]
    then
      echo "Patching LLVM/Clang"
      cd clang
        cd tools
          git clone 'https://github.com/mikael-s-persson/templight.git'
          rm -rf templight/.git
          echo 'add_clang_subdirectory(templight)' >> CMakeLists.txt
        cd ..
        git apply tools/templight/templight_clang_patch.diff
      cd ..
    else
      echo "Patching LLVM/Clang is disabled"
    fi

    echo "Removing the unused parts of the llvm source code"
    rm -rf \
      libcxx/cmake \
      libcxx/CMakeLists.txt \
      libcxx/lib \
      libcxx/Makefile \
      libcxx/src \
      libcxx/test \
      libcxx/fuzzing \
      libcxx/docs \
      libcxx/utils \
      libcxx/benchmarks \
      libcxx/www \
      \
      clang-tools-extra \
      debuginfo-tests \
      libc \
      libclc \
      libcxxabi \
      libunwind \
      lld \
      lldb \
      llgo \
      mlir \
      openmp \
      parallel-libs \
      polly \
      pstl \
      \
      .github

  cd ../..
else
  echo "Please run this script from the root directory of the Metashell source code"
fi

