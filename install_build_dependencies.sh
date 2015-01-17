#!/bin/sh
#
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

if [ -e /etc/redhat-release ] || [ -e /etc/fedora-release ]
then
  yum -y install \
    git \
    gcc \
    gcc-c++ \
    cmake \
    readline-devel \
    rpm-build
elif [ -e /etc/SuSE-release ]
then
  sudo zypper --non-interactive install \
    git \
    cmake \
    gcc-c++ \
    readline-devel \
    rpm-build \
    termcap
elif [ "DISTRIB_ID=Ubuntu" = "$(cat /etc/lsb-release 2>/dev/null | grep DISTRIB_ID)" ]
then
  sudo apt-get -y install git g++ cmake libreadline-dev python-pip
  sudo pip install mkdocs
elif [ -e /etc/debian_version ]
then
  apt-get -y install git g++ cmake libreadline-dev
elif [ `uname` = "FreeBSD" ]
then
  pkg install -y git cmake gcc
elif [ `uname` = "OpenBSD" ]
then
  if [ `uname -a | cut -d ' ' -f 3` = "5.5" ]
  then
    export PKG_PATH=ftp://ftp.fsn.hu/pub/OpenBSD/5.5/packages/`machine -a`/
    pkg_add git g++ cmake python
  else
    echo Unsupported OpenBSD version
    exit 1
  fi
else
  echo Unknown platform. Please install dependencies manually.
  exit 1
fi

