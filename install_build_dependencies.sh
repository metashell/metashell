#!/bin/sh
#
# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

PLATFORM="$(tools/detect_platform.sh)"

echo "Platform: ${PLATFORM}"

case "${PLATFORM}" in
fedora)
  sudo yum -y install \
    git \
    gcc \
    gcc-c++ \
    cmake \
    readline-devel \
    rpm-build
  ;;
opensuse)
  sudo zypper --non-interactive install \
    git \
    cmake \
    gcc-c++ \
    readline-devel \
    rpm-build \
    termcap
  ;;
ubuntu)
  sudo apt-get -y install git g++ cmake libreadline-dev python-pip
  sudo pip install mkdocs
  if [ "$(tools/detect_platform.sh --version)" = "14.04" ]
  then
    wget -O - http://llvm.org/apt/llvm-snapshot.gpg.key | sudo apt-key add -

    FN="clang_3.7.list"
    echo "deb http://llvm.org/apt/trusty/ llvm-toolchain-trusty main" > "${FN}"
    echo "deb-src http://llvm.org/apt/trusty/ llvm-toolchain-trusty main" >> "${FN}"
    echo "deb http://llvm.org/apt/trusty/ llvm-toolchain-trusty-3.7 main" >> "${FN}"
    echo "deb-src http://llvm.org/apt/trusty/ llvm-toolchain-trusty-3.7 main" >> "${FN}"
    sudo mv "${FN}" "/etc/apt/sources.list.d/${FN}"
    sudo apt-get update
    sudo apt-get -y install clang-3.7 clang-format-3.7 clang-tidy-3.7
  fi
  ;;
debian)
  sudo apt-get -y install git g++ cmake libreadline-dev
  ;;
freebsd)
  pkg install -y git cmake gcc
  ;;
openbsd)
  if [ "$(tools/detect_platform.sh --version)" = "5.5" ]
  then
    export PKG_PATH="ftp://ftp.fsn.hu/pub/OpenBSD/5.5/packages/$(machine -a)/"
    pkg_add git g++ cmake python
  else
    echo Unsupported OpenBSD version
    exit 1
  fi
  ;;
*)
  echo Unknown platform. Please install dependencies manually.
  exit 1
  ;;
esac
