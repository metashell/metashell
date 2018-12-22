#!/usr/bin/python3
"""Utility to display library dependencies"""

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

# Disable the following to make this file pass with Python 2.7 pylint
# pylint: disable=superfluous-parens,import-error,undefined-variable


import argparse
import os
import re
import itertools
import copy
import sys
import subprocess


def source_files_of_lib(source_root, lib_name):
    """Generator of the source files of the given library"""
    for path_to_walk in (os.path.join(source_root, 'lib'), source_root):
        for path, _, files in os.walk(os.path.join(path_to_walk, lib_name)):
            for filename in files:
                if filename.endswith('.hpp') or filename.endswith('.cpp'):
                    yield os.path.join(path, filename)


def source_lines_of_lib(source_root, lib_name):
    """Generator for all source code lines of a library"""
    for path in source_files_of_lib(source_root, lib_name):
        with open(path, 'r') as in_f:
            for line in in_f.readlines():
                yield line


def included_by_lib(source_root, lib_name):
    """Returns the files included by the given library"""
    include_line = re.compile(
        '^[ \t]*#[ \t]*include[ \t]+(<[^>]*>|"[^"]")[ \t\n\r]*$'
    )
    for line in source_lines_of_lib(source_root, lib_name):
        match = include_line.match(line)
        if match:
            yield match.group(1)


def dependencies_of_lib(source_root, libs, lib_name):
    """Generator of the #include dependencies of the library"""
    inc_lib = re.compile('^[<"](metashell[\\\\/])?([^>"]*).*$')
    for path in included_by_lib(source_root, lib_name):
        match = inc_lib.match(path)
        if \
                match and \
                not (path.startswith('<boost') or path.startswith('"boost')):
            dep_lib = match.group(2)
            dep_lib = dep_lib[:dep_lib.rfind('/')]
            if dep_lib != lib_name and dep_lib in libs:
                yield dep_lib


def subdirs_of(path):
    """Returns the subdirectories of the given path"""
    return next(os.walk(path))[1]


def subpaths_containing_sources(path):
    """Generator of subpaths containing source files"""
    is_source = re.compile('.*\\.[hc]pp$')
    return (
        p[len(path) + 1:] for p, _, fs in os.walk(path)
        if any(is_source.match(f) for f in fs)
    )


def sublib_of(sub, outer):
    """Checks if sub is a sublib of outer"""
    louter = len(outer)
    return \
        len(sub) > louter \
        and sub.startswith(outer) \
        and sub[louter] in ('\\', '/')


def remove_nested(libs):
    """Removes nested libs (eg. when x is in libs, x/y gets removed)"""
    result = set()
    for lib in sorted(libs, key=len):
        if not any(sublib_of(lib, l) for l in result):
            result.add(lib)
    return result


def prefix_path(prefix, generator):
    """Generator prefixing all elements of another generator"""
    for item in generator:
        yield os.path.join(prefix, item)


def collect_dependencies(source_root):
    """Collect all the dependencies"""
    lib_path = os.path.join(source_root, 'lib')
    libs = sorted(
        p[len(lib_path) + 1:-len('/include')] for p, _, fs in os.walk(lib_path)
        if p.endswith('/include')
    )
    apps = sorted(remove_nested(itertools.chain(
        prefix_path(
            'app',
            subpaths_containing_sources(os.path.join(source_root, 'app'))
        ),
        prefix_path(
            'test',
            subpaths_containing_sources(os.path.join(source_root, 'test'))
        )
    )))
    return (
        {l: set(dependencies_of_lib(source_root, libs, l)) for l in libs},
        {a: set(dependencies_of_lib(source_root, libs, a)) for a in apps}
    )


def back_edges(deps):
    """Find the back edges of the graph"""
    start = {}
    end = {}
    stack = list(deps.keys())
    step = 0
    while stack:
        snode = stack[-1]
        if snode not in start:
            start[snode] = step
            for tnode in deps[snode]:
                if tnode not in start:
                    stack.append(tnode)
        else:
            if snode not in end:
                end[snode] = step
            stack = stack[:-1]
        step += 1

    return {
        u: set(v for v in deps[u] if start[u] > start[v] and end[u] < end[v])
        for u in deps.keys()
    }


def merge_into(dst, additional):
    """Merge a graph into another one"""
    for snode, tnodes in additional.items():
        if snode not in dst:
            dst[snode] = set()
        dst[snode].update(tnodes)


def remove_from(src, additional):
    """Remove the edges of a graph from another one"""
    for snode, tnodes in additional.items():
        src[snode].difference_update(tnodes)


def one_path(deps, start, end):
    """Finds one path from start to end. Returns None when there is no path."""
    visited = set()
    max_len = len(deps) + 1
    dist = {k: 0 if k == start else max_len for k in deps.keys()}
    to_visit = [start]
    while to_visit:
        snode = to_visit[0]
        to_visit = to_visit[1:]
        if snode not in visited:
            visited.add(snode)
            to_visit.extend(deps[snode])
            dfrom_here = dist[snode] + 1
            for tnode in deps[snode]:
                dist[tnode] = min(dist[tnode], dfrom_here)
    if dist[end] < max_len:
        result = {}
        at_node = end
        while at_node != start:
            node = None
            for snode, tnodes in deps.items():
                if \
                        at_node in tnodes \
                        and (node is None or dist[snode] < dist[node]):
                    node = snode
            if node is None:
                return None
            else:
                result[node] = set([at_node])
                at_node = node
        return result
    return None


def all_paths(deps, start, end):
    """Return all paths between the given nodes"""
    grp = copy.deepcopy(deps)
    result = {}
    while True:
        path = one_path(grp, start, end)
        if path is None:
            return result
        else:
            merge_into(result, path)
            remove_from(grp, path)


def find_circular_dependencies(deps):
    """Find the dependencies participating in a circular dependency"""
    backs = back_edges(deps)
    result = copy.deepcopy(backs)
    for snode, tnodes in backs.items():
        for tnode in tnodes:
            merge_into(result, all_paths(deps, tnode, snode))
    return result


def reachable_in_two_steps(graph, src):
    """Returns the list of nodes reachable from src in at most two steps"""
    dsts = graph.get(src, [])
    result = set(dsts)
    for dst in dsts:
        result.update(graph.get(dst, []))
    return list(sorted(result))


def transitive_closure(graph):
    """Returns the transitive closure of the graph"""
    prev = None
    result = graph
    while prev != result:
        prev = result
        result = {n: reachable_in_two_steps(prev, n) for n in prev.keys()}
    return result


def edges(graph):
    """Generator of all edges of the graph"""
    for src in sorted(graph.keys()):
        for dst in sorted(graph[src]):
            yield (src, dst)


def remove_transitive_edges(graph):
    """Returns a subgraph, in which there is a path between two nodes if any
    only if there is one in the original one as well."""
    reachable = transitive_closure(graph)
    prev = None
    result = graph
    while prev != result:
        prev = result
        result = copy.deepcopy(prev)
        for src, dst in edges(prev):
            result[src].remove(dst)
            if transitive_closure(result) != reachable:
                result[src].append(dst)
    return result


class Dependencies(object):
    """Represents a set of dependencies"""

    def __init__(self, source_root):
        lib_deps, app_deps = collect_dependencies(source_root)

        self.libs = sorted(lib_deps.keys())
        self.apps = sorted(app_deps.keys())
        self.deps = {k: list(sorted(v)) for k, v in lib_deps.items()}
        for key, vals in app_deps.items():
            if key in self.deps:
                self.deps[key] = list(sorted(self.deps[key] + vals))
            else:
                self.deps[key] = list(sorted(vals))
        self.deps = remove_transitive_edges(self.deps)

        self.circular = find_circular_dependencies(lib_deps)
        self.circular_libs = \
            set(k for k, v in self.circular.items() if len(v) > 0)
        for tnodes in self.circular.values():
            self.circular_libs.update(tnodes)

    def is_circular_dependency(self, item, dependee):
        """Determines if the given dependency is part of a circle"""
        return item in self.circular and dependee in self.circular[item]

    def to_dot(self):
        """Generates the dot representation of the dependencies"""
        result = ['digraph {']

        for lib in self.libs:
            is_circular = lib in self.circular_libs
            result.append(
                '  "{0}" [shape=oval, fillcolor={1}, style=filled];'.format(
                    lib,
                    'red' if is_circular else 'white'
                )
            )

        for app in self.apps:
            result.append('  "{0}" [shape=rect];'.format(app))

        result.append('')

        for key in itertools.chain(self.libs, self.apps):
            for dep in self.deps[key]:
                result.append(
                    '  "{0}" -> "{1}" [color={2}];'.format(
                        key,
                        dep,
                        'red' if self.is_circular_dependency(key, dep)
                        else 'black'
                    )
                )

        result.append('}')

        return '\n'.join(result)


def platform_id(source_root):
    """Determine the PLATFORM_ID"""
    return subprocess.check_output(
        [os.path.join(source_root, 'tools', 'detect_platform.sh'), '--id']
    ).decode('utf-8').strip()


def path_of_dot(source_root):
    """Determines the path of the dot command"""
    return os.path.join(
        source_root, 'bin', platform_id(source_root), 'graphviz', 'bin', 'dot'
    )


def main():
    """The main utility of the application"""
    parser = argparse.ArgumentParser(
        description='Utility to display library dependencies'
    )
    parser.add_argument(
        '--source_root',
        required=True,
        help='The root of the source tree'
    )
    parser.add_argument(
        '--out',
        choices=['svg', 'dot', 'circular_deps'],
        default='svg',
        help='The output format'
    )
    args = parser.parse_args()

    deps = Dependencies(args.source_root)

    if args.out == 'dot':
        print(deps.to_dot())
    elif args.out == 'svg':
        out, _ = subprocess.Popen(
            [path_of_dot(args.source_root), '-Tsvg'],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE
        ).communicate(deps.to_dot().encode('utf-8'))
        sys.stdout.write(
            re.sub(
                '(Generated by graphviz version [0-9.]*) \\([^)]*\\)',
                '\\1',
                out.decode('utf-8')
            )
        )
    elif args.out == 'circular_deps':
        if deps.circular_libs:
            print()
            print()
            print(
                'The following libraries have circular'
                ' #include dependencies:\n'
            )
            print('\n'.join(sorted(deps.circular_libs)))
            print()
            print()
            sys.exit(2)
    else:
        print('Unsupported output format: {0}'.format(args.out))
        sys.exit(1)


if __name__ == '__main__':
    main()
