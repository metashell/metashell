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

SRC_ROOT=$(dirname "$0")

# Arguments
if [ -z "${BUILD_THREADS}" ]
then
  BUILD_THREADS=1
fi

if [ -z "${NO_SUDO}" ]
then
  SUDO=sudo
else
  SUDO=
fi

PLATFORM="$($SRC_ROOT/tools/detect_platform.sh)"
CLANG_VERSION=10.0.1
GRAPHVIZ_VERSION=2.40.1

# Show argument & config summary
echo "Number of threads used: ${BUILD_THREADS}"
echo "Platform: ${PLATFORM}"
echo "sudo: ${SUDO}"

case "${PLATFORM}" in
arch)
  # If `gcc-multilib` is already installed, don't try to install plain `gcc`
  pacman -Qqs gcc-multilib > /dev/null
  if [ $? ]; then
    ${SUDO} pacman --needed -S cmake git python
  else
    ${SUDO} pacman --needed -S cmake git python gcc
  fi
  ;;
fedora)
  ${SUDO} yum -y install \
    git \
    gcc \
    gcc-c++ \
    cmake \
    rpm-build \
    python \
    make
  ;;
gentoo)
  ${SUDO} emerge dev-vcs/git dev-util/cmake sys-libs/libtermcap-compat app-arch/zip
  ;;
opensuse)
  ${SUDO} zypper --non-interactive install \
    git \
    cmake \
    gcc-c++ \
    rpm-build \
    termcap \
    zip
  ;;
ubuntu)
  apt-cache policy | grep universe > /dev/null
  if [ $? -ne 0 ]
  then
    ${SUDO} apt-add-repository universe
  fi
  ${SUDO} apt-get -y install \
      git g++ cmake zip python3-pip libtinfo5 \
      libtool automake autoconf libltdl-dev pkg-config bison flex rpm
  PLATFORM_VERSION="$($SRC_ROOT/tools/detect_platform.sh --version)"
  if [ "${PLATFORM_VERSION}" = "22.04" ]
  then
    ${SUDO} apt -y install g++-12
  fi
  ${SUDO} -H pip3 install pycodestyle pylint gitpython daemonize mkdocs cheetah3
  PLATFORM_ID="$($SRC_ROOT/tools/detect_platform.sh --id)"
  CLANG_ARCHIVE="clang+llvm-${CLANG_VERSION}-x86_64-linux-gnu-ubuntu-16.04"
  mkdir -p "bin/${PLATFORM_ID}"
  cd "bin/${PLATFORM_ID}"
    wget https://github.com/llvm/llvm-project/releases/download/llvmorg-${CLANG_VERSION}/${CLANG_ARCHIVE}.tar.xz
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
  ${SUDO} apt-get -y install git g++ cmake zip python3 file rpm
  ;;
freebsd)
  pkg install -y git cmake gcc
  ;;
openbsd)
  if [ "$($SRC_ROOT/tools/detect_platform.sh --version)" = "5.5" ]
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
