/**
 * @file time.h
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
#ifndef	_TIME_H
#define	_TIME_H
#pragma GCC system_header
#include <_mingw.h>

#define __need_wchar_t
#define __need_size_t
#define __need_NULL

#ifndef RC_INVOKED
#include <stddef.h>
#endif	/* Not RC_INVOKED */

/*
 * Number of clock ticks per second. A clock tick is the unit by which
 * processor time is measured and is returned by 'clock'.
 */
#define	CLOCKS_PER_SEC	((clock_t)1000)
#define	CLK_TCK		CLOCKS_PER_SEC


#ifndef RC_INVOKED

/*
 * A type for storing the current time and date. This is the number of
 * seconds since midnight Jan 1, 1970.
 * NOTE: This is also defined in non-ISO sys/types.h.
 */
#ifndef _TIME32_T_DEFINED
typedef __int32 __time32_t;
#define _TIME32_T_DEFINED
#endif

#ifndef _TIME64_T_DEFINED
/* A 64-bit time_t to get to Y3K */
typedef __int64 __time64_t;
#define _TIME64_T_DEFINED
#endif

#ifndef _TIME_T_DEFINED
# if defined(_USE_32BIT_TIME_T)
   typedef	__time32_t time_t;
# else
   typedef	__time64_t time_t;
# endif /* _USE_32BIT_TIME_T */
# define _TIME_T_DEFINED
#endif

/*
 * A type for measuring processor time (in clock ticks).
 */
#ifndef _CLOCK_T_DEFINED
typedef	long	clock_t;
#define _CLOCK_T_DEFINED
#endif

#ifndef _TM_DEFINED
/*
 * A structure for storing all kinds of useful information about the
 * current (or another) time.
 */
struct tm
{
	int	tm_sec;		/* Seconds: 0-59 (K&R says 0-61?) */
	int	tm_min;		/* Minutes: 0-59 */
	int	tm_hour;	/* Hours since midnight: 0-23 */
	int	tm_mday;	/* Day of the month: 1-31 */
	int	tm_mon;		/* Months *since* january: 0-11 */
	int	tm_year;	/* Years since 1900 */
	int	tm_wday;	/* Days since Sunday (0-6) */
	int	tm_yday;	/* Days since Jan. 1: 0-365 */
	int	tm_isdst;	/* +1 Daylight Savings Time, 0 No DST,
				 * -1 don't know */
};
#define _TM_DEFINED
#endif

#ifdef	__cplusplus
extern "C" {
#endif

_CRTIMP clock_t __cdecl __MINGW_NOTHROW	clock (void);

/*
 * These functions write to and return pointers to static buffers that may
 * be overwritten by other function calls. Yikes!
 *
 * NOTE: localtime, and perhaps the others of the four functions grouped
 * below may return NULL if their argument is not 'acceptable'. Also note
 * that calling asctime with a NULL pointer will produce an Invalid Page
 * Fault and crap out your program. Guess how I know. Hint: stat called on
 * a directory gives 'invalid' times in st_atime etc...
 */
_CRTIMP char*       __cdecl __MINGW_NOTHROW asctime (const struct tm*);
_CRTIMP size_t	   __cdecl __MINGW_NOTHROW strftime (char*, size_t, const char*, const struct tm*);
#if MSVCRT_VERSION < 800
_CRTIMP char*	    __cdecl __MINGW_NOTHROW ctime (const time_t*);
_CRTIMP struct tm*  __cdecl __MINGW_NOTHROW gmtime (const time_t*);
_CRTIMP struct tm*  __cdecl __MINGW_NOTHROW localtime (const time_t*);

_CRTIMP time_t     __cdecl __MINGW_NOTHROW time       (time_t*);
_CRTIMP double     __cdecl __MINGW_NOTHROW difftime   (time_t, time_t);
_CRTIMP time_t     __cdecl __MINGW_NOTHROW mktime     (struct tm*);
#endif

#ifndef __STRICT_ANSI__

extern _CRTIMP void __cdecl __MINGW_NOTHROW _tzset (void);

#ifndef _NO_OLDNAMES
extern _CRTIMP void __cdecl __MINGW_NOTHROW tzset (void);
#endif

_CRTIMP char* __cdecl __MINGW_NOTHROW	_strdate(char*);
_CRTIMP char* __cdecl __MINGW_NOTHROW	_strtime(char*);

/* These require newer versions of msvcrt.dll (6.10 or higher). */
_CRTIMP __time64_t __cdecl __MINGW_NOTHROW  _time64( __time64_t*);
_CRTIMP __time64_t __cdecl __MINGW_NOTHROW  _mktime64 (struct tm*);
_CRTIMP char* __cdecl __MINGW_NOTHROW _ctime64 (const __time64_t*);
_CRTIMP struct tm*  __cdecl __MINGW_NOTHROW _gmtime64 (const __time64_t*);
_CRTIMP struct tm*  __cdecl __MINGW_NOTHROW _localtime64 (const __time64_t*);

/* These require newer versions of msvcrt.dll (8.00 or higher). */
#if MSVCRT_VERSION >= 800
_CRTIMP __time32_t __cdecl __MINGW_NOTHROW	_time32     (__time32_t*);
_CRTIMP double	   __cdecl __MINGW_NOTHROW	_difftime32 (__time32_t, __time32_t);
_CRTIMP double	   __cdecl __MINGW_NOTHROW	_difftime64 (__time64_t, __time64_t);
_CRTIMP __time32_t __cdecl __MINGW_NOTHROW	_mktime32   (struct tm*);
_CRTIMP __time32_t __cdecl __MINGW_NOTHROW	_mkgmtime32 (struct tm*);
_CRTIMP __time64_t __cdecl __MINGW_NOTHROW	_mkgmtime64 (struct tm*);
_CRTIMP char*	   __cdecl __MINGW_NOTHROW	_ctime32    (const __time32_t*);
_CRTIMP struct tm* __cdecl __MINGW_NOTHROW	_gmtime32   (const __time32_t*);
_CRTIMP struct tm* __cdecl __MINGW_NOTHROW	_localtime32(const __time32_t*);
#else /* MSVCRT_VERSION < 800 */
_CRTALIAS __time32_t __cdecl __MINGW_NOTHROW _time32  (__time32_t *_v) {
    return ((__time32_t)time((time_t*)_v));
}
_CRTALIAS double __cdecl __MINGW_NOTHROW _difftime32 (__time32_t _v1, __time32_t _v2) {
    return (difftime((time_t)_v1, (time_t)_v2));
}
_CRTALIAS __time32_t __cdecl __MINGW_NOTHROW _mktime32(struct tm* _v1) {
    return ((__time32_t)mktime(_v1));
}
_CRTALIAS char* __cdecl __MINGW_NOTHROW _ctime32(const __time32_t* _v1) {
    return (ctime((time_t*)_v1));
}
_CRTALIAS struct tm* __cdecl __MINGW_NOTHROW _gmtime32(const __time32_t* _v1) {
    return (gmtime((time_t*)_v1));
}
_CRTALIAS struct tm* __cdecl __MINGW_NOTHROW _localtime32(const __time32_t* _v1) {
    return (localtime((time_t*)_v1));
}
#endif /* MSVCRT_VERSION >= 800 */

#if defined(_USE_32BIT_TIME_T)
/* Prevent circular recursion. */
#if MSVCRT_VERSION >= 800
_CRTALIAS time_t     __cdecl __MINGW_NOTHROW	time (time_t* _v)
    { return(_time32 (_v)); }
_CRTALIAS double     __cdecl __MINGW_NOTHROW	difftime(time_t _v1, time_t _v2)
    { return(_difftime32 (_v1,_v2)); }
_CRTALIAS time_t     __cdecl __MINGW_NOTHROW	mktime (struct tm* _v)
    { return(_mktime32 (_v)); }
_CRTALIAS time_t     __cdecl __MINGW_NOTHROW	_mkgmtime (struct tm* _v)
    { return(_mkgmtime32 (_v)); }
_CRTALIAS char*	     __cdecl __MINGW_NOTHROW	ctime (const time_t* _v)
    { return(_ctime32 (_v)); }
_CRTALIAS struct tm* __cdecl __MINGW_NOTHROW	gmtime (const time_t* _v)
    { return(_gmtime32 (_v)); }
_CRTALIAS struct tm* __cdecl __MINGW_NOTHROW	localtime (const time_t* _v)
    { return(_localtime32 (_v)); }
#endif /* MSVCRT_VERSION >= 800 */

#else /* !defined(_USE_32BIT_TIME_T) */
/* _difftime64 and _mkgmtime64 are missing from MSVCRT.DLL pre Vista. */
#if MSVCRT_VERSION >= 800
_CRTALIAS double	   __cdecl __MINGW_NOTHROW	difftime(time_t _v1, time_t _v2)
    { return(_difftime64 (_v1,_v2)); }
_CRTALIAS time_t	   __cdecl __MINGW_NOTHROW	_mkgmtime (struct tm* _v)
    { return(_mkgmtime64 (_v)); }
#endif /* MSVCRT_VERSION >= 800 */
_CRTALIAS time_t	   __cdecl __MINGW_NOTHROW	time (time_t* _v)
    { return(_time64 (_v)); }
_CRTALIAS time_t	   __cdecl __MINGW_NOTHROW	mktime (struct tm* _v)
    { return(_mktime64 (_v)); }
_CRTALIAS char*		   __cdecl __MINGW_NOTHROW	ctime (const time_t* _v)
    { return(_ctime64 (_v)); }
_CRTALIAS struct tm*   __cdecl __MINGW_NOTHROW	gmtime (const time_t* _v)
    { return(_gmtime64 (_v)); }
_CRTALIAS struct tm*   __cdecl __MINGW_NOTHROW	localtime (const time_t* _v)
    { return(_localtime64 (_v)); }
#endif /* _USE_32BIT_TIME_T */

/*
 * _daylight: non zero if daylight savings time is used.
 * _timezone: difference in seconds between GMT and local time.
 * _tzname: standard/daylight savings time zone names (an array with two
 *          elements).
 */

/* These are for compatibility with pre-VC 5.0 suppied MSVCRT. */
extern _CRTIMP int* __cdecl __MINGW_NOTHROW	__p__daylight (void);
extern _CRTIMP long* __cdecl __MINGW_NOTHROW	__p__timezone (void);
extern _CRTIMP char** __cdecl __MINGW_NOTHROW	__p__tzname (void);

__MINGW_IMPORT int	_daylight;
__MINGW_IMPORT long	_timezone;
__MINGW_IMPORT char 	*_tzname[2];

#endif	/* Not __STRICT_ANSI__ */

#ifndef _NO_OLDNAMES
/* These go in the oldnames import library for MSVCRT. */
__MINGW_IMPORT int	daylight;
__MINGW_IMPORT long	timezone;
__MINGW_IMPORT char 	*tzname[2];
#endif	/* Not _NO_OLDNAMES */

#ifndef _WTIME_DEFINED
/* wide function prototypes, also declared in wchar.h */

#ifndef __STRICT_ANSI__
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW	_wasctime(const struct tm*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW	_wstrdate(wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW	_wstrtime(wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW	_wctime64(const __time64_t*);
#if MSVCRT_VERSION >= 800
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW	_wctime32(const __time32_t*);
#else
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW    _wctime(const time_t*);
_CRTALIAS wchar_t* __cdecl __MINGW_NOTHROW  _wctime32(const __time32_t* _v) {
    return(_wctime((time_t*)_v));
}
#endif /* MSVCRT_VERSION >= 800 */

#ifdef _USE_32BIT_TIME_T
#if MSVCRT_VERSION >= 800
_CRTALIAS wchar_t* __cdecl __MINGW_NOTHROW	_wctime (const time_t* _v) {
    return(_wctime32 (_v));
}
#endif
#else /* ndef _USE_32BIT_TIME_T */
_CRTALIAS wchar_t* __cdecl __MINGW_NOTHROW	_wctime (const time_t* _v) {
    return(_wctime64 (_v));
}
#endif /* def _USE_32BIT_TIME_T */

#endif /* __STRICT_ANSI__ */

_CRTIMP size_t __cdecl __MINGW_NOTHROW		wcsftime (wchar_t*, size_t, const wchar_t*, const struct tm*);
#define _WTIME_DEFINED
#endif /* _WTIME_DEFINED */

#ifdef	__cplusplus
}
#endif

#endif	/* Not RC_INVOKED */

#endif	/* Not _TIME_H */
