/**
 * @file wchar.h
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
#ifndef	_WCHAR_H_
#define	_WCHAR_H_
#ifndef __CRT_TESTING__
#pragma GCC system_header
#endif
#include <_mingw.h>

#ifndef RC_INVOKED

#define __need_size_t
#define __need_wint_t
#define __need_wchar_t
#define __need_NULL
#include <stddef.h>

#ifndef __VALIST
#define __need___va_list
#include <stdarg.h>
#define __VALIST __builtin_va_list
#endif /* ndef __VALIST */

#endif /* Not RC_INVOKED */

/*
 * MSDN says that isw* char classifications are in wchar.h and wctype.h.
 * Although the wctype names are ANSI, their exposure in this header is
 * not.
 */
#include <wctype.h>
#include <sys/types.h>

#define WCHAR_MIN	0
#define WCHAR_MAX	0xffff

#ifndef WEOF
#define	WEOF	(wchar_t)(0xFFFF)
#endif

#ifndef FILENAME_MAX
#define FILENAME_MAX 260
#endif

#ifndef RC_INVOKED

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _FILE_DEFINED  /* Also in stdio.h */
#define	_FILE_DEFINED
typedef struct _iobuf
{
	char*	_ptr;
	int	_cnt;
	char*	_base;
	int	_flag;
	int	_file;
	int	_charbuf;
	int	_bufsiz;
	char*	_tmpfname;
} FILE;
#endif	/* Not _FILE_DEFINED */

#ifndef _TM_DEFINED /* Also in time.h */
struct tm {
        int tm_sec;     /* seconds after the minute - [0,59] */
        int tm_min;     /* minutes after the hour - [0,59] */
        int tm_hour;    /* hours since midnight - [0,23] */
        int tm_mday;    /* day of the month - [1,31] */
        int tm_mon;     /* months since January - [0,11] */
        int tm_year;    /* years since 1900 */
        int tm_wday;    /* days since Sunday - [0,6] */
        int tm_yday;    /* days since January 1 - [0,365] */
        int tm_isdst;   /* daylight savings time flag */
        };
#define _TM_DEFINED
#endif

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

#ifndef _WSTDIO_DEFINED
/*  Also in stdio.h - keep in sync */
_CRTIMP int __cdecl __MINGW_NOTHROW	fwprintf (FILE*, const wchar_t*, ...);
_CRTIMP int __cdecl __MINGW_NOTHROW	wprintf (const wchar_t*, ...);
_CRTIMP int __cdecl __MINGW_NOTHROW	_snwprintf (wchar_t*, size_t, const wchar_t*, ...);
_CRTIMP int __cdecl __MINGW_NOTHROW	vfwprintf (FILE*, const wchar_t*, __VALIST);
_CRTIMP int __cdecl __MINGW_NOTHROW	vwprintf (const wchar_t*, __VALIST);
_CRTIMP int __cdecl __MINGW_NOTHROW	_vsnwprintf (wchar_t*, size_t, const wchar_t*, __VALIST);
_CRTIMP int __cdecl __MINGW_NOTHROW	_vscwprintf (const wchar_t*, __VALIST);
_CRTIMP int __cdecl __MINGW_NOTHROW	fwscanf (FILE*, const wchar_t*, ...);
_CRTIMP int __cdecl __MINGW_NOTHROW	wscanf (const wchar_t*, ...);
_CRTIMP int __cdecl __MINGW_NOTHROW	swscanf (const wchar_t*, const wchar_t*, ...);
_CRTIMP wint_t __cdecl __MINGW_NOTHROW	fgetwc (FILE*);
_CRTIMP wint_t __cdecl __MINGW_NOTHROW	fputwc (wchar_t, FILE*);
_CRTIMP wint_t __cdecl __MINGW_NOTHROW	ungetwc (wchar_t, FILE*);

/* These differ from the ISO C prototypes, which have a maxlen parameter like snprintf.  */
#ifndef __STRICT_ANSI__
_CRTIMP int __cdecl __MINGW_NOTHROW	swprintf (wchar_t*, const wchar_t*, ...);
_CRTIMP int __cdecl __MINGW_NOTHROW	vswprintf (wchar_t*, const wchar_t*, __VALIST);
#endif /* ndef __STRICT_ANSI__ */

_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW fgetws (wchar_t*, int, FILE*);
_CRTIMP int __cdecl __MINGW_NOTHROW	fputws (const wchar_t*, FILE*);
_CRTIMP wint_t __cdecl __MINGW_NOTHROW	getwc (FILE*);
_CRTIMP wint_t __cdecl __MINGW_NOTHROW	getwchar (void);
_CRTIMP wint_t __cdecl __MINGW_NOTHROW	putwc (wint_t, FILE*);
_CRTIMP wint_t __cdecl __MINGW_NOTHROW	putwchar (wint_t);

#ifndef __STRICT_ANSI__
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _getws (wchar_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW	_putws (const wchar_t*);
_CRTIMP FILE* __cdecl __MINGW_NOTHROW	_wfdopen(int, const wchar_t *);
_CRTIMP FILE* __cdecl __MINGW_NOTHROW	_wfopen (const wchar_t*, const wchar_t*);
_CRTIMP FILE* __cdecl __MINGW_NOTHROW	_wfreopen (const wchar_t*, const wchar_t*, FILE*);
_CRTIMP FILE* __cdecl __MINGW_NOTHROW	_wfsopen (const wchar_t*, const wchar_t*, int);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wtmpnam (wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wtempnam (const wchar_t*, const wchar_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW	_wrename (const wchar_t*, const wchar_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW	_wremove (const wchar_t*);
_CRTIMP void __cdecl __MINGW_NOTHROW	_wperror (const wchar_t*);
_CRTIMP FILE* __cdecl __MINGW_NOTHROW	_wpopen (const wchar_t*, const wchar_t*);
#endif  /* __STRICT_ANSI__ */

#ifndef __NO_ISOCEXT  /* externs in libmingwex.a */
int __cdecl __MINGW_NOTHROW snwprintf (wchar_t*, size_t, const wchar_t*, ...);
int __cdecl __MINGW_NOTHROW vsnwprintf (wchar_t*, size_t, const wchar_t*, __VALIST);

#ifndef __NO_INLINE__
__CRT_INLINE int __cdecl __MINGW_NOTHROW
vsnwprintf (wchar_t* s, size_t n, const wchar_t* format, __VALIST arg)
  { return _vsnwprintf ( s, n, format, arg);}
#endif

int __cdecl __MINGW_NOTHROW vwscanf (const wchar_t * __restrict__, __VALIST);
int __cdecl __MINGW_NOTHROW vfwscanf (FILE * __restrict__,
		       const wchar_t * __restrict__, __VALIST);
int __cdecl __MINGW_NOTHROW vswscanf (const wchar_t * __restrict__,
		       const wchar_t * __restrict__, __VALIST);
#endif /* ndef __NO_ISOCEXT__ */

#define _WSTDIO_DEFINED
#endif /* _WSTDIO_DEFINED */

#ifndef _WSTDLIB_DEFINED /* also declared in stdlib.h */
_CRTIMP long __cdecl __MINGW_NOTHROW 	wcstol (const wchar_t*, wchar_t**, int);
_CRTIMP unsigned long __cdecl __MINGW_NOTHROW wcstoul (const wchar_t*, wchar_t**, int);
_CRTIMP double __cdecl __MINGW_NOTHROW	wcstod (const wchar_t*, wchar_t**);

#if !defined __NO_ISOCEXT /* in libmingwex.a */
float __cdecl __MINGW_NOTHROW wcstof (const wchar_t * __restrict__, wchar_t ** __restrict__);
long double __cdecl __MINGW_NOTHROW wcstold (const wchar_t * __restrict__, wchar_t ** __restrict__);
#endif /* __NO_ISOCEXT */

_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wgetenv(const wchar_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW	_wputenv(const wchar_t*);
_CRTIMP void __cdecl __MINGW_NOTHROW	_wsearchenv(const wchar_t*, const wchar_t*, wchar_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW	_wsystem(const wchar_t*);
_CRTIMP void __cdecl __MINGW_NOTHROW	_wmakepath(wchar_t*, const wchar_t*, const wchar_t*, const wchar_t*, const wchar_t*);
_CRTIMP void __cdecl __MINGW_NOTHROW	_wsplitpath (const wchar_t*, wchar_t*, wchar_t*, wchar_t*, wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wfullpath (wchar_t*, const wchar_t*, size_t);
_CRTIMP __int64 __cdecl _wcstoi64(const wchar_t *, wchar_t **, int);
_CRTIMP __int64 __cdecl _wcstoi64_l(const wchar_t *, wchar_t **, int, _locale_t);
_CRTIMP unsigned __int64 __cdecl _wcstoui64(const wchar_t *, wchar_t **, int);
_CRTIMP unsigned __int64 __cdecl _wcstoui64_l(const wchar_t *, wchar_t **, int, _locale_t);
#define  _WSTDLIB_DEFINED
#endif /* _WSTDLIB_DEFINED */

#ifndef _WTIME_DEFINED
#ifndef __STRICT_ANSI__
/* wide function prototypes, also declared in time.h */
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW	_wasctime (const struct tm*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW	_wstrdate (wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW	_wstrtime (wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW	_wctime64 (const __time64_t*);

/* MSVCRT.DLL may not have _wctime32 depending on OS version */
#if MSVCRT_VERSION >= 800
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW	_wctime32 (const __time32_t*);

#else /* MSVCRT_VERSION < 800 */
_CRTALIAS wchar_t* __cdecl __MINGW_NOTHROW  _wctime32 (const __time32_t* _v1) {
    _CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wctime (const __time32_t*);
    return _wctime(_v1);
}
#endif /* MSVCRT_VERSION >= 800 */

#if MSVCRT_VERSION >= 800
#ifdef _USE_32BIT_TIME_T
_CRTALIAS wchar_t* __cdecl __MINGW_NOTHROW	_wctime (const time_t* _v)	{ return(_wctime32 (_v)); }

#else /* ! _USE_32BIT_TIME_T */
_CRTALIAS wchar_t* __cdecl __MINGW_NOTHROW	_wctime (const time_t* _v)	{ return(_wctime64 (_v)); }
#endif /* _USE_32BIT_TIME_T */

#else /* MSVCRT_VERSION < 800 */
#ifdef _USE_32BIT_TIME_T
#define _wctime _wctime32

#else /* ndef _USE_32BIT_TIME_T */
#define _wctime _wctime64

#endif /* def _USE_32BIT_TIME_T */
#endif /* MSVCRT_VERSION >= 800 */
#endif /* __STRICT_ANSI__ */

_CRTIMP size_t __cdecl __MINGW_NOTHROW	wcsftime (wchar_t*, size_t, const wchar_t*, const struct tm*);

#ifndef _WUTIME_DEFINED
_CRTIMP int __cdecl __MINGW_NOTHROW	_wutime64 (const wchar_t*, struct __utimbuf64*);
#if MSVCRT_VERSION >= 800
_CRTIMP int __cdecl __MINGW_NOTHROW	_wutime32 (const wchar_t*, struct __utimbuf32*);

#else /* MSVCRT_VERSION < 800 */
_CRTIMP int __cdecl __MINGW_NOTHROW	_wutime (const wchar_t*, struct _utimbuf*);
_CRTALIAS int __cdecl __MINGW_NOTHROW _wutime32 (const wchar_t* _v1, struct __utimbuf32* _v2) {
    return _wutime(_v1, (struct _utimbuf *)_v2);
}

#endif /* MSVCRT_VERSION >= 800 */
#ifdef _USE_32BIT_TIME_T
#if MSVCRT_VERSION >= 800
_CRTALIAS int __cdecl __MINGW_NOTHROW _wutime (const wchar_t* _v1, struct _utimbuf* _v2) {
    return(_wutime32 (_v1,(struct __utimbuf32*)_v2));
}
#endif /* MSVCRT_VERSION >= 800 */

#else /* ndef _USE_32BIT_TIME_T */
_CRTALIAS int __cdecl __MINGW_NOTHROW _wutime (const wchar_t* _v1, struct _utimbuf* _v2) {
    return(_wutime64 (_v1,(struct __utimbuf64*)_v2));
}

#endif /* def _USE_32BIT_TIME_T */
#define _WUTIME_DEFINED
#endif /* ndef _WUTIME_DEFINED */

#define _WTIME_DEFINED
#endif /* _WTIME_DEFINED */


#ifndef _WSTRING_DEFINED
/*
 * Unicode versions of the standard string calls.
 * Also in string.h.
 */
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW wcscat (wchar_t*, const wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW wcschr (const wchar_t*, wchar_t);
_CRTIMP int __cdecl __MINGW_NOTHROW	wcscmp (const wchar_t*, const wchar_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW	wcscoll (const wchar_t*, const wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW wcscpy (wchar_t*, const wchar_t*);
_CRTIMP size_t __cdecl __MINGW_NOTHROW	wcscspn (const wchar_t*, const wchar_t*);
/* Note:  _wcserror requires __MSVCRT_VERSION__ >= 0x0700.  */
_CRTIMP size_t __cdecl __MINGW_NOTHROW	wcslen (const wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW wcsncat (wchar_t*, const wchar_t*, size_t);
_CRTIMP int __cdecl __MINGW_NOTHROW	wcsncmp(const wchar_t*, const wchar_t*, size_t);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW wcsncpy(wchar_t*, const wchar_t*, size_t);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW wcspbrk(const wchar_t*, const wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW wcsrchr(const wchar_t*, wchar_t);
_CRTIMP size_t __cdecl __MINGW_NOTHROW	wcsspn(const wchar_t*, const wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW wcsstr(const wchar_t*, const wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW wcstok(wchar_t*, const wchar_t*);
_CRTIMP size_t __cdecl __MINGW_NOTHROW	wcsxfrm(wchar_t*, const wchar_t*, size_t);

#ifndef	__STRICT_ANSI__
/* NOTE: _wcscmpi not provided by runtime dll, this define is for portability */
#define		_wcscmpi	_wcsicmp

_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wcsdup (const wchar_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW	_wcsicmp (const wchar_t*, const wchar_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW	_wcsicoll (const wchar_t*, const wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wcslwr (wchar_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW	_wcsnicmp (const wchar_t*, const wchar_t*, size_t);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wcsnset (wchar_t*, wchar_t, size_t);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wcsrev (wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wcsset (wchar_t*, wchar_t);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wcsupr (wchar_t*);

_CRTIMP int __cdecl __MINGW_NOTHROW  _wcsncoll(const wchar_t*, const wchar_t*, size_t);
_CRTIMP int   __cdecl __MINGW_NOTHROW _wcsnicoll(const wchar_t*, const wchar_t*, size_t);
_CRTIMP  wchar_t* __cdecl __MINGW_NOTHROW _wcserror(int);
_CRTIMP  wchar_t* __cdecl __MINGW_NOTHROW __wcserror(const wchar_t*);

#ifndef	_NO_OLDNAMES
/* NOTE: There is no _wcscmpi, but this is for compatibility. */
int __cdecl __MINGW_NOTHROW wcscmpi (const wchar_t *, const wchar_t *);
_CRTALIAS int __cdecl __MINGW_NOTHROW
wcscmpi (const wchar_t * __ws1, const wchar_t * __ws2)
  {return _wcsicmp (__ws1, __ws2);}
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW wcsdup (const wchar_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW	wcsicmp (const wchar_t*, const wchar_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW	wcsicoll (const wchar_t*, const wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW wcslwr (wchar_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW	wcsnicmp (const wchar_t*, const wchar_t*, size_t);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW wcsnset (wchar_t*, wchar_t, size_t);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW wcsrev (wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW wcsset (wchar_t*, wchar_t);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW wcsupr (wchar_t*);
#endif	/* Not _NO_OLDNAMES */

#endif	/* Not strict ANSI */

#define _WSTRING_DEFINED
#endif  /* _WSTRING_DEFINED */

/* These are resolved by -lmingwex. Alternatively, they can be resolved by
   adding -lmsvcp60 to your command line, which will give you the VC++
   versions of these functions. If you want the latter and don't have
   msvcp60.dll in your windows system directory, you can easily obtain
   it with a search from your favorite search engine.  */

#ifndef __STRICT_ANSI__
typedef wchar_t _Wint_t;
#endif

typedef int mbstate_t;

wint_t __cdecl __MINGW_NOTHROW btowc(int);
size_t __cdecl __MINGW_NOTHROW mbrlen(const char * __restrict__, size_t,
		      mbstate_t * __restrict__);
size_t __cdecl __MINGW_NOTHROW mbrtowc(wchar_t * __restrict__, const char * __restrict__,
		       size_t, mbstate_t * __restrict__);
size_t __cdecl __MINGW_NOTHROW mbsrtowcs(wchar_t * __restrict__, const char ** __restrict__,
			 size_t, mbstate_t * __restrict__);
size_t __cdecl __MINGW_NOTHROW wcrtomb(char * __restrict__, wchar_t,
		       mbstate_t * __restrict__);
size_t __cdecl __MINGW_NOTHROW wcsrtombs(char * __restrict__, const wchar_t ** __restrict__,
			 size_t, mbstate_t * __restrict__);
int __cdecl __MINGW_NOTHROW wctob(wint_t);

#ifndef __NO_ISOCEXT /* these need static lib libmingwex.a */
int __cdecl __MINGW_NOTHROW fwide(FILE*, int);
int __cdecl __MINGW_NOTHROW mbsinit(const mbstate_t*);

#ifndef __NO_INLINE__
__CRT_INLINE int __cdecl __MINGW_NOTHROW fwide(FILE* __UNUSED_PARAM(stream),
					       int mode)
  {return mode;} /* Nothing to do  */
__CRT_INLINE int __cdecl __MINGW_NOTHROW mbsinit(const mbstate_t* __UNUSED_PARAM(ps))
  {return 1;}
#endif

wchar_t* __cdecl __MINGW_NOTHROW wmemset(wchar_t *, wchar_t, size_t);
wchar_t* __cdecl __MINGW_NOTHROW wmemchr(const wchar_t*, wchar_t, size_t);
int wmemcmp(const wchar_t*, const wchar_t *, size_t);
wchar_t* __cdecl __MINGW_NOTHROW wmemcpy(wchar_t* __restrict__,
		         const wchar_t* __restrict__,
			 size_t);
wchar_t* __cdecl __MINGW_NOTHROW wmemmove(wchar_t* s1, const wchar_t *, size_t);
long long __cdecl __MINGW_NOTHROW wcstoll(const wchar_t * __restrict__,
			  wchar_t** __restrict__, int);
unsigned long long __cdecl __MINGW_NOTHROW wcstoull(const wchar_t * __restrict__,
			    wchar_t ** __restrict__, int);
#endif /* __NO_ISOCEXT */

#ifndef	__STRICT_ANSI__
/* non-ANSI wide char functions from io.h, direct.h, sys/stat.h and locale.h.  */

#ifndef	_FSIZE_T_DEFINED
typedef	unsigned long	_fsize_t;
#define _FSIZE_T_DEFINED
#endif

#ifndef _WFINDDATA_T_DEFINED
struct _wfinddata_t {
	unsigned	attrib;
	time_t		time_create;	/* -1 for FAT file systems */
	time_t		time_access;	/* -1 for FAT file systems */
	time_t		time_write;
	_fsize_t	size;
	wchar_t		name[FILENAME_MAX];	/* may include spaces. */
};
struct _wfinddatai64_t {
	unsigned    attrib;
	time_t      time_create;
	time_t      time_access;
	time_t      time_write;
	__int64     size;
	wchar_t     name[FILENAME_MAX];
};
struct _wfinddata64_t {
        unsigned    attrib;
        __time64_t  time_create;
        __time64_t  time_access;
        __time64_t  time_write;
/* 8 bytes are returned so it can't be _fsize_t */
        __int64    size;
        wchar_t     name[FILENAME_MAX];
};
struct _wfinddata32_t {
	unsigned	attrib;
	__time32_t	time_create;
	__time32_t	time_access;
	__time32_t	time_write;
	__int32		size;
	wchar_t		name[FILENAME_MAX];
};

struct _wfinddata32i64_t {
	unsigned	attrib;
	__time32_t	time_create;
	__time32_t	time_access;
	__time32_t	time_write;
	__int64		size;
	wchar_t		name[FILENAME_MAX];
};

struct _wfinddata64i32_t {
	unsigned	attrib;
	__time64_t	time_create;
	__time64_t	time_access;
	__time64_t	time_write;
	__int32		size;
	wchar_t		name[FILENAME_MAX];
};
#define _WFINDDATA_T_DEFINED
#endif

/* Wide character versions. Also defined in io.h. */
#if !defined (_WIO_DEFINED)
#define _WIO_DEFINED
#include <stdint.h>  /* For intptr_t.  */
_CRTIMP int __cdecl __MINGW_NOTHROW	_waccess (const wchar_t*, int);
_CRTIMP int __cdecl __MINGW_NOTHROW	_wchmod (const wchar_t*, int);
_CRTIMP int __cdecl __MINGW_NOTHROW	_wcreat (const wchar_t*, int);
_CRTIMP int __cdecl __MINGW_NOTHROW	_wunlink (const wchar_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW	_wopen (const wchar_t*, int, ...);
_CRTIMP int __cdecl __MINGW_NOTHROW	_wsopen (const wchar_t*, int, int, ...);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wmktemp (wchar_t*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wfindfirst (const wchar_t*, struct _wfinddata32_t*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW	_wfindfirsti64 (const wchar_t*, struct _wfinddatai64_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW 	_wfindnexti64 (long, struct _wfinddatai64_t*);
intptr_t __cdecl __MINGW_NOTHROW	_wfindfirst32i64 (const wchar_t*, struct _wfinddata32i64_t*);
intptr_t __cdecl __MINGW_NOTHROW	_wfindfirst64i32 (const wchar_t*, struct _wfinddata64i32_t*);
_CRTIMP int  __cdecl __MINGW_NOTHROW _wfindnext (intptr_t, struct _wfinddata32_t*);
int  __cdecl __MINGW_NOTHROW	_wfindnext32i64 (intptr_t, struct _wfinddata32i64_t*);
int  __cdecl __MINGW_NOTHROW	_wfindnext64i32 (intptr_t, struct _wfinddata64i32_t*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wfindfirst64(const wchar_t*, struct _wfinddata64_t*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wfindnext64(intptr_t, struct _wfinddata64_t*);

/* _wfindfirst32 and _wfindnext32 do not exist in MSVCRT.DLL */
_CRTALIAS intptr_t __cdecl __MINGW_NOTHROW _wfindfirst32 (const wchar_t* _v1, struct _wfinddata32_t* _v2) {
    return _wfindfirst(_v1, _v2);
}
_CRTALIAS int  __cdecl __MINGW_NOTHROW	_wfindnext32 (intptr_t _v1, struct _wfinddata32_t* _v2) {
    return _wfindnext(_v1, _v2);
}

#include <string.h>
__CRT_MAYBE_INLINE __cdecl __MINGW_NOTHROW intptr_t _wfindfirst32i64(const wchar_t* _filename, struct _wfinddata32i64_t* _fdata) {
    struct _wfinddata64_t fd;
    intptr_t ret = _wfindfirst64(_filename, &fd);
    if (ret == -1) {
        memset(_fdata, 0, sizeof(struct _wfinddata64_t));
        return ret;
    }
    _fdata->attrib = fd.attrib;
    _fdata->time_create = (__time32_t)fd.time_create;
    _fdata->time_access = (__time32_t)fd.time_access;
    _fdata->time_write  = (__time32_t)fd.time_write;
    _fdata->size        = fd.size;
    wcsncpy(_fdata->name, fd.name, FILENAME_MAX);
    return ret;
}

__CRT_MAYBE_INLINE __cdecl __MINGW_NOTHROW intptr_t _wfindfirst64i32(const wchar_t* _filename, struct _wfinddata64i32_t* _fdata) {
    struct _wfinddata32_t fd;
    intptr_t ret = _wfindfirst32(_filename, &fd);
    if (ret == -1) {
        memset(_fdata, 0, sizeof(struct _wfinddata32_t));
        return ret;
    }
    _fdata->attrib = fd.attrib;
    _fdata->time_create = (__time64_t)fd.time_create;
    _fdata->time_access = (__time64_t)fd.time_access;
    _fdata->time_write  = (__time64_t)fd.time_write;
    _fdata->size        = fd.size;
    wcsncpy(_fdata->name, fd.name, FILENAME_MAX);
    return ret;
}

__CRT_MAYBE_INLINE __cdecl __MINGW_NOTHROW intptr_t _wfindnext32i64(intptr_t _fp, struct _wfinddata32i64_t* _fdata) {
    struct _wfinddata64_t fd;
    int ret = _wfindnext64(_fp,&fd);
    if (ret == -1) {
      memset(_fdata, 0, sizeof(struct _wfinddata32i64_t));
      return ret;
    }
    _fdata->attrib = fd.attrib;
    _fdata->time_create = (__time32_t)fd.time_create;
    _fdata->time_access = (__time32_t)fd.time_access;
    _fdata->time_write  = (__time32_t)fd.time_write;
    _fdata->size        = fd.size;
    wcsncpy(_fdata->name, fd.name, FILENAME_MAX);
    return ret;
}

__CRT_MAYBE_INLINE __cdecl __MINGW_NOTHROW intptr_t _wfindnext64i32(intptr_t _fp, struct _wfinddata64i32_t* _fdata) {
    struct _wfinddata32_t fd;
    int ret = _wfindnext32(_fp, &fd);
    if (ret == -1) {
      memset(_fdata, 0, sizeof(struct _wfinddata64i32_t));
      return ret;
    }
    _fdata->attrib = fd.attrib;
    _fdata->time_create = (__time64_t)fd.time_create;
    _fdata->time_access = (__time64_t)fd.time_access;
    _fdata->time_write  = (__time64_t)fd.time_write;
    _fdata->size        = fd.size;
    wcsncpy(_fdata->name, fd.name, FILENAME_MAX);
    return ret;
}

#if defined(_USE_32BIT_TIME_T)
#define _wfinddata_t _wfinddata32_t
#define _wfinddatai64_t _wfinddata32i64_t
#define _wfindfirst _wfindfirst32
#define _wfindnext _wfindnext32
#define _wfindfirsti64 _wfindfirst32i64
#define _wfindnexti64 _wfindnext32i64

#else /* !defined(_USE_32BIT_TIME_T) */
#define _wfinddata_t _wfinddata64i32_t
#define _wfinddatai64_t _wfinddata64_t
#define _wfindfirst _wfindfirst64i32
#define _wfindnext _wfindnext64i32
#define _wfindfirsti64 _wfindfirst64
#define _wfindnexti64 _wfindnext64

#endif /* defined(_USE_32BIT_TIME_T) */

#endif /* _WIO_DEFINED */

#ifndef _WDIRECT_DEFINED
/* Also in direct.h */
_CRTIMP int __cdecl __MINGW_NOTHROW	  _wchdir (const wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW  _wgetcwd (wchar_t*, int);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW  _wgetdcwd (int, wchar_t*, int);
_CRTIMP int __cdecl __MINGW_NOTHROW	  _wmkdir (const wchar_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW	  _wrmdir (const wchar_t*);
#define _WDIRECT_DEFINED
#endif /* _WDIRECT_DEFINED */

/* @TODO: Use sys/stat.h to define these instead of duplicated code. */
#ifndef _STAT_DEFINED
/*
 * The structure manipulated and returned by stat and fstat.
 *
 * NOTE: If called on a directory the values in the time fields are not only
 * invalid, they will cause localtime et. al. to return NULL. And calling
 * asctime with a NULL pointer causes an Invalid Page Fault. So watch it!
 */
struct _stat32
{
	_dev_t	   st_dev;	/* Equivalent to drive number 0=A 1=B ... */
	_ino_t	   st_ino;	/* Always zero ? */
	_mode_t	   st_mode;	/* See above constants */
	short	   st_nlink;	/* Number of links. */
	short	   st_uid;	/* User: Maybe significant on NT ? */
	short	   st_gid;	/* Group: Ditto */
	_dev_t	   st_rdev;	/* Seems useless (not even filled in) */
	_off_t	   st_size;	/* File size in bytes */
	__time32_t st_atime;	/* Accessed date (always 00:00 hrs local
				 * on FAT) */
	__time32_t st_mtime;	/* Modified time */
	__time32_t st_ctime;	/* Creation time */
};

#ifndef __STRICT_ANSI__
struct stat {
	_dev_t	   st_dev;	/* Equivalent to drive number 0=A 1=B ... */
	_ino_t	   st_ino;	/* Always zero ? */
	_mode_t    st_mode;	/* See above constants */
	short	   st_nlink;	/* Number of links. */
	short	   st_uid;	/* User: Maybe significant on NT ? */
	short	   st_gid;	/* Group: Ditto */
	_dev_t	   st_rdev;	/* Seems useless (not even filled in) */
	_off_t	   st_size;	/* File size in bytes */
	time_t	   st_atime;	/* Accessed date (always 00:00 hrs local
				 * on FAT) */
	time_t	   st_mtime;	/* Modified time */
	time_t	   st_ctime;	/* Creation time */
};
#endif /* __STRICT_ANSI__ */

struct _stat64 {
	dev_t	   st_dev;	/* Equivalent to drive number 0=A 1=B ... */
	ino_t	   st_ino;	/* Always zero ? */
	mode_t	   st_mode;	/* See above constants */
	short	   st_nlink;	/* Number of links. */
	short	   st_uid;	/* User: Maybe significant on NT ? */
	short	   st_gid;	/* Group: Ditto */
	dev_t	   st_rdev;	/* Seems useless (not even filled in) */
	_off64_t   st_size;	/* File size in bytes */
	__time64_t st_atime;	/* Accessed date (always 00:00 hrs local
				 * on FAT) */
	__time64_t st_mtime;	/* Modified time */
	__time64_t st_ctime;	/* Creation time */
};

struct _stat32i64 {
	_dev_t		st_dev;
	_ino_t		st_ino;
	_mode_t		st_mode;
	short		st_nlink;
	short		st_uid;
	short		st_gid;
	_dev_t		st_rdev;
	_off64_t	st_size;
	__time32_t	st_atime;
	__time32_t	st_mtime;
	__time32_t	st_ctime;
};

struct _stat64i32 {
	_dev_t		st_dev;
	_ino_t		st_ino;
	_mode_t		st_mode;
	short		st_nlink;
	short		st_uid;
	short		st_gid;
	_dev_t		st_rdev;
	_off_t		st_size;
	__time64_t	st_atime;
	__time64_t	st_mtime;
	__time64_t	st_ctime;
};

/* _stat32 does not exist in MSVCRT.DLL */
_CRTIMP int __cdecl __MINGW_NOTHROW _stat (const char*, struct _stat32*);
_CRTALIAS int __cdecl __MINGW_NOTHROW _stat32 (const char* _v1, struct _stat32* _v2) {
  return _stat(_v1, _v2);
}

_CRTIMP int __cdecl __MINGW_NOTHROW _stat64 (const char*, struct _stat64*);
int __cdecl __MINGW_NOTHROW _stat32i64 (const char*, struct _stat32i64*);
int __cdecl __MINGW_NOTHROW _stat64i32 (const char*, struct _stat64i32*);

/* _fstat32 does not exist in MSVCRT.DLL */
_CRTIMP int __cdecl __MINGW_NOTHROW _fstat (int, struct _stat32*);
_CRTALIAS int __cdecl __MINGW_NOTHROW _fstat32 (int _v1, struct _stat32* _v2) {
    return _fstat(_v1, _v2);
}

_CRTIMP int __cdecl __MINGW_NOTHROW _fstat64 (int, struct _stat64*);
int __cdecl __MINGW_NOTHROW _fstat32i64 (int, struct _stat32i64*);
int __cdecl __MINGW_NOTHROW _fstat64i32 (int, struct _stat64i32*);

__CRT_MAYBE_INLINE int __cdecl _fstat64i32(int desc, struct _stat64i32 *_stat) {
  struct _stat64 st;
  int ret = _fstat64(desc, &st);
  if (ret == -1) {
    memset(_stat, 0, sizeof(struct _stat64i32));
    return -1;
  }
  _stat->st_dev = st.st_dev;
  _stat->st_ino = st.st_ino;
  _stat->st_mode = st.st_mode;
  _stat->st_nlink = st.st_nlink;
  _stat->st_uid = st.st_uid;
  _stat->st_gid = st.st_gid;
  _stat->st_rdev = st.st_rdev;
  _stat->st_size = (_off_t) st.st_size;
  _stat->st_atime = st.st_atime;
  _stat->st_mtime = st.st_mtime;
  _stat->st_ctime = st.st_ctime;
  return ret;
}
__CRT_MAYBE_INLINE int __cdecl _fstat32i64(int desc, struct _stat32i64 *_stat) {
  struct _stat32 st;
  int ret = _fstat32(desc, &st);
  if (ret == -1) {
    memset(_stat, 0, sizeof(struct _stat32i64));
    return -1;
  }
  _stat->st_dev = st.st_dev;
  _stat->st_ino = st.st_ino;
  _stat->st_mode = st.st_mode;
  _stat->st_nlink = st.st_nlink;
  _stat->st_uid = st.st_uid;
  _stat->st_gid = st.st_gid;
  _stat->st_rdev = st.st_rdev;
  _stat->st_size = (_off_t) st.st_size;
  _stat->st_atime = st.st_atime;
  _stat->st_mtime = st.st_mtime;
  _stat->st_ctime = st.st_ctime;
  return ret;
}
__CRT_MAYBE_INLINE int __cdecl _stat64i32(const char *fname, struct _stat64i32 *_stat) {
  struct _stat64 st;
  int ret = _stat64(fname, &st);
  if (ret == -1) {
    memset(_stat, 0, sizeof(struct _stat64i32));
    return -1;
  }
  _stat->st_dev = st.st_dev;
  _stat->st_ino = st.st_ino;
  _stat->st_mode = st.st_mode;
  _stat->st_nlink = st.st_nlink;
  _stat->st_uid = st.st_uid;
  _stat->st_gid = st.st_gid;
  _stat->st_rdev = st.st_rdev;
  _stat->st_size = (_off_t) st.st_size;
  _stat->st_atime = st.st_atime;
  _stat->st_mtime = st.st_mtime;
  _stat->st_ctime = st.st_ctime;
  return ret;
}
__CRT_MAYBE_INLINE int __cdecl _stat32i64(const char *fname, struct _stat32i64 *_stat) {
  struct _stat32 st;
  int ret = _stat32(fname, &st);
  if (ret == -1) {
    memset(_stat, 0, sizeof(struct _stat32i64));
    return -1;
  }
  _stat->st_dev = st.st_dev;
  _stat->st_ino = st.st_ino;
  _stat->st_mode = st.st_mode;
  _stat->st_nlink = st.st_nlink;
  _stat->st_uid = st.st_uid;
  _stat->st_gid = st.st_gid;
  _stat->st_rdev = st.st_rdev;
  _stat->st_size = (_off_t) st.st_size;
  _stat->st_atime = st.st_atime;
  _stat->st_mtime = st.st_mtime;
  _stat->st_ctime = st.st_ctime;
  return ret;
}

#define __stat64 _stat64
#if defined(_USE_32BIT_TIME_T)
#define _fstat      _fstat32
#define _fstati64   _fstat32i64
#define _stat       _stat32
#define _stati64    _stat32i64

#else  /* !_USE_32BIT_TIME_T */
#define _fstat      _fstat64i32
#define _fstati64   _fstat64
#define _stat       _stat64i32
#define _stati64    _stat64

#endif /* _USE_32BIT_TIME_T */
#define _STAT_DEFINED

#endif /* _STAT_DEFINED */

#if !defined ( _WSTAT_DEFINED) /* also declared in sys/stat.h */
/* _wstat32 does not exist in MSVCRT.DLL */
_CRTIMP int __cdecl __MINGW_NOTHROW _wstat (const wchar_t*, struct _stat32*);
_CRTALIAS int __cdecl __MINGW_NOTHROW _wstat32 (const wchar_t* _v1, struct _stat32* _v2) {
    return _wstat(_v1, _v2);
}

_CRTIMP int __cdecl __MINGW_NOTHROW _wstat64 (const wchar_t*, struct _stat64*);
_CRTIMP int __cdecl __MINGW_NOTHROW _wstat32i64 (const wchar_t*, struct _stat32i64*);
int __cdecl __MINGW_NOTHROW _wstat64i32 (const wchar_t*, struct _stat64i32*);
#include <string.h> /* Need memset declaration. */
__CRT_MAYBE_INLINE int __cdecl _wstat64i32(const wchar_t *fname, struct _stat64i32 *_stat)
  {
    struct _stat64 st;
    int ret = _wstat64(fname, &st);
    if (ret == -1) {
      memset(_stat, 0, sizeof(struct _stat64i32));
      return -1;
    }
    _stat->st_dev = st.st_dev;
    _stat->st_ino = st.st_ino;
    _stat->st_mode = st.st_mode;
    _stat->st_nlink = st.st_nlink;
    _stat->st_uid = st.st_uid;
    _stat->st_gid = st.st_gid;
    _stat->st_rdev = st.st_rdev;
    _stat->st_size = (_off_t) st.st_size;
    _stat->st_atime = st.st_atime;
    _stat->st_mtime = st.st_mtime;
    _stat->st_ctime = st.st_ctime;
    return ret;
}
__CRT_MAYBE_INLINE int __cdecl _wstat32i64(const wchar_t *fname, struct _stat32i64 *_stat) {
  struct _stat32 st;
  int ret = _wstat32(fname, &st);
  if (ret == -1) {
    memset(_stat, 0, sizeof(struct _stat32i64));
    return -1;
  }
  _stat->st_dev = st.st_dev;
  _stat->st_ino = st.st_ino;
  _stat->st_mode = st.st_mode;
  _stat->st_nlink = st.st_nlink;
  _stat->st_uid = st.st_uid;
  _stat->st_gid = st.st_gid;
  _stat->st_rdev = st.st_rdev;
  _stat->st_size = (_off_t) st.st_size;
  _stat->st_atime = st.st_atime;
  _stat->st_mtime = st.st_mtime;
  _stat->st_ctime = st.st_ctime;
  return ret;
}

#if defined(_USE_32BIT_TIME_T)
#define _wstat      _wstat32
#define _wstati64   _wstat32i64

#else /* ! _USE_32BIT_TIME_T */
#define _wstat      _wstat64i32
#define _wstati64   _wstat64
#endif /* _USE_32BIT_TIME_T */

#define _WSTAT_DEFINED
#endif /* _WSTAT_DEFIND */

#ifndef _WLOCALE_DEFINED  /* also declared in locale.h */
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wsetlocale (int, const wchar_t*);
#define _WLOCALE_DEFINED
#endif

#ifndef _WPROCESS_DEFINED  /* also declared in process.h */
#include <stdint.h>  /* For intptr_t.  */
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wexecl	(const wchar_t*, const wchar_t*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wexecle	(const wchar_t*, const wchar_t*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wexeclp	(const wchar_t*, const wchar_t*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wexeclpe	(const wchar_t*, const wchar_t*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wexecv	(const wchar_t*, const wchar_t* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wexecve	(const wchar_t*, const wchar_t* const*, const wchar_t* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wexecvp	(const wchar_t*, const wchar_t* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wexecvpe	(const wchar_t*, const wchar_t* const*, const wchar_t* const*);

_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wspawnl	(int, const wchar_t*, const wchar_t*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wspawnle	(int, const wchar_t*, const wchar_t*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wspawnlp	(int, const wchar_t*, const wchar_t*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wspawnlpe	(int, const wchar_t*, const wchar_t*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wspawnv	(int, const wchar_t*, const wchar_t* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wspawnve	(int, const wchar_t*, const wchar_t* const*, const wchar_t* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wspawnvp	(int, const wchar_t*, const wchar_t* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wspawnvpe	(int, const wchar_t*, const wchar_t* const*, const wchar_t* const*);

#define _WPROCESS_DEFINED
#endif

#endif /* not __STRICT_ANSI__ */

#ifdef __cplusplus
}	/* end of extern "C" */
#endif

#endif /* Not RC_INVOKED */

#endif /* not _WCHAR_H_ */
