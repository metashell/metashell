#!/bin/bash

set -ex

if [ "$CXX" = "g++" ]; then
  brew update >/dev/null
  brew install homebrew/versions/gcc5
  export CXX="g++-5"
fi

# Test that the download version links are correct

git fetch --tags
git tag
egrep $(tools/latest_release --no_dots --prefix=version-) README.md
egrep $(tools/latest_release --no_dots --prefix=version-) docs/index.md

# Get the templight binary

cd 3rd/templight
  wget https://github.com/sabel83/templight_binary/releases/download/templight_35f974_v2/templight_osx10.10.4_x86_64.tar.bz2
  tar -xvjf templight_osx10.10.4_x86_64.tar.bz2
cd ../..

# Test the code

BUILD_THREADS=2 CXXFLAGS=-Werror NO_TEMPLIGHT=1 NO_INSTALLER=1 ./build.sh

# Test that the documentation about the built-in pragmas and mdb commands is up to date

tools/validate/generated_doc.sh bin/app/metashell
