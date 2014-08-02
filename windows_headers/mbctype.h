/**
 * @file mbctype.h
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
#ifndef _MBCTYPE_H
#define _MBCTYPE_H
#pragma GCC system_header
#include <_mingw.h>

/* 
 * Functions for testing multibyte character types and converting characters.
 */

/* return values for _mbsbtype  and  _mbbtype in mbstring.h */
#define _MBC_SINGLE 0 
#define _MBC_LEAD 1  
#define _MBC_TRAIL 2
#define _MBC_ILLEGAL (-1)

/*  args for setmbcp (in lieu of actual codepage)  */
#define _MB_CP_SBCS 0
#define _MB_CP_OEM (-2)
#define _MB_CP_ANSI (-3)
#define _MB_CP_LOCALE (-4)

/* TODO: bit masks */
/*
#define _MS
#define _MP
#define _M1
#define _M2
#define _SBUP
#define _SBLOW
*/

#ifndef RC_INVOKED

#ifdef __cplusplus 
extern "C" {
#endif

#ifndef	__STRICT_ANSI__

_CRTIMP int __cdecl __MINGW_NOTHROW _setmbcp (int);
_CRTIMP int __cdecl __MINGW_NOTHROW _getmbcp (void);

/* byte classification  */
/* NB: Corresponding _ismbc* functions are in mbstring.h */

_CRTIMP int __cdecl __MINGW_NOTHROW _ismbbalpha (unsigned int);
_CRTIMP int __cdecl __MINGW_NOTHROW _ismbbalnum (unsigned int);
_CRTIMP int __cdecl __MINGW_NOTHROW _ismbbgraph (unsigned int);
_CRTIMP int __cdecl __MINGW_NOTHROW _ismbbprint (unsigned int);
_CRTIMP int __cdecl __MINGW_NOTHROW _ismbbpunct (unsigned int);

_CRTIMP int __cdecl __MINGW_NOTHROW _ismbbkana (unsigned int);
_CRTIMP int __cdecl __MINGW_NOTHROW _ismbbkalnum (unsigned int);
_CRTIMP int __cdecl __MINGW_NOTHROW _ismbbkprint (unsigned int);
_CRTIMP int __cdecl __MINGW_NOTHROW _ismbbkpunct (unsigned int);


/* these are also in mbstring.h */
_CRTIMP int __cdecl __MINGW_NOTHROW _ismbblead (unsigned int);
_CRTIMP int __cdecl __MINGW_NOTHROW _ismbbtrail (unsigned int);
_CRTIMP int __cdecl __MINGW_NOTHROW _ismbslead (const unsigned char*, const unsigned char*);
_CRTIMP int __cdecl __MINGW_NOTHROW _ismbstrail (const unsigned char*, const unsigned char*);

__MINGW_IMPORT unsigned char _mbctype[];
__MINGW_IMPORT unsigned char _mbcasemap[];

/* TODO : _MBCS_ mappings go in tchar.h */

#endif	/* Not strict ANSI */

#ifdef __cplusplus
}
#endif

#endif	/* Not RC_INVOKED */

#endif	/* Not _MCTYPE_H */
