#!/bin/bash

set -ex

brew update
brew install homebrew/versions/gcc5

if [ "$CXX" == "g++" ]; then export CXX="g++-5"; fi

# Test the code

mkdir bin
cd bin

cmake .. -DCMAKE_CXX_FLAGS:STRING="-Werror" -DTEMPLIGHT_DEBUG=true
make -j2
make test || (cat Testing/Temporary/LastTest.log && false)

# Run the system tests

test/system/app/core/metashell_core_system_test app/metashell -I../3rd/boost/include

# Test that the documentation about the built-in pragmas and mdb commands is up to date

app/metashell --show_pragma_help | ../tools/replace_part -i ../docs/reference/pragmas.md -m '<!-- pragma_info -->' -o - -r - | diff ../docs/reference/pragmas.md -
app/metashell --show_mdb_help | ../tools/replace_part -i ../docs/reference/mdb_commands.md -m '<!-- mdb_info -->' -o - -r - | diff ../docs/reference/mdb_commands.md -
