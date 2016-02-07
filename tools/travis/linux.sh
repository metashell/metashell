#!/bin/bash

set -ex

if [ "$CXX" == "g++" ]; then
  export CXX="g++-4.8";
fi

# Test that the download version links are correct

git fetch --tags
git tag
egrep $(tools/latest_release --no_dots --prefix=version-) README.md
egrep $(tools/latest_release --no_dots --prefix=version-) docs/index.md

# Do static validations

sudo pip install pep8 pylint gitpython daemonize
tools/validate/all_static.sh

# Get the templight binary

cd 3rd/templight
  wget https://github.com/sabel83/templight_binary/releases/download/templight_35f974_v2/templight_ubuntu12.04_x86_64.tar.bz2
  tar -xvjf templight_ubuntu12.04_x86_64.tar.bz2
cd ../..

# Test the code

BUILD_THREADS=2 CXXFLAGS=-Werror NO_TEMPLIGHT=1 ./build.sh

sudo apt-get install clang-3.7
cd bin
  ../tools/clang_tidy.sh | tee clang_tidy_output.txt
  [ ! -s clang_tidy_output.txt ]
cd ..

# Test that the documentation about the built-in pragmas and mdb commands is up to date

tools/validate/generated_doc.sh bin/app/metashell/metashell

# Test the demo server's code

tools/demo_server/test/test_git_clone
tools/demo_server/test/test_deploy
