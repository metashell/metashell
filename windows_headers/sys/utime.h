/**
 * @file utime.h
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
#ifndef	_UTIME_H_
#define	_UTIME_H_
#pragma GCC system_header
#include <_mingw.h>

#define __need_wchar_t
#define __need_size_t

#include <sys/types.h>

#ifndef RC_INVOKED
#include <stddef.h>
/*
 * Structure used by _utime function.
 */
#ifndef _UTIMBUF_DEFINED
struct _utimbuf
{
	time_t	actime;		/* Access time */
	time_t	modtime;	/* Modification time */
};
struct __utimbuf32
{
	__time32_t actime;
	__time32_t modtime;
};
struct __utimbuf64
{
	__time64_t actime;
	__time64_t modtime;
};


#ifndef	_NO_OLDNAMES
/* NOTE: Must be the same as _utimbuf above. */
struct utimbuf
{
	time_t	actime;
	time_t	modtime;
};
struct utimbuf32
{
    __time32_t actime;
    __time32_t modtime;
};
#endif	/* Not _NO_OLDNAMES */
#define _UTIMBUF_DEFINED
#endif /* ndef _UTIMBUF_DEFINED */

#ifdef	__cplusplus
extern "C" {
#endif

_CRTIMP int __cdecl __MINGW_NOTHROW	_utime64 (const char*, struct __utimbuf64*);
_CRTIMP int __cdecl __MINGW_NOTHROW	_futime64 (int, struct __utimbuf64*);

#if MSVCRT_VERSION >= 800
_CRTIMP int __cdecl __MINGW_NOTHROW	_utime32 (const char*, struct __utimbuf32*);
_CRTIMP int __cdecl __MINGW_NOTHROW	_futime32 (int, struct __utimbuf32*);

#else
_CRTIMP int __cdecl __MINGW_NOTHROW	_utime (const char*, struct _utimbuf*);
_CRTALIAS int __cdecl __MINGW_NOTHROW _utime32 (const char* _v1, struct __utimbuf32* _v2) {
    return _utime(_v1, (struct _utimbuf*)_v2);
}

_CRTIMP int __cdecl __MINGW_NOTHROW	_futime (int, struct _utimbuf*);
_CRTALIAS int __cdecl __MINGW_NOTHROW _futime32 (int _v1, struct __utimbuf32* _v2) {
    return _futime(_v1, (struct _utimbuf*)_v2);
}

#endif

#ifndef	_NO_OLDNAMES
#ifdef _USE_32BIT_TIME_T
_CRTIMP int __cdecl __MINGW_NOTHROW	utime (const char*, struct utimbuf*);
#else
int __cdecl __MINGW_NOTHROW utime (const char*, struct utimbuf*);
_CRTALIAS int __cdecl __MINGW_NOTHROW	utime (const char* _v1, struct utimbuf* _v2) {
  return _utime64(_v1, (struct __utimbuf64*)_v2);
}
#endif
#endif	/* Not _NO_OLDNAMES */

#ifndef _USE_32BIT_TIME_T
_CRTALIAS int __cdecl __MINGW_NOTHROW	_utime (const char* _v1, struct _utimbuf* _v2) {
    return(_utime64  (_v1,(struct __utimbuf64*)_v2));
}
_CRTALIAS int __cdecl __MINGW_NOTHROW	_futime (int _v1, struct _utimbuf* _v2) {
    return(_futime64 (_v1,(struct __utimbuf64*)_v2));
}

#else /* def _USE_32BIT_TIME_T */
#if MSVCRT_VERSION >= 800
_CRTALIAS int __cdecl __MINGW_NOTHROW	_utime (const char* _v1, struct _utimbuf* _v2) {
    return(_utime32  (_v1,(struct __utimbuf32*)_v2));
}
_CRTALIAS int __cdecl __MINGW_NOTHROW	_futime (int _v1, struct _utimbuf* _v2)	{
    return(_futime32 (_v1,(struct __utimbuf32*)_v2));
}
#endif /* MSVCRT_VERSION >= 800 */

#endif /* ndef _USE_32BIT_TIME_T */

#ifndef _WUTIME_DEFINED
_CRTIMP int __cdecl __MINGW_NOTHROW	_wutime64 (const wchar_t*, struct __utimbuf64*);
#if MSVCRT_VERSION >= 800
_CRTIMP int __cdecl __MINGW_NOTHROW	_wutime32 (const wchar_t*, struct __utimbuf32*);

#else /* MSVCRT_VERSION < 800 */
_CRTIMP int __cdecl __MINGW_NOTHROW	_wutime (const wchar_t*, struct _utimbuf*);
_CRTALIAS int __cdecl __MINGW_NOTHROW _wutime32 (const wchar_t* _v1, struct __utimbuf32* _v2) {
    return _wutime(_v1, (struct _utimbuf*)_v2);
}

#endif /* MSVCRT_VERSION >= 800 */
#ifdef _USE_32BIT_TIME_T
#if MSVCRT_VERSION >= 800
_CRTALIAS int __cdecl __MINGW_NOTHROW	_wutime (const wchar_t* _v1, struct _utimbuf* _v2) {
    return(_wutime32 (_v1,(struct __utimbuf32*)_v2));
}
#endif /* MSVCRT_VERSION >= 800 */

#else /* ndef _USE_32BIT_TIME_T */
_CRTALIAS int __cdecl __MINGW_NOTHROW	_wutime (const wchar_t* _v1, struct _utimbuf* _v2) {
    return(_wutime64 (_v1,(struct __utimbuf64*)_v2));
}

#endif /* def _USE_32BIT_TIME_T */
#define _WUTIME_DEFINED
#endif /* ndef _WUTIME_DEFINED */

#ifdef	__cplusplus
}
#endif

#endif	/* Not RC_INVOKED */

#endif	/* Not _UTIME_H_ */