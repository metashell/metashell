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

if [ ! -d cmake ]
then
  echo "Please run this script from the root directory of the Metashell source code"
  exit 1
fi

PLATFORM="$(tools/detect_platform.sh)"

echo "Platform: ${PLATFORM}"

if [ "${PLATFORM}" = "opensuse" ]
then
  sudo zypper --non-interactive addrepo http://download.opensuse.org/repositories/home:namtrac:clang/openSUSE_Factory/home:namtrac:clang.repo
  sudo zypper --non-interactive refresh
  sudo zypper --non-interactive install cmake llvm-clang readline-devel termcap rpm-build
else
  echo "Installing system dependiencies on ${PLATFORM} is unsupported"
  exit 1
fi
