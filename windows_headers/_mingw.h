/**
 * @file _mingw.h
 * Copyright (C) 2012 MinGW.org Project
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

/* -------------------------------------------------------------------------
 * Implementation of MinGW specific macros; these are to be included by
 * all other WSL header files.
 * -------------------------------------------------------------------------
 */
#ifndef __MINGW_H
#define __MINGW_H
#ifndef __CRT_TESTING__
#pragma GCC system_header
#endif
#include <sdkddkver.h>

#define __MINGW_VERSION             4.0
#define __MINGW_MAJOR_VERSION       4
#define __MINGW_MINOR_VERSION       0
#define __MINGW_PATCHLEVEL          0

/* The following four macros are deprecated and will be removed
 * in the release greater than 4.1.
 */
#define __MINGW32_VERSION           3.20
#define __MINGW32_MAJOR_VERSION     3
#define __MINGW32_MINOR_VERSION     20
#define __MINGW32_PATCHLEVEL        0

#ifndef __GNUC__
#error ERROR: You must use a GNU Compiler.
#endif

#if (__GNUC__ < 3 || !defined(__GNUC_MINOR__) || (__GNUC__ == 3 && __GNUC_MINOR__ < 4) || (__GNUC__ == 3 && __GNUC_MINOR__ == 4 && __GNUC_PATCHLEVEL__ < 5))
#error ERROR: You must use a GNU Compiler version >= 3.4.5.
#endif

/* These are defined by the user (or the compiler)
 * to specify how identifiers are imported from a DLL.
 *
 * __MINGW_IMPORT                  The attribute definition to specify imported
 *                                 variables/functions.
 * _CRTIMP                         As above.  For MS compatibility.
 * __MINGW_VERSION                 Runtime version.
 * __MINGW_MAJOR_VERSION           Runtime major version.
 * __MINGW_MINOR_VERSION           Runtime minor version.
 * __MINGW_BUILD_DATE              Runtime build date.
 *
 * Macros to enable MinGW features which deviate from standard MSVC
 * compatible behaviour; these may be specified directly in user code,
 * activated implicitly, (e.g. by specifying _POSIX_C_SOURCE or such),
 * or by inclusion in __MINGW_FEATURES__:
 *
 * __USE_MINGW_ANSI_STDIO          Select a more ANSI C99 compatible
 *                                 implementation of printf() and friends.
 *
 * Other macros:
 *
 * __int64                         define to be long long.  Using a typedef
 *                                 doesn't work for "unsigned __int64"
 *
 *
 * Manifest definitions for flags to control globbing of the command line
 * during application start up, (before main() is called).  The first pair,
 * when assigned as bit flags within _CRT_glob, select the globbing algorithm
 * to be used; (the MINGW algorithm overrides MSCVRT, if both are specified).
 * Prior to mingwrt-3.21, only the MSVCRT option was supported; this choice
 * may produce different results, depending on which particular version of
 * MSVCRT.DLL is in use; (in recent versions, it seems to have become
 * definitively broken, when globbing within double quotes).
 */
#define __CRT_GLOB_USE_MSVCRT__  	0x0001

/* From mingwrt-3.21 onward, this should be the preferred choice; it will
 * produce consistent results, regardless of the MSVCRT.DLL version in use.
 */
#define __CRT_GLOB_USE_MINGW__   	0x0002

/* When the __CRT_GLOB_USE_MINGW__ flag is set, within _CRT_glob, the
 * following additional options are also available, (but are not enabled
 * by default):
 *
 *    __CRT_GLOB_USE_SINGLE_QUOTE__	allows use of single (apostrophe)
 *    					quoting characters, analogously to
 *    					POSIX usage, as an alternative to
 *    					double quotes, for collection of
 *    					arguments separated by white space
 *    					into a single logical argument.
 *
 *    __CRT_GLOB_BRACKET_GROUPS__	enable interpretation of bracketed
 *    					character groups as POSIX compatible
 *    					globbing patterns, matching any one
 *    					character which is either included
 *    					in, or excluded from the group.
 *
 *    __CRT_GLOB_CASE_SENSITIVE__	enable case sensitive matching for
 *    					globbing patterns; this is default
 *    					behaviour for POSIX, but because of
 *    					the case insensitive nature of the
 *    					MS-Windows file system, it is more
 *    					appropriate to use case insensitive
 *    					globbing as the MinGW default.
 *
 */
#define __CRT_GLOB_USE_SINGLE_QUOTE__	0x0010
#define __CRT_GLOB_BRACKET_GROUPS__	0x0020
#define __CRT_GLOB_CASE_SENSITIVE__	0x0040

/* The MinGW globbing algorithm uses the ASCII DEL control code as a marker
 * for globbing characters which were embedded within quoted arguments; (the
 * quotes are stripped away BEFORE the argument is globbed; the globbing code
 * treats the marked character as immutable, and strips out the DEL markers,
 * before storing the resultant argument).  The DEL code is mapped to this
 * function here; DO NOT change it, without rebuilding the runtime.
 */
#define __CRT_GLOB_ESCAPE_CHAR__	(char)(127)


/* Manifest definitions identifying the flag bits, controlling activation
 * of MinGW features, as specified by the user in __MINGW_FEATURES__.
 */
#define __MINGW_ANSI_STDIO__		0x0000000000000001ULL
/*
 * The following three are not yet formally supported; they are
 * included here, to document anticipated future usage.
 */
#define __MINGW_LC_EXTENSIONS__ 	0x0000000000000050ULL
#define __MINGW_LC_MESSAGES__		0x0000000000000010ULL
#define __MINGW_LC_ENVVARS__		0x0000000000000040ULL

/* Try to avoid problems with outdated checks for GCC __attribute__ support.  */
#undef __attribute__

#ifndef __MINGW_IMPORT
 /* Note the extern. This is needed to work around GCC's
  * limitations in handling dllimport attribute.
  */
# define __MINGW_IMPORT  extern __attribute__ ((__dllimport__))
#endif
#ifndef _CRTIMP
# ifdef __USE_CRTIMP
#  define _CRTIMP  __attribute__ ((dllimport))
# else
#  define _CRTIMP
# endif
#endif

#ifndef __int64
# define __int64 long long
#endif
#ifndef __int32
# define __int32 long
#endif
#ifndef __int16
# define __int16 short
#endif
#ifndef __int8
# define __int8 char
#endif
#ifndef __small
# define __small char
#endif
#ifndef __hyper
# define __hyper long long
#endif

#define __MINGW_GNUC_PREREQ(major, minor) \
 (__GNUC__ > (major) || (__GNUC__ == (major) && __GNUC_MINOR__ >= (minor)))

#ifdef __cplusplus
# define __CRT_INLINE inline
#else
# if __GNUC_STDC_INLINE__
#  define __CRT_INLINE extern inline __attribute__((__gnu_inline__))
# else
#  define __CRT_INLINE extern __inline__
# endif
#endif

/* _CRTALIAS will be used when we have a function whose purpose is to return
 * the value of a similar function. This alias function will contain one line
 * of code.
 */
#define _CRTALIAS __CRT_INLINE __attribute__ ((__always_inline__))

/* __CRT_MAYBE_INLINE is to be used when we provide functions in the headers
 * to provide compatibility between differing versions of MSVCRT.DLL for
 * differing OS versions.  See stat.h for examples.
 */
#ifndef __NO_INLINE__
#define __CRT_MAYBE_INLINE __CRT_INLINE
#else /* def __NO_INLINE__ */
#define __CRT_MAYBE_INLINE _CRTALIAS /* We need to inline to stop auto-export */
#endif /* ndef __NO_INLINE__ */

#ifdef __cplusplus
# define   BEGIN_C_DECLS	extern "C" {
# define __UNUSED_PARAM(x)
# define   END_C_DECLS		}
#else
# define   BEGIN_C_DECLS
# define __UNUSED_PARAM(x) 	x __attribute__ ((__unused__))
# define   END_C_DECLS
#endif

#define __MINGW_ATTRIB_NORETURN __attribute__ ((__noreturn__))
#define __MINGW_ATTRIB_CONST __attribute__ ((__const__))
#define __MINGW_ATTRIB_MALLOC __attribute__ ((__malloc__))
#define __MINGW_ATTRIB_PURE __attribute__ ((__pure__))
#define __MINGW_ATTRIB_NONNULL(arg) __attribute__ ((__nonnull__ (arg)))
#define __MINGW_ATTRIB_DEPRECATED __attribute__ ((__deprecated__))
#define __MINGW_NOTHROW __attribute__ ((__nothrow__))


/* Activation of MinGW specific extended features:
 *
 * TODO: Mark (almost) all CRT functions as __MINGW_NOTHROW.  This will
 * allow GCC to optimize away some EH unwind code, at least in DW2 case.
 */
#ifndef __USE_MINGW_ANSI_STDIO
/*
 * If user didn't specify it explicitly...
 */
# if   defined __STRICT_ANSI__  ||  defined _ISOC99_SOURCE \
   ||  defined _POSIX_SOURCE    ||  defined _POSIX_C_SOURCE \
   ||  defined _XOPEN_SOURCE    ||  defined _XOPEN_SOURCE_EXTENDED \
   ||  defined _GNU_SOURCE      ||  defined _BSD_SOURCE \
   ||  defined _SVID_SOURCE
   /*
    * but where any of these source code qualifiers are specified,
    * then assume ANSI I/O standards are preferred over Microsoft's...
    */
#  define __USE_MINGW_ANSI_STDIO    1
# else
   /*
    * otherwise use whatever __MINGW_FEATURES__ specifies...
    */
#  define __USE_MINGW_ANSI_STDIO    (__MINGW_FEATURES__ & __MINGW_ANSI_STDIO__)
# endif
#endif

/* @TODO: Need to convert __MSVCRT_VERSION__ value to MSVCRT_VERSION value.
 * HEX to DECIMAL will not work. */
#if 0
#ifdef __MSVCRT_VERSION__
#ifdef MSVCRT_VERSION
#if MSVCRT_VERSION != __MSVCRT_VERSION__
#error You have specified __MSVCRT_VERSION__ and MSVCRT_VERSION and they are \
not equal.
#endif /* MSVCRT_VERSION != __MSVCRT_VERSION__ */
#else /* ndef MSVCRT_VERSION */
#define MSVCRT_VERSION __MSVCRT_VERSION__
#endif /* MSVCRT_VERSION */
#endif /* __MSVCRT_VERSION__ */
#endif /* 0 */

/*
 * We need to set a default MSVCRT_VERSION which describes the MSVCRT.DLL on
 * the users system.  We are defaulting to XP but we recommend the user define
 * this in his config.h or Makefile file based on the minimum supported version
 * of OS for his program.
 * ME = 600
 * XP = 710
 * VISTA = 800
 * WIN7 = 900
 * WIN8 = 1010
 */
#ifndef MSVCRT_VERSION
#if _WIN32_WINNT >= _WIN32_WINNT_WIN8
#define MSVCRT_VERSION 1010
#elif _WIN32_WINNT >= _WIN32_WINNT_WIN7
#define MSVCRT_VERSION 900
#elif _WIN32_WINNT >= _WIN32_WINNT_VISTA
#define MSVCRT_VERSION 800
#elif _WIN32_WINNT >= _WIN32_WINNT_WINXP
#define MSVCRT_VERSION 710
#elif _WIN32_WINNT >= _WIN32_WINNT_WIN2K
#define MSVCRT_VERSION 700
#elif _WIN32_WINNT >= _WIN32_WINNT_WINME
#define MSVCRT_VERSION 600
#else
#define MSVCRT_VERSION 700
#endif /* _WIN32_WINNT >= _WIN32_WINNT_WINME */
#endif /* ndef MSVCRT_VERSION */

struct threadlocalinfostruct;
struct threadmbinfostruct;
typedef struct threadlocalinfostruct *pthreadlocinfo;
typedef struct threadmbcinfostruct *pthreadmbcinfo;

typedef struct localeinfo_struct {
  pthreadlocinfo locinfo;
  pthreadmbcinfo mbcinfo;
} _locale_tstruct, *_locale_t;

/* The __AW() definition will be used for mapping UNICODE versus ASCII versions
 * to the non represented names.  This is accomplished by macro expansion of
 * the symbol passed and concantenating either A or W to the symbol.
 */
#define __AW__(AW, AW_) AW ## AW_
#if ( \
 (!defined(__TEST_SQL_NOUNICODEMAP) && defined(UNICODE)) || \
 (!defined(__TEST_SQL_NOUNICODEMAP) && defined(_UNICODE)) || \
 defined(FORCE_UNICODE) || \
 (defined(__TEST_SQL_NOUNICODEMAP) && !defined(SQL_NOUNICODEMAP) && \
   (defined(UNICODE) || defined(_UNICODE))) \
 )
# define __AW(AW) __AW__(AW, W)
# define __STR(AW) __AW__(L, AW)
#else
# define __AW(AW) __AW__(AW, A)
# define __STR(AW) __AW__(, AW)
#endif

#endif /* __MINGW_H */
