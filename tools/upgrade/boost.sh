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
  echo "Usage: $0 <boost version>"
  echo "Example: $0 1.61.0"
  exit 1
fi

BOOST_VERSION="$1"
BOOST_DIR="../3rd/boost"

function get_lib {
  LIB="$1"

  rm -rf $(
    find "${BOOST_DIR}/${LIB}" -not \( -type d -or -name CMakeLists.txt \)
  )

  git clone "https://github.com/boostorg/${LIB}.git"
  cd "${LIB}"
    git checkout "boost-${BOOST_VERSION}"
    mkdir -p "../${BOOST_DIR}/${LIB}"
    cp -r include "../${BOOST_DIR}/${LIB}"
    if [ -d src ]
    then
      cp -r src "../${BOOST_DIR}/${LIB}"
    fi
  cd ..
}

mkdir -p 3rd/boost

rm -rf tmp
mkdir tmp
cd tmp

  get_lib algorithm
  get_lib align
  get_lib any
  get_lib array
  get_lib assert
  get_lib assign
  get_lib atomic
  get_lib bind
  get_lib chrono
  get_lib concept_check
  get_lib config
  get_lib container
  get_lib container_hash
  get_lib conversion
  get_lib core
  get_lib date_time
  get_lib detail
  get_lib exception
  get_lib filesystem
  get_lib foreach
  get_lib function
  get_lib functional
  get_lib function_types
  get_lib fusion
  get_lib graph
  get_lib integer
  get_lib intrusive
  get_lib io
  get_lib iostreams
  get_lib iterator
  get_lib lexical_cast
  get_lib math
  get_lib move
  get_lib mpl
  get_lib multi_index
  get_lib numeric_conversion
  get_lib optional
  get_lib phoenix
  get_lib pool
  get_lib predef
  get_lib preprocessor
  get_lib property_map
  get_lib proto
  get_lib range
  get_lib ratio
  get_lib regex
  get_lib serialization
  get_lib smart_ptr
  get_lib spirit
  get_lib static_assert
  get_lib system
  get_lib thread
  get_lib throw_exception
  get_lib tokenizer
  get_lib tuple
  get_lib typeof
  get_lib type_index
  get_lib type_traits
  get_lib unordered
  get_lib utility
  get_lib variant
  get_lib wave
  get_lib winapi

 # These warnings can not be disabled
 sed -i 's/^[ \t]*#[ \t]*pragma[ \t]*message.*//' \
   $(find "${BOOST_DIR}" -name '*.hpp')

cd ..
rm -rf tmp

