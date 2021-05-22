#!/usr/bin/python3
"""Run a command in the background and display a message on stderr
periodically"""

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

import argparse
import sys
import subprocess
import threading


class Condition:
    """Wraps a condition variable"""

    def __init__(self):
        self._cond = threading.Condition()
        self._value = False

    def wait_set(self, timeout_sec):
        """Waits for the condition to be set. Returns if it was set within the
        timeout"""
        with self._cond:
            if not self._value:
                self._cond.wait(timeout=timeout_sec)
        return self._value

    def set(self):
        """Sets the value"""
        with self._cond:
            self._value = True
            self._cond.notifyAll()

    def __enter__(self):
        return self

    def __exit__(self, typ, value, traceback):
        self.set()


def do_notifying(interval_sec, msg, running):
    """The thread displaying notifications"""
    while not running.wait_set(interval_sec):
        sys.stderr.write(msg + '\n')


def main():
    """The main function of the utility"""
    try:
        sep_at = sys.argv.index('--')
    except ValueError:
        print('Usage: {0} <args> -- <command to run>'.format(sys.argv[0]))
        sys.exit(1)

    cmd = sys.argv[sep_at+1:]

    parser = argparse.ArgumentParser(description=sys.modules[__name__].__doc__)
    parser.add_argument(
        '--period_sec',
        required=True,
        type=int,
        help='The delay between two messages'
    )
    parser.add_argument(
        '--message',
        required=False,
        default='Still running ' + ' '.join(cmd),
        help='The message to display'
    )
    args = parser.parse_args(sys.argv[1:sep_at])

    with Condition() as running:
        bg_thread = threading.Thread(
            target=do_notifying,
            args=(args.period_sec, args.message, running)
        )
        bg_thread.start()

        with subprocess.Popen(
            cmd,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE
        ) as proc:
            out, err = proc.communicate()

    bg_thread.join()

    sys.stdout.write(out.decode('utf-8'))
    sys.stderr.write(err.decode('utf-8'))
    sys.exit(proc.returncode)


if __name__ == '__main__':
    main()
