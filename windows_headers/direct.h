/**
 * @file direct.h
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
#ifndef	_DIRECT_H_
#define	_DIRECT_H_
#pragma GCC system_header
#include <_mingw.h>

#define __need_wchar_t
#ifndef RC_INVOKED
#include <stddef.h>
#endif	/* Not RC_INVOKED */

#include <io.h>

#ifndef RC_INVOKED

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef _DISKFREE_T_DEFINED
/* needed by _getdiskfree (also in dos.h) */
struct _diskfree_t {
	unsigned total_clusters;
	unsigned avail_clusters;
	unsigned sectors_per_cluster;
	unsigned bytes_per_sector;
};
#define _DISKFREE_T_DEFINED
#endif  

/*
 * You really shouldn't be using these. Use the Win32 API functions instead.
 * However, it does make it easier to port older code.
 */
_CRTIMP int __cdecl __MINGW_NOTHROW _getdrive (void);
_CRTIMP unsigned long __cdecl __MINGW_NOTHROW _getdrives(void);
_CRTIMP int __cdecl __MINGW_NOTHROW _chdrive (int);
_CRTIMP char* __cdecl __MINGW_NOTHROW _getdcwd (int, char*, int);
_CRTIMP unsigned __cdecl __MINGW_NOTHROW _getdiskfree (unsigned, struct _diskfree_t *);

#ifndef	_NO_OLDNAMES
# define diskfree_t _diskfree_t
#endif

#ifndef _WDIRECT_DEFINED
/* wide character versions. Also in wchar.h */
_CRTIMP int __cdecl __MINGW_NOTHROW _wchdir(const wchar_t*);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wgetcwd(wchar_t*, int);
_CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wgetdcwd(int, wchar_t*, int);
_CRTIMP int __cdecl __MINGW_NOTHROW _wmkdir(const wchar_t*);
_CRTIMP int __cdecl __MINGW_NOTHROW _wrmdir(const wchar_t*);
#define _WDIRECT_DEFINED
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* Not RC_INVOKED */

#endif	/* Not _DIRECT_H_ */
