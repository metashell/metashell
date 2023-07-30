# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2022, Abel Sinkovics (abel@sinkovics.hu)
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

project(readline)

aux_source_directory(src SOURCES)

# These c files are included directly keymaps.c intentionally
list(REMOVE_ITEM SOURCES src/vi_keymap.c src/emacs_keymap.c)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

disable_warnings(${PROJECT_NAME})

target_include_directories(
  ${PROJECT_NAME} SYSTEM
  INTERFACE ${PROJECT_SOURCE_DIR}/include
  PRIVATE ${PROJECT_SOURCE_DIR}/include/readline
)

target_compile_definitions(
  ${PROJECT_NAME}
  
  PRIVATE
    READLINE_LIBRARY

    TERMIOS_TTY_DRIVER
    VOID_SIGHANDLER

    HAVE_DIRENT_H
    HAVE_GETTIMEOFDAY
    HAVE_LOCALE_H
    HAVE_POSIX_SIGNALS
    HAVE_PSELECT
    HAVE_PWD_H
    HAVE_STDARG_H
    HAVE_STDLIB_H
    HAVE_STRING_H
    HAVE_SYS_FILE_H
    HAVE_SYS_IOCTL_H
    HAVE_TIMEVAL
    HAVE_UNISTD_H
)
