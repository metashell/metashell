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

# Arguments
if [ -z "${BUILD_THREADS}" ]
then
  BUILD_THREADS=1
fi

PLATFORM="$(tools/detect_platform.sh)"
CLANG_VERSION=3.8.0
GRAPHVIZ_VERSION=2.40.1

# Show argument & config summary
echo "Number of threads used: ${BUILD_THREADS}"
echo "Platform: ${PLATFORM}"

case "${PLATFORM}" in
arch)
  # If `gcc-multilib` is already installed, don't try to install plain `gcc`
  pacman -Qqs gcc-multilib > /dev/null
  if [ $? ]; then
    sudo pacman --needed -S cmake git python readline
  else
    sudo pacman --needed -S cmake git python readline gcc
  fi
  ;;
fedora)
  sudo yum -y install \
    git \
    gcc \
    gcc-c++ \
    cmake \
    readline-devel \
    rpm-build \
    python \
    make
  ;;
gentoo)
  sudo emerge dev-vcs/git dev-util/cmake sys-libs/libtermcap-compat app-arch/zip
  ;;
opensuse)
  sudo zypper --non-interactive install \
    git \
    cmake \
    gcc-c++ \
    readline-devel \
    rpm-build \
    termcap \
    zip
  ;;
ubuntu)
  apt-cache policy | grep universe > /dev/null
  if [ $? -ne 0 ]
  then
    sudo apt-add-repository universe
  fi
  sudo apt-get -y install \
      git g++ cmake libreadline-dev python-pip zip python3-pip \
      libtool automake autoconf libltdl-dev pkg-config bison flex
  sudo -H pip install pycodestyle pylint gitpython daemonize mkdocs cheetah
  PLATFORM_ID="$(tools/detect_platform.sh --id)"
  UBUNTU_VERSION="$(tools/detect_platform.sh --version)"
  if [ "${UBUNTU_VERSION}" = "18.04" ]
  then
    UBUNTU_VERSION="16.04"
  fi
  CLANG_ARCHIVE="clang+llvm-${CLANG_VERSION}-x86_64-linux-gnu-ubuntu-${UBUNTU_VERSION}"
  mkdir -p "bin/${PLATFORM_ID}"
  cd "bin/${PLATFORM_ID}"
    wget http://llvm.org/releases/${CLANG_VERSION}/${CLANG_ARCHIVE}.tar.xz
    tar -xf ${CLANG_ARCHIVE}.tar.xz
    rm -rf clang
    mv ${CLANG_ARCHIVE} clang
    rm ${CLANG_ARCHIVE}.tar.xz

    if [ "${NO_GRAPHVIZ}" = "" ]
    then
      rm -rf graphviz
      mkdir graphviz
      cd graphviz
        GRAPHVIZ_BIN="$(pwd)"
        cp -r "../../../3rd/graphviz" src
        cd src
          ./autogen.sh
          ./configure --prefix "${GRAPHVIZ_BIN}"
          make -j${BUILD_THREADS}
          make install
        cd ..
      cd ..
    fi
  cd ../..
  ;;
debian)
  sudo apt-get -y install git g++ cmake libreadline-dev zip
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
