#!/bin/bash

set -ex

brew update >/dev/null

brew install p7zip

if [ "$CXX" = "g++" ]; then
  brew install gcc@6
  export CXX="g++-6"
fi

# Test that the download version links are correct

git fetch --tags
git tag
egrep $(tools/latest_release --no_dots --prefix=version-) README.md
egrep $(tools/latest_release --no_dots --prefix=version-) docs/index.md

PLATFORM_ID="$(tools/detect_platform.sh --id)"

# Get the templight binary

mkdir -p "bin/${PLATFORM_ID}"
cd "bin/${PLATFORM_ID}"
  ARCHIVE_NAME=templight_osx10.13.4_x86_64.tar.bz2
  wget https://github.com/metashell/templight_binary/releases/download/templight_9732a7/${ARCHIVE_NAME}
  tar -xvjf ${ARCHIVE_NAME}
  mv build templight
cd ../..

# Test the code

BUILD_THREADS=2 \
TEST_THREADS=1 \
NO_TEMPLIGHT=1 \
METASHELL_NO_DOC_GENERATION=1 \
./build.sh
