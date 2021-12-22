#!/usr/bin/python3
# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2021, Abel Sinkovics (abel@sinkovics.hu)
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
import re
import urllib.request
import json


RES = [
    re.compile('set\\(MAJOR_VERSION ([^)]*)\\)'),
    re.compile('set\\(MINOR_VERSION ([^)]*)\\)'),
    re.compile('set\\(PATCH_VERSION ([^)]*)\\)')
]


def set_output(name, value):
    """Set a specific output"""
    sys.stdout.write(f'##[set-output name={name};]{value}\n')


def determine_version(cmakelists):
    """Determine the version"""
    version = ['', '', '']

    with open(cmakelists, 'r') as in_f:
       for line in in_f:
           for nth, regex in enumerate(RES):
               match = regex.search(line)
               if match:
                   version[nth] = match.group(1)

    return '.'.join(version)


def determine_upload_url(repo, release):
    """Determine the upload URL"""
    return json.loads(urllib.request.urlopen(
        f'https://api.github.com/repos/{repo}/releases/tags/{release}'
    ).read())['upload_url']


def main():
    """The main function of this utility"""
    version = determine_version('CMakeLists.txt')
    upload_url = determine_upload_url(sys.argv[1], sys.argv[2])

    set_output('tag', version)
    set_output('upload_url', upload_url)


if __name__ == '__main__':
    main()
