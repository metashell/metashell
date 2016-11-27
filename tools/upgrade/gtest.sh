#!/bin/bash
# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

if [ $# -ne 1 ]
then
  echo "Usage: $0 <gtest version>"
  echo "Example: $0 1.8.0"
  exit 1
fi

GTEST_VERSION="$1"
GTEST_ARCHIVE="release-${GTEST_VERSION}.tar.gz"

DIRS="googletest/include googletest/src googlemock/include googlemock/src"

cd 3rd
  rm -rf ${DIRS}
  wget "https://github.com/google/googletest/archive/${GTEST_ARCHIVE}"
  for DIR in ${DIRS}
  do
    tar -xvzf "${GTEST_ARCHIVE}" \
      "googletest-release-${GTEST_VERSION}/${DIR}" \
      --transform 's/^[^\/]*\///'
  done
  rm "${GTEST_ARCHIVE}"
