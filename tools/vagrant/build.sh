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

set -ex

if [ ! -d cmake ]
then
  echo "Please run this script from the root directory of the Metashell source code"
  exit 1
fi

if [ -z "$1" ];
then
  echo "Usage: $0 <platform>"
  exit 1
fi

TARGET_PLATFORM="$1"

VAGRANT_DIR="tools/vagrant/${TARGET_PLATFORM}"
VAGRANT_FILE="${VAGRANT_DIR}/Vagrantfile"

if [ ! -f "${VAGRANT_FILE}" ];
then
  echo "Error: ${VAGRANT_FILE} doesn't exist"
  exit 1
fi

# Cleanup if there was a previous run
rm -rf "${VAGRANT_DIR}/metashell"

# VAGRANT_DIR will be mounted up to the guest to /vagrant.
# Clone the local repo there.
git clone . "${VAGRANT_DIR}/metashell"
git -C "${VAGRANT_DIR}/metashell" checkout "$(git rev-parse HEAD)"

cd "${VAGRANT_DIR}"
  vagrant up --provider virtualbox
  vagrant ssh -c "/vagrant/metashell/tools/vagrant/guest_full_build.sh"
  vagrant halt
cd ../../..
