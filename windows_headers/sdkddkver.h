/**
 * @file sdkddkver.h
 * Copyright 2012, 2013 MinGW.org project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef _SDKDDKVER_H
#define _SDKDDKVER_H
#pragma GCC system_header
/* Do not include _mingw.h since _mingw.h includes this file.
#include <_mingw.h>
*/

/**
 * Define version masks
 * http://msdn.microsoft.com/en-us/library/windows/desktop/aa383745(v=vs.85).aspx
 * Values and names are guessed based on comments in the documentation.
 */
#define OSVERSION_MASK            0xFFFF0000
#define SPVERSION_MASK            0x0000FF00
#define SUBVERSION_MASK           0x000000FF

/**
 * Macros to extract values from NTDDI version.
 * Derived from comments on MSDN or social.microsoft.com
 */
#define OSVER(ver) ((ver) & OSVERSION_MASK)
#define SPVER(ver) (((ver) & SPVERSION_MASK) >> 8)
#define SUBVER(ver) ((ver) & SUBVERSION_MASK)
#define WINNTVER(ver) ((ver) / 0x00010000)

/**
 * Macros to create the minimal NTDDI version from _WIN32_WINNT value.
 */
#define NTDDI_VERSION_FROM_WIN32_WINNT(ver) _NTDDI_VERSION_FROM_WIN32_WINNT(ver)
#define _NTDDI_VERSION_FROM_WIN32_WINNT(ver) ver##0000

/**
 * Version constants defining _WIN32_WINNT versions.
 * http://msdn.microsoft.com/en-us/library/windows/desktop/aa383745(v=vs.85).aspx
 */
#define _WIN32_WINNT_NT4          0x0400
#define _WIN32_WINNT_NT4E         0x0401
#define _WIN32_WINNT_WIN95        0x0400
#define _WIN32_WINNT_WIN98        0x0410
#define _WIN32_WINNT_WINME        0x0490
#define _WIN32_WINNT_WIN2K        0x0500
#define _WIN32_WINNT_WINXP        0x0501
#define _WIN32_WINNT_WS03         0x0502
#define _WIN32_WINNT_WIN6         0x0600
#define _WIN32_WINNT_VISTA        0x0600
#define _WIN32_WINNT_WS08         0x0600
#define _WIN32_WINNT_LONGORN      0x0600
#define _WIN32_WINNT_WIN7         0x0601
#define _WIN32_WINNT_WIN8         0x0602

/**
 * Version constants defining _WIN32_IE versions.
 * http://msdn.microsoft.com/en-us/library/windows/desktop/aa383745(v=vs.85).aspx
 */
#define _WIN32_IE_IE50            0x0500
#define _WIN32_IE_IE501           0x0501
#define _WIN32_IE_IE55            0x0550
#define _WIN32_IE_IE60            0x0600
#define _WIN32_IE_IE60SP1         0x0601
#define _WIN32_IE_IE60SP2         0x0603
#define _WIN32_IE_IE70            0x0700
#define _WIN32_IE_IE80            0x0800

/**
 * Version constants defining NTDDI_VERSION.
 * http://msdn.microsoft.com/en-us/library/windows/desktop/aa383745(v=vs.85).aspx
 */
#define __NTDDI_WIN5               0x05000000
#define __NTDDI_WIN51              0x05010000
#define __NTDDI_WIN52              0x05020000
#define __NTDDI_WIN6               0x06000000
#define __NTDDI_WIN61              0x06010000
#define __NTDDI_SP0                0x00000000
#define __NTDDI_SP1                0x00000100
#define __NTDDI_SP2                0x00000200
#define __NTDDI_SP3                0x00000300
#define __NTDDI_SP4                0x00000400

#define NTDDI_WIN2K               __NTDDI_WIN5 + __NTDDI_SP0
#define NTDDI_WIN2KSP1            __NTDDI_WIN5 + __NTDDI_SP1
#define NTDDI_WIN2KSP2            __NTDDI_WIN5 + __NTDDI_SP2
#define NTDDI_WIN2KSP3            __NTDDI_WIN5 + __NTDDI_SP3
#define NTDDI_WIN2KSP4            __NTDDI_WIN5 + __NTDDI_SP4

#define NTDDI_WINXP               __NTDDI_WIN51 + __NTDDI_SP0
#define NTDDI_WINXPSP1            __NTDDI_WIN51 + __NTDDI_SP1
#define NTDDI_WINXPSP2            __NTDDI_WIN51 + __NTDDI_SP2
#define NTDDI_WINXPSP3            __NTDDI_WIN51 + __NTDDI_SP3

#define NTDDI_WS03                __NTDDI_WIN52 + __NTDDI_SP0
#define NTDDI_WS03SP1             __NTDDI_WIN52 + __NTDDI_SP1
#define NTDDI_WS03SP2             __NTDDI_WIN52 + __NTDDI_SP2

#define NTDDI_VISTA               __NTDDI_WIN6 + __NTDDI_SP0
#define NTDDI_VISTASP1            __NTDDI_WIN6 + __NTDDI_SP1
#define NTDDI_VISTASP2		  __NTDDI_WIN6 + __NTDDI_SP2

#define NTDDI_LONGHORN            NTDDI_VISTA

#define NTDDI_WIN6                NTDDI_VISTA
#define NTDDI_WIN6SP1             NTDDI_VISTASP1
#define NTDDI_WIN6SP2		  NTDDI_VISTASP2

#define NTDDI_WS08                __NTDDI_WIN6 + __NTDDI_SP1

#define NTDDI_WIN7                __NTDDI_WIN61 + __NTDDI_SP0

/**
 * Assign defaults
 */
#ifdef NTDDI_VERSION
#  ifdef _WIN32_WINNT
#    if _WIN32_WINNT != OSVER(NTDDI_VERSION)
#      error The _WIN32_WINNT value does not match NTDDI_VERSION
#    endif
#  else
#    define _WIN32_WINNT WINNTVER(NTDDI_VERSION)
#    ifndef WINVER
#      define WINVER _WIN32_WINNT
#    endif
#  endif
#endif

#ifndef _WIN32_WINNT
#  ifdef WINVER
#    define _WIN32_WINNT WINVER
#  else
#    ifdef _WARN_DEFAULTS
#      warning _WIN32_WINNT is defaulting to _WIN32_WINNT_WIN2K
#    endif
#    define _WIN32_WINNT _WIN32_WINNT_WIN2K
#  endif
#endif

#ifndef WINVER
#  define WINVER _WIN32_WINNT
#endif

#ifndef NTDDI_VERSION
#  ifdef _WARN_DEFAULTS
#    warning NTDDI_VERSION is defaulting to _WIN32_WINNT version SPK0
#  endif
#  define NTDDI_VERSION NTDDI_VERSION_FROM_WIN32_WINNT(_WIN32_WINNT)
#endif

#endif
