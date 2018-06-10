#!/usr/bin/python
"""A stub to use as a metashell binary replacement"""

# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

import sys
import json


def load_file(path):
    """Load the content of the file"""
    with open(path, 'r') as in_f:
        return in_f.read()


def referenced_files(args):
    """Generator for the list of files referenced by the args"""
    keep_next = False
    for arg in args:
        if arg == '--':
            return
        elif keep_next:
            yield arg
            keep_next = False
        elif arg == '--load_configs':
            keep_next = True


def determine_output(args):
    """Determine the output of this utility given the arguments"""
    sep_at = args.index('--')
    return {
        'metashell_args': args[:sep_at],
        'clang_args': args[sep_at+1:],
        'files': {f: load_file(f) for f in referenced_files(args)}
    }


def main():
    """The main function of the utility"""
    print json.dumps(determine_output(sys.argv))


if __name__ == '__main__':
    main()
