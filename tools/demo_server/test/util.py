#!/usr/bin/python
"""Utilities for testing"""

# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

import errno
import imp
import inspect
import json
import os
import shutil
import sys
import tempfile
import git


def directory_of_this_script():
    """Returns the path to the directory of the current script"""
    return os.path.dirname(
        os.path.abspath(inspect.getfile(inspect.currentframe()))
    )


def load_from_bin(name):
    """Loads the file called name from the bin directory"""
    # Loading the code into a string and compiling that to avoid the automatic
    # bytecode file (ending with c) generation as a side-effect of testing.
    module_path = os.path.join(directory_of_this_script(), '..', 'bin', name)
    with open(module_path) as modulefile:
        code = modulefile.read()
    module = imp.new_module(name)
    # pylint: disable=exec-used
    exec code in module.__dict__
    # pylint: enable=exec-used
    return module


def commit(repo, files, msg='Some commit'):
    """Write the files, add them and commit the changes. Returns sha of the
    commit."""
    for name, content in files.iteritems():
        repo.write_file(name, content)
        repo.add(name)
    return repo.commit(msg)


class TempDir(object):
    """Temporary directory"""

    def __init__(self):
        self.path = None

    def __enter__(self):
        self.path = tempfile.mkdtemp()
        return self

    def __exit__(self, typ, value, traceback):
        shutil.rmtree(self.path)
        self.path = None

    def relative(self, path):
        """Return the absolute path relative to the temp directory"""
        return os.path.join(self.path, path)

    def exists(self, path):
        """Check if the path exists"""
        return os.path.isfile(self.relative(path))

    def write(self, path, content):
        """Write content into the file"""
        with open(self.relative(path), 'w') as out_file:
            out_file.write(content)

    def read(self, path):
        """Read the content of the file"""
        with open(self.relative(path), 'r') as in_file:
            return in_file.read()


class GitRepository(object):
    """Temporary git repository"""

    def __init__(self):
        self.temp_dir = TempDir()
        self.repo = None
        self.submodule_paths = {}

    def in_repo(self, path):
        """Turns a path relative to the repository to an absolute path"""
        return os.path.join(self.temp_dir.path, path)

    def write_file(self, path, content):
        """Create/write into a file in the repository"""
        full_path = self.in_repo(path)
        dirname = os.path.dirname(full_path)
        if dirname != '':
            try:
                os.makedirs(dirname)
            except OSError, err:
                if err.errno != errno.EEXIST:
                    raise
        with open(full_path, 'w') as out_file:
            out_file.write(content)

    def add(self, path):
        """Add a file"""
        self.repo.index.add([path])

    def commit(self, msg):
        """Make a git commit. Returns sha of the new commit"""
        return self.repo.index.commit(msg).hexsha

    def repository(self):
        """Return the "git clone" argument for cloning this repository"""
        return self.temp_dir.path

    def create_branch(self, name):
        """Create a new branch"""
        self.repo.create_head(name)

    def checkout_branch(self, name, create=False):
        """Checkout a branch"""
        if create:
            self.create_branch(name)
        self.repo.heads[name].checkout()

    def delete_branch(self, name):
        """Delete a branch"""
        self.repo.delete_head(name, force=True)

    def add_submodule(self, name, path, sub_repository):
        """Add a Git submodule"""
        self.repo.create_submodule(name, path, url=sub_repository.repository())
        self.submodule_paths[name] = path

    def pull_submodule(self, name):
        """Git pull a submodule"""
        submod = self.repo.submodule(name)
        path = self.submodule_paths[name]
        url = submod.url
        submod.remove()
        self.repo.create_submodule(name, path, url)

    def __enter__(self):
        self.temp_dir.__enter__()
        git.Repo.init(self.temp_dir.path)
        self.repo = git.Repo(self.temp_dir.path)
        return self

    def __exit__(self, typ, value, traceback):
        self.temp_dir.__exit__(typ, value, traceback)


class LogCollector(object):
    """Logging functor collecting the logs in a buffer"""
    def __init__(self):
        self.messages = []

    def append(self, msg):
        """Log a message"""
        self.messages.append(msg)

    def clear(self):
        """Clear log messages"""
        self.messages = []

    def dump(self, target=sys.stderr):
        """Dump log messages"""
        target.write('\n\n')
        target.write('\n'.join(self.messages))
        target.write('\n\n')

    def __call__(self, msg):
        prefix = '=========='
        if not (msg.startswith(prefix) and msg.endswith(prefix)):
            self.append(msg)


class CommonEnv(object):
    """A common test environment"""

    def __init__(self, git_repo_num):
        self.log = LogCollector()
        self.repos = [GitRepository() for _ in range(git_repo_num)]
        self.config_dir = TempDir()
        self.out_dir = TempDir()
        self.in_with = False

    def write_config(self, content):
        """Dump content as JSON into the config file"""
        self.config_dir.write('config.json', json.dumps(content))

    def commit(self, repo_index, files):
        """Commit files to a repository"""
        return commit(self.repos[repo_index], files)

    def repository(self, repo_index):
        """Return the "git clone" argument for cloning the repository"""
        return self.repos[repo_index].repository()

    def destroy_repository(self, repo_index):
        """Remove and clean-up the repository"""
        if self.in_with:
            self.repos[repo_index].__exit__(None, None, None)
        self.repos[repo_index] = None

    def __enter__(self):
        for repo in self.repos:
            if repo:
                repo.__enter__()
        self.config_dir.__enter__()
        self.out_dir.__enter__()

        self.in_with = True

        return self

    def __exit__(self, typ, value, traceback):
        self.out_dir.__exit__(typ, value, traceback)
        self.config_dir.__exit__(typ, value, traceback)
        for repo in self.repos:
            if repo:
                repo.__exit__(typ, value, traceback)

        self.in_with = False


def delete_everything_in(path):
    """Delete all files and directories in path"""
    for filename in os.listdir(path):
        full_path = os.path.join(path, filename)
        if os.path.isdir(full_path):
            shutil.rmtree(full_path)
        else:
            os.unlink(full_path)
