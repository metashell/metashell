/**
 * @file timeb.h
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
#ifndef	_TIMEB_H_
#define	_TIMEB_H_
#pragma GCC system_header
#include <_mingw.h>

/*
 * Support for the UNIX System V ftime system call.
 */

#include <sys/types.h>

#ifndef	RC_INVOKED

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * TODO: Structure not tested.
 */
struct __timeb32
{
	__time32_t	time;
	short	millitm;
	short	timezone;
	short	dstflag;
};

/* This requires newer versions of msvcrt.dll (6.10 or higher).  */
struct __timeb64
{
  __time64_t time;
  short millitm;
  short timezone;
  short dstflag;
};

#ifdef _USE_32BIT_TIME_T
#define _timeb __timeb32
#else
#define _timeb __timeb64
#endif

_CRTIMP void __cdecl __MINGW_NOTHROW	_ftime (struct _timeb*);
_CRTIMP void __cdecl __MINGW_NOTHROW	_ftime64 (struct __timeb64*);

#if MSVCRT_VERSION >= 800
_CRTIMP void __cdecl __MINGW_NOTHROW	_ftime32 (struct __timeb32*);
#else
_CRTALIAS void __cdecl __MINGW_NOTHROW  _ftime32 (struct __timeb32* _v1) {
    return _ftime((struct _timeb *)_v1);
}
#endif

#if defined(_USE_32BIT_TIME_T)
#define _ftime _ftime32

#else
#define _ftime _ftime64
#endif

#ifndef	_NO_OLDNAMES
#define ftime _ftime
#define timeb _timeb
#endif	/* Not _NO_OLDNAMES */

#ifdef	__cplusplus
}
#endif

#endif	/* Not RC_INVOKED */

#endif	/* Not _TIMEB_H_ */
