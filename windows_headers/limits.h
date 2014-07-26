/**
 * @file limits.h
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
#ifndef _LIMITS_H
#define _LIMITS_H
#pragma GCC system_header
#include <_mingw.h>

/* 
 * Functions for manipulating paths and directories (included from io.h)
 * plus functions for setting the current drive.
 *
 * Defines constants for the sizes of integral types.
 *
 * NOTE: GCC should supply a version of this header and it should be safe to
 *       use that version instead of this one (maybe safer).
 */

/*
 * File system limits
 *
 * TODO: NAME_MAX and OPEN_MAX are file system limits or not? Are they the
 *       same as FILENAME_MAX and FOPEN_MAX from stdio.h?
 * NOTE: PATH_MAX is the POSIX equivalent for Microsoft's MAX_PATH; the two
 *       are semantically identical, with a limit of 259 characters for the
 *       path name, plus one for a terminating NUL, for a total of 260.
 */
# define PATH_MAX	260

/*
 * Characteristics of the char data type.
 *
 * TODO: Is MB_LEN_MAX correct?
 */
#define CHAR_BIT	8
#define MB_LEN_MAX	2

#define SCHAR_MIN	(-128)
#define SCHAR_MAX	127

#define UCHAR_MAX	255

#ifndef _CHAR_UNSIGNED
#define CHAR_MIN	SCHAR_MIN
#define CHAR_MAX	SCHAR_MAX
#else
#define CHAR_MIN	0
#define CHAR_MAX	UCHAR_MAX
#endif

/*
 * Maximum and minimum values for ints.
 */
#define INT_MAX		2147483647
#define INT_MIN		(-INT_MAX-1)

#define UINT_MAX	0xffffffff

/*
 * Maximum and minimum values for shorts.
 */
#define SHRT_MAX	32767
#define SHRT_MIN	(-SHRT_MAX-1)

#define USHRT_MAX	0xffff

/*
 * Maximum and minimum values for longs and unsigned longs.
 */
#define LONG_MAX	2147483647L
#define LONG_MIN	(-LONG_MAX-1)
#define ULONG_MAX	0xffffffffUL

#define SSIZE_MAX LONG_MAX

#define LLONG_MAX 9223372036854775807LL
#define LLONG_MIN (-LLONG_MAX - 1)
#define ULLONG_MAX (2ULL * LLONG_MAX + 1)

#define LONG_LONG_MAX	9223372036854775807LL
#define LONG_LONG_MIN	(-LONG_LONG_MAX-1)
#define ULONG_LONG_MAX	(2ULL * LONG_LONG_MAX + 1)

/* MSVC compatibility */
#define _I64_MIN LONG_LONG_MIN
#define _I64_MAX LONG_LONG_MAX
#define _UI64_MAX ULONG_LONG_MAX

#endif /* not _LIMITS_H */
