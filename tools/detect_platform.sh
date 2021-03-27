#!/bin/sh
#
# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2015, Andras Kucsma (andras.kucsma@gmail.com)
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

if [ "${PLATFORM}${VERSION}${SYSTEM_PROCESSOR}" = "" ]
then
  if [ -e /etc/redhat-release ] || [ -e /etc/fedora-release ]
  then
    PLATFORM="fedora"
    VERSION=$(cat /etc/fedora-release | egrep -o '[0-9.]+')
  elif [ -e /etc/SuSE-release ]
  then
    PLATFORM="opensuse"
    VERSION=$(egrep -o 'VERSION[ ]*=[^\n]*' /etc/SuSE-release | egrep -o '[0-9.]*')
  elif [ -e /etc/gentoo-release ]
  then
    PLATFORM="gentoo"
  elif [ "$(uname)" = "Darwin" ]
  then
    PLATFORM="osx"
    VERSION=$(sw_vers -productVersion)
  elif [ "`cat /etc/lsb-release 2>/dev/null | grep DISTRIB_ID`" = "DISTRIB_ID=Ubuntu" ]
  then
    PLATFORM="ubuntu"
    VERSION=$(lsb_release -a 2>/dev/null | grep Release | sed 's/^Release:[ \t]*//')
  elif [ -e /etc/debian_version ]
  then
    PLATFORM="debian"
    VERSION=$(cat /etc/debian_version)
  elif [ -e /etc/arch-release ]
  then
    PLATFORM="arch"
  elif [ "$(uname)" = "FreeBSD" ]
  then
    PLATFORM="freebsd"
  elif [ "$(uname)" = "OpenBSD" ]
  then
    PLATFORM="openbsd"
    VERSION=$(uname -a | cut -d ' ' -f 3)
  elif [ -e /etc/os-release ] && grep openSUSE /etc/os-release > /dev/null
  then
    PLATFORM="opensuse"
    VERSION=$(egrep -o 'VERSION[ ]*=[^\n]*' /etc/os-release | egrep -o '[0-9.]*')
  else
    PLATFORM="unknown"
  fi
  SYSTEM_PROCESSOR=$(uname -m)
fi

if [ "$1" = "--version" ]
then
  if [ "$VERSION" = "" ]
  then
    echo "Detecting version of this platform has not been implemented"
    exit 1
  else
    echo "$VERSION"
  fi
elif [ "$1" = "--id" ]
then
  if [ "$VERSION" = "" ]
  then
    echo "Detecting version (needed for id) of this platform has not been implemented"
    exit 1
  elif [ "${SYSTEM_PROCESSOR}" = "" ]
  then
    echo "Detecting system processor (needed for id) of this platform has not been implemented"
    exit 1
  else
    echo "${PLATFORM}${VERSION}_${SYSTEM_PROCESSOR}"
  fi
else
  echo $PLATFORM
fi
