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

mkdir bin
cd bin

cmake .. -DCMAKE_CXX_FLAGS:STRING="-Werror" -DTEMPLIGHT_DEBUG=true
make -j2
make test || (cat Testing/Temporary/LastTest.log && false)

for t in core pp mdb; do
  test/system/app/${t}/metashell_${t}_system_test \
    app/metashell -- -I$(readlink -m ../3rd/boost/include) --

  test/system/app/${t}/metashell_${t}_system_test \
    app/metashell \
    --engine clang \
    -- \
    $(readlink -m app/templight_metashell) \
    -std=c++0x \
    -ftemplate-depth=256 \
    -Wfatal-errors \
    -I$(readlink -m include/metashell/libcxx) \
    -I$(readlink -m include/metashell/templight) \
    -I$(readlink -m ../3rd/boost/include) \
    --
done

# Test that the documentation about the built-in pragmas and mdb commands is up to date

app/metashell --show_pragma_help | ../tools/replace_part -i ../docs/reference/pragmas.md -m '<!-- pragma_info -->' -o - -r - | diff ../docs/reference/pragmas.md -
app/metashell --show_mdb_help | ../tools/replace_part -i ../docs/reference/mdb_commands.md -m '<!-- mdb_info -->' -o - -r - | diff ../docs/reference/mdb_commands.md -
