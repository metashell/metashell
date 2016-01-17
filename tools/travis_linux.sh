#!/bin/bash

function python_files {
  for f in $1/*
  do
    if [ -f "$f" ] && (head -1 "$f" | egrep '^#!.*python' > /dev/null)
    then
      echo $f
    fi
  done
}

function assert_no_git_tracked_files_changed {
  if ! git diff-index --quiet HEAD --; then
    echo "Diff found:"
    git diff
    return 1
  fi
  return 0
}

set -ex

if [ "$CXX" == "g++" ]; then
  export CXX="g++-4.8";
fi

# Test that code formatting is correct
tools/reformat.sh
assert_no_git_tracked_files_changed

# Test that the download version links are correct

git fetch --tags
git tag
egrep $(tools/latest_release --no_dots --prefix=version-) README.md
egrep $(tools/latest_release --no_dots --prefix=version-) docs/index.md

# Check the python scripts

sudo pip install pep8 pylint
pep8 $(python_files tools)
pylint --disable=bare-except $(python_files tools)

# Test mkdocs.yml

tools/check_mkdocs mkdocs.yml

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

# Test that the documentation about the built-in pragmas and mdb commands is up to date

app/metashell --show_pragma_help | ../tools/replace_part -i ../docs/reference/pragmas.md -m '<!-- pragma_info -->' -o - -r - | diff ../docs/reference/pragmas.md -
app/metashell --show_mdb_help | ../tools/replace_part -i ../docs/reference/mdb_commands.md -m '<!-- mdb_info -->' -o - -r - | diff ../docs/reference/mdb_commands.md -
