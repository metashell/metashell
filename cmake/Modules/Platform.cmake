# Metashell - Interactive C++ template metaprogramming shell
# Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

# Including this file defines the following:

#  SUSE_LINUX on SuSE
#  FEDORA_LINUX on Fedora and Red Hat
#  UBUNTU_LINUX on Ubuntu
#  FREE_BSD on FreeBSD
#  OPEN_BSD on OpenBSD

#  PLATFORM_CPACK_GENERATOR: the platform's native CPACK_GENERATOR value where
#                            available
#  PLATFORM_NAME:            the name of the current platform
#  PLATFORM_VERSION:         the distribution's version

if (WIN32)
  set(PLATFORM_NAME "windows")
  set(PLATFORM_CPACK_GENERATOR "NSIS")
elseif ( EXISTS "/etc/SuSE-release" )
  set(SUSE_LINUX true)
  set(PLATFORM_CPACK_GENERATOR "RPM")
  set(PLATFORM_NAME "opensuse")

  file(READ "/etc/SuSE-release" SUSE_RELEASE)
  string(REGEX MATCH "VERSION[ ]*=[^\n]*" VS "${SUSE_RELEASE}")
  string(REGEX MATCH "[0-9.]+" PLATFORM_VERSION "${VS}")
elseif ( EXISTS "/etc/redhat-release" OR EXISTS "/etc/fedora-release" )
  set(FEDORA_LINUX true)
  set(PLATFORM_CPACK_GENERATOR "RPM")
  set(PLATFORM_NAME "fedora")
  execute_process(
    COMMAND rpm -q --queryformat '%{VERSION}\n' fedora-release
    OUTPUT_VARIABLE PV
  )
  string(REGEX MATCH "[0-9.]+" PLATFORM_VERSION "${PV}")
elseif (EXISTS "/etc/lsb-release")
  file(READ "/etc/lsb-release" LSB_RELEASE)
  string(REGEX MATCH "DISTRIB_ID=[^\n]*" D_ID "${LSB_RELEASE}")
  if ("${D_ID}" STREQUAL "DISTRIB_ID=Ubuntu")
    set(UBUNTU_LINUX true)
    set(PLATFORM_CPACK_GENERATOR "DEB")
    set(PLATFORM_NAME "ubuntu")

    string(REGEX MATCH "DISTRIB_RELEASE=[^\n]*" D_R "${LSB_RELEASE}")
    string(REGEX MATCH "[0-9.]+" PLATFORM_VERSION "${D_R}")
  endif ()
else()
  execute_process(COMMAND uname OUTPUT_VARIABLE UN)
  string(STRIP "${UN}" UNAME)
  if ("${UNAME}" STREQUAL "FreeBSD")
    set(FREE_BSD true)
    set(PLATFORM_NAME "freebsd")
  elseif ("${UNAME}" STREQUAL "OpenBSD")
    set(OPEN_BSD true)
    set(PLATFORM_NAME "openbsd")
  endif()
endif ()

message(STATUS "Platform: ${PLATFORM_NAME} ${PLATFORM_VERSION}")
message(STATUS "Default cpack generator: ${PLATFORM_CPACK_GENERATOR}")

