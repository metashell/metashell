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

if [ -e /etc/redhat-release ] || [ -e /etc/fedora-release ]
then
  PLATFORM="fedora"
elif [ -e /etc/SuSE-release ]
then
  PLATFORM="opensuse"
elif [ "$(uname)" = "Darwin" ]
then
  PLATFORM="osx"
elif [ "`cat /etc/lsb-release 2>/dev/null | grep DISTRIB_ID`" = "DISTRIB_ID=Ubuntu" ]
then
  PLATFORM="ubuntu"
elif [ -e /etc/debian_version ]
then
  PLATFORM="debian"
elif [ -e /etc/arch-release ]
then
  PLATFORM="arch"
elif [ "$(uname)" = "FreeBSD" ]
then
  PLATFORM="freebsd"
elif [ "$(uname)" = "OpenBSD" ]
then
  PLATFORM="openbsd"
else
  PLATFORM="unknown"
fi

if [ "$1" = "--version" ]
then
  if [ "$PLATFORM" = "ubuntu" ]
  then
    lsb_release -a 2>/dev/null | grep Release | sed 's/^Release:[ \t]*//'
  elif [ "$PLATFORM" = "openbsd" ]
  then
    uname -a | cut -d ' ' -f 3
  else
    echo "Detecting version of this platform has not been implemented"
    exit 1
  fi
else
  echo $PLATFORM
fi
