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
  echo "fedora"
elif [ -e /etc/SuSE-release ]
then
  echo "opensuse"
elif [ "$(uname)" = "Darwin" ]
then
  echo "osx"
elif [ "`cat /etc/lsb-release 2>/dev/null | grep DISTRIB_ID`" = "DISTRIB_ID=Ubuntu" ]
then
  echo "ubuntu"
elif [ -e /etc/debian_version ]
then
  echo "debian"
elif [ -e /etc/arch-release ]
then
  echo "arch"
elif [ "$(uname)" = "FreeBSD" ]
then
  echo "freebsd"
elif [ "$(uname)" = "OpenBSD" ]
then
  echo "openbsd"
else
  echo "unknown"
fi
