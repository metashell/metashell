#!/bin/bash
#
# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

if [ -e /etc/redhat-release ] || [ -e /etc/fedora-release ]; then
  PACKAGE_FORMAT=RPM
  CMAKE_ARG="-DCLANG_LIBRARYDIR=/usr/lib/llvm"
elif [ -e /etc/SuSE-release ]; then
  PACKAGE_FORMAT=RPM
elif [ -e /etc/lsb-release ] && grep Ubuntu /etc/lsb-release; then
  UBUNTU_VERSION=$(grep DISTRIB_RELEASE /etc/lsb-release | sed 's/^[^=]*=//')
  case $UBUNTU_VERSION in
    12.04)
      sed -i 's/\(libboost-[^0-9]*\)\([0-9]*\.\)\{2\}[0-9]*/\11.48.0/g' CMakeLists.txt
      ;;
    13.04)
      # Nothing to do
      ;;
    *)
      echo "The script does not support Ubuntu $UBUNTU_VERSION. Please build manually."
      exit 1
      ;;
  esac
  PACKAGE_FORMAT=DEB
else
  echo "The script could not detect your system. Please build manually."
  exit 1
fi

tools/clang_default_path > lib/extra_sysinclude.hpp \
  \
  && mkdir -p bin \
  && cd bin \
  && cmake .. $CMAKE_ARG \
  && make \
  && make test \
  \
  && cpack -G $PACKAGE_FORMAT \
  \
  && echo done

