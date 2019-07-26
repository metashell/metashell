#!/bin/bash

set -ex

export BUILD_THREADS=2

if [ "${STATIC_CHECKS}" != "true" ]
then
  export NO_GRAPHVIZ=1
fi

./install_build_dependencies.sh

# gcc 5 support
sudo unlink /usr/bin/gcc && sudo ln -s /usr/bin/gcc-6 /usr/bin/gcc
sudo unlink /usr/bin/g++ && sudo ln -s /usr/bin/g++-6 /usr/bin/g++

PLATFORM_ID="$(tools/detect_platform.sh --id)"

if [ "${STATIC_CHECKS}" = "true" ]
then

  # Test that the download version links are correct

  git fetch --tags
  git tag
  egrep $(tools/latest_release --no_dots --prefix=version-) README.md
  egrep $(tools/latest_release --no_dots --prefix=version-) docs/index.md

  # Do static validations

  tools/validate/all_static.sh

  # Build the docs

  mkdocs build

  # Run clang-tidy

  mkdir -p "bin/${PLATFORM_ID}/metashell"
  cd "bin/${PLATFORM_ID}/metashell"
    cmake ../../..
    ../../..tools/still_working.py --period_sec 60 -- /bin/bash ../../../tools/clang_tidy.sh \
      | tee clang_tidy_output.txt
    [ ! -s clang_tidy_output.txt ]
  cd ../../..

else

  # Get the templight binary

  mkdir -p "bin/${PLATFORM_ID}"
  cd "bin/${PLATFORM_ID}"
    ARCHIVE_NAME=templight_${PLATFORM_ID}.tar.bz2
    wget https://github.com/metashell/templight_binary/releases/download/templight_9732a7/${ARCHIVE_NAME}
    tar -xvjf ${ARCHIVE_NAME}
    mv build templight
  cd ../..

  # Test the code

  export CXXFLAGS="-Werror"

  NO_TEMPLIGHT=1 METASHELL_NO_DOC_GENERATION=1 ./build.sh
fi
