#!/bin/bash

set -ex

./install_build_dependencies.sh

# gcc 5 support
sudo unlink /usr/bin/gcc && sudo ln -s /usr/bin/gcc-5 /usr/bin/gcc
sudo unlink /usr/bin/g++ && sudo ln -s /usr/bin/g++-5 /usr/bin/g++

[ "${COVERAGE}" = "true" ] && sudo pip install cpp-coveralls==0.3.12

UBUNTU_VERSION="$(tools/detect_platform.sh --version)"

# Test that the download version links are correct

git fetch --tags
git tag
egrep $(tools/latest_release --no_dots --prefix=version-) README.md
egrep $(tools/latest_release --no_dots --prefix=version-) docs/index.md

# Do static validations

tools/validate/all_static.sh

# Build the docs

mkdocs build

# Get the templight binary

cd 3rd/templight
  ARCHIVE_NAME=templight_ubuntu${UBUNTU_VERSION}_x86_64.tar.bz2
  wget https://github.com/metashell/templight_binary/releases/download/templight_9732a7/${ARCHIVE_NAME}
  tar -xvjf ${ARCHIVE_NAME}
cd ../..

# Test the code

export CXXFLAGS="-Werror"

# If we check for coverage, than add an extra compiler flag
[ "${COVERAGE}" = "true" ] && export CXXFLAGS="${CXXFLAGS} --coverage"
[ "${COVERAGE}" = "true" ] && export BUILD_TYPE="Debug"

BUILD_THREADS=2 NO_TEMPLIGHT=1 METASHELL_NO_DOC_GENERATION=1 ./build.sh

# Collect and upload coverage data
[ "${COVERAGE}" = "true" ] && coveralls \
  -b "bin" \
  --exclude "3rd" \
  --exclude "test" \
  --exclude "bin/3rd" \
  --exclude "bin/test" \
  --exclude "bin/app/include/metashell/" \
  --exclude "bin/_CPack_Packages" \
  --exclude "windows_headers" \
  --gcov gcov-4.8 --gcov-options '\-lp'

cd bin
  ../tools/still_working.py --period_sec 60 -- /bin/bash ../tools/clang_tidy.sh \
    | tee clang_tidy_output.txt
  [ ! -s clang_tidy_output.txt ]
cd ..
