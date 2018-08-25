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

function clean_lib {
  LIB="$1"

  rm -rf $(
    find "${BOOST_DIR}/libs/${LIB}" -not \( -type d -or -name CMakeLists.txt \)
  )
}

function get_headers {
  LIB="$1"

  git clone "https://github.com/boostorg/${LIB}.git"
  cd "${LIB}"
    git checkout "boost-${BOOST_VERSION}"
    cp -r "include" "../${BOOST_DIR}"
  cd ..
}

function get_lib {
  LIB="$1"

  clean_lib "${LIB}"

  get_headers "${LIB}"

  cd "${LIB}"
    cp -r src/* "../${BOOST_DIR}/libs/${LIB}"
  cd ..
}

mkdir -p 3rd/boost
rm -rf 3rd/boost/include

rm -rf tmp
mkdir tmp
cd tmp

  get_headers algorithm
  get_headers align
  get_headers any
  get_headers array
  get_headers assert
  get_headers assign
  get_lib     atomic
  get_headers bind
  get_headers chrono
  get_headers concept_check
  get_headers config
  get_headers container
  get_headers container_hash
  get_headers conversion
  get_headers core
  get_headers date_time
  get_headers detail
  get_headers exception
  get_lib     filesystem
  get_headers foreach
  get_headers function
  get_headers functional
  get_headers function_types
  get_headers fusion
  get_headers graph
  get_headers integer
  get_headers intrusive
  get_headers io
  get_headers iostreams
  get_headers iterator
  get_headers lexical_cast
  get_headers math
  get_headers move
  get_headers mpl
  get_headers multi_index
  get_headers numeric_conversion
  get_headers optional
  get_headers phoenix
  get_headers pool
  get_headers predef
  get_headers preprocessor
  get_lib     program_options
  get_headers property_map
  get_headers proto
  get_headers range
  get_headers ratio
  get_lib     regex
  get_headers serialization
  get_headers smart_ptr
  get_headers spirit
  get_headers static_assert
  get_lib     system
  get_lib     thread
  get_headers throw_exception
  get_headers tokenizer
  get_headers tuple
  get_headers typeof
  get_headers type_index
  get_headers type_traits
  get_headers unordered
  get_headers utility
  get_headers variant
  get_lib     wave
  get_headers winapi
  get_headers xpressive

 # These warnings can not be disabled
 sed -i 's/^[ \t]*#[ \t]*pragma[ \t]*message.*//' \
   $(find "${BOOST_DIR}/include" -name '*.hpp')

cd ..
rm -rf tmp

