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

# First install the dependencies, so we guaranteed to have git
cd /vagrant/metashell
  ./install_build_dependencies.sh
cd ~

# Clone the repo to home. Build doesn't work directly in the mounted directory
git clone /vagrant/metashell

cd metashell
  ./build.sh
  # TODO some nicer way to get the name of the generated package
  cp bin/*.rpm /vagrant
cd ..
