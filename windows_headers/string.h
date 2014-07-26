/**
 * @file string.h
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
#ifndef _STRING_H
#define	_STRING_H
#ifndef __CRT_TESTING__
#pragma GCC system_header
#endif
#include <_mingw.h>

/*
 * Define size_t, wchar_t and NULL
 */
#define __need_size_t
#define __need_wchar_t
#define	__need_NULL

#ifndef RC_INVOKED
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Prototypes of the ANSI Standard C library string functions.
 */
_CRTIMP void* __cdecl __MINGW_NOTHROW	memchr (const void*, int, size_t) __MINGW_ATTRIB_PURE;
_CRTIMP int __cdecl __MINGW_NOTHROW 	memcmp (const void*, const void*, size_t) __MINGW_ATTRIB_PURE;
_CRTIMP void* __cdecl __MINGW_NOTHROW 	memcpy (void*, const void*, size_t);
_CRTIMP void* __cdecl __MINGW_NOTHROW	memmove (void*, const void*, size_t);
_CRTIMP void* __cdecl __MINGW_NOTHROW	memset (void*, int, size_t);
_CRTIMP char* __cdecl __MINGW_NOTHROW	strcat (char*, const char*);
_CRTIMP char* __cdecl __MINGW_NOTHROW	strchr (const char*, int)  __MINGW_ATTRIB_PURE;
_CRTIMP int __cdecl __MINGW_NOTHROW	strcmp (const char*, const char*)  __MINGW_ATTRIB_PURE;
_CRTIMP int __cdecl __MINGW_NOTHROW	strcoll (const char*, const char*);	/* Compare using locale */
_CRTIMP char* __cdecl __MINGW_NOTHROW	strcpy (char*, const char*);
_CRTIMP size_t __cdecl __MINGW_NOTHROW	strcspn (const char*, const char*)  __MINGW_ATTRIB_PURE;
_CRTIMP char* __cdecl __MINGW_NOTHROW	strerror (int); /* NOTE: NOT an old name wrapper. */

_CRTIMP size_t __cdecl __MINGW_NOTHROW	strlen (const char*)  __MINGW_ATTRIB_PURE;
_CRTIMP char* __cdecl __MINGW_NOTHROW	strncat (char*, const char*, size_t);
_CRTIMP int __cdecl __MINGW_NOTHROW	strncmp (const char*, const char*, size_t)  __MINGW_ATTRIB_PURE;
_CRTIMP char* __cdecl __MINGW_NOTHROW	strncpy (char*, const char*, size_t);
_CRTIMP char* __cdecl __MINGW_NOTHROW	strpbrk (const char*, const char*)  __MINGW_ATTRIB_PURE;
_CRTIMP char* __cdecl __MINGW_NOTHROW	strrchr (const char*, int)  __MINGW_ATTRIB_PURE;
_CRTIMP size_t __cdecl __MINGW_NOTHROW	strspn (const char*, const char*)  __MINGW_ATTRIB_PURE;
_CRTIMP char* __cdecl __MINGW_NOTHROW	strstr (const char*, const char*)  __MINGW_ATTRIB_PURE;
_CRTIMP char* __cdecl __MINGW_NOTHROW	strtok (char*, const char*);
_CRTIMP size_t __cdecl __MINGW_NOTHROW	strxfrm (char*, const char*, size_t);

#ifndef __STRICT_ANSI__
/*
 * Extra non-ANSI functions provided by the runtime library
 */
_CRTIMP char* __cdecl __MINGW_NOTHROW	_strerror (const char *);
_CRTIMP void* __cdecl __MINGW_NOTHROW	_memccpy (void*, const void*, int, size_t);
_CRTIMP int __cdecl __MINGW_NOTHROW 	_memicmp (const void*, const void*, size_t);
_CRTIMP char* __cdecl __MINGW_NOTHROW 	_strdup (const char*) __MINGW_ATTRIB_MALLOC;
_CRTIMP int __cdecl __MINGW_NOTHROW	_strcmpi (const char*, const char*);
_CRTIMP int __cdecl __MINGW_NOTHROW	_stricmp (const char*, const char*);
_CRTIMP int __cdecl __MINGW_NOTHROW	_stricoll (const char*, const char*);
_CRTIMP char* __cdecl __MINGW_NOTHROW	_strlwr (char*);
_CRTIMP int __cdecl __MINGW_NOTHROW	_strnicmp (const char*, const char*, size_t);
_CRTIMP char* __cdecl __MINGW_NOTHROW	_strnset (char*, int, size_t);
_CRTIMP char* __cdecl __MINGW_NOTHROW	_strrev (char*);
_CRTIMP char* __cdecl __MINGW_NOTHROW	_strset (char*, int);
_CRTIMP char* __cdecl __MINGW_NOTHROW	_strupr (char*);
_CRTIMP void __cdecl __MINGW_NOTHROW	_swab (const char*, char*, size_t);

_CRTIMP int __cdecl __MINGW_NOTHROW  _strncoll(const char*, const char*, size_t);
_CRTIMP int __cdecl __MINGW_NOTHROW  _strnicoll(const char*, const char*, size_t);

#ifndef	_NO_OLDNAMES
/*
 * Non-underscored versions of non-ANSI functions. They live in liboldnames.a
 * and provide a little extra portability. Also a few extra UNIX-isms like
 * strcasecmp.
 */
_CRTIMP void* __cdecl __MINGW_NOTHROW	memccpy (void*, const void*, int, size_t);
_CRTIMP int __cdecl __MINGW_NOTHROW	memicmp (const void*, const void*, size_t);
_CRTIMP char* __cdecl __MINGW_NOTHROW	strdup (const char*) __MINGW_ATTRIB_MALLOC;
_CRTIMP int __cdecl __MINGW_NOTHROW	strcmpi (const char*, const char*);
_CRTIMP int __cdecl __MINGW_NOTHROW	stricmp (const char*, const char*);
int __cdecl __MINGW_NOTHROW strcasecmp (const char*, const char *);

#ifndef __NO_INLINE__
__CRT_INLINE int __cdecl __MINGW_NOTHROW
strcasecmp (const char * __sz1, const char * __sz2)
  {return _stricmp (__sz1, __sz2);}
#else
#define strcasecmp _stricmp
#endif

_CRTIMP int __cdecl __MINGW_NOTHROW	stricoll (const char*, const char*);
_CRTIMP char* __cdecl __MINGW_NOTHROW	strlwr (char*);
_CRTIMP int __cdecl __MINGW_NOTHROW	strnicmp (const char*, const char*, size_t);
int  __cdecl __MINGW_NOTHROW strncasecmp (const char *, const char *, size_t);

#ifndef __NO_INLINE__
__CRT_INLINE int __cdecl __MINGW_NOTHROW
strncasecmp (const char * __sz1, const char * __sz2, size_t __sizeMaxCompare)
  {return _strnicmp (__sz1, __sz2, __sizeMaxCompare);}
#else
#define strncasecmp _strnicmp
#endif

_CRTIMP char* __cdecl __MINGW_NOTHROW	strnset (char*, int, size_t);
_CRTIMP char* __cdecl __MINGW_NOTHROW	strrev (char*);
_CRTIMP char* __cdecl __MINGW_NOTHROW	strset (char*, int);
_CRTIMP char* __cdecl __MINGW_NOTHROW	strupr (char*);
_CRTIMP void __cdecl __MINGW_NOTHROW	swab (const char*, char*, size_t);
#endif /* _NO_OLDNAMES */

#endif	/* Not __STRICT_ANSI__ */

#ifndef _WSTRING_DEFINED
/*
 * Unicode versions of the standard calls.
 * Also in wchar.h, where they belong according to ISO standard.
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

/* NOTE: _wcscmpi not provided by MSVCRT.DLL, this define is for portability */
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
int __cdecl __MINGW_NOTHROW wcscmpi (const wchar_t * __ws1, const wchar_t * __ws2);

#ifndef __NO_INLINE__
__CRT_INLINE int __cdecl __MINGW_NOTHROW
wcscmpi (const wchar_t * __ws1, const wchar_t * __ws2)
  {return _wcsicmp (__ws1, __ws2);}
#endif

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

#ifdef __cplusplus
}
#endif

#endif	/* Not RC_INVOKED */

#endif	/* Not _STRING_H */
