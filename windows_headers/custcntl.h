/**
 * @file custcntl.h
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
#ifndef _CUSTCNTL_H
#define _CUSTCNTL_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CCF_NOTEXT 1
#define CCHCCCLASS 32
#define CCHCCDESC 32
#define CCHCCTEXT 256

typedef struct tagCCSTYLEA {
	DWORD flStyle;
	DWORD flExtStyle;
	CHAR szText[CCHCCTEXT];
	LANGID lgid;
	WORD wReserved1;
} CCSTYLEA,*LPCCSTYLEA;
typedef struct tagCCSTYLEW {
	DWORD flStyle;
	DWORD flExtStyle;
	WCHAR szText[CCHCCTEXT];
	LANGID lgid;
	WORD wReserved1;
} CCSTYLEW,*LPCCSTYLEW;
typedef struct tagCCSTYLEFLAGA {
	DWORD flStyle;
	DWORD flStyleMask;
	LPSTR pszStyle;
} CCSTYLEFLAGA,*LPCCSTYLEFLAGA;
typedef struct tagCCSTYLEFLAGW {
	DWORD flStyle;
	DWORD flStyleMask;
	LPWSTR pszStyle;
} CCSTYLEFLAGW,*LPCCSTYLEFLAGW;
typedef BOOL(CALLBACK* LPFNCCSTYLEA)(HWND,LPCCSTYLEA);
typedef INT(CALLBACK* LPFNCCSIZETOTEXTA)(DWORD,DWORD,HFONT,LPSTR);
typedef struct tagCCINFOA {
	CHAR szClass[CCHCCCLASS];
	DWORD flOptions;
	CHAR szDesc[CCHCCDESC];
	UINT cxDefault;
	UINT cyDefault;
	DWORD flStyleDefault;
	DWORD flExtStyleDefault;
	DWORD flCtrlTypeMask;
	CHAR szTextDefault[CCHCCTEXT];
	INT cStyleFlags;
	LPCCSTYLEFLAGA aStyleFlags;
	LPFNCCSTYLEA lpfnStyle;
	LPFNCCSIZETOTEXTA lpfnSizeToText;
	DWORD dwReserved1;
	DWORD dwReserved2;
} CCINFOA,*LPCCINFOA;
typedef UINT(CALLBACK* LPFNCCINFOA)(LPCCINFOA);
typedef BOOL(CALLBACK* LPFNCCSTYLEW)(HWND,LPCCSTYLEW);
typedef INT (CALLBACK* LPFNCCSIZETOTEXTW)(DWORD,DWORD,HFONT,LPWSTR);
typedef struct tagCCINFOW {
	WCHAR szClass[CCHCCCLASS];
	DWORD flOptions;
	WCHAR szDesc[CCHCCDESC];
	UINT cxDefault;
	UINT cyDefault;
	DWORD flStyleDefault;
	DWORD flExtStyleDefault;
	DWORD flCtrlTypeMask;
	WCHAR szTextDefault[CCHCCTEXT];
	INT cStyleFlags;
	LPCCSTYLEFLAGW aStyleFlags;
	LPFNCCSTYLEW lpfnStyle;
	LPFNCCSIZETOTEXTW lpfnSizeToText;
	DWORD dwReserved1;
	DWORD dwReserved2;
} CCINFOW,*LPCCINFOW;
typedef UINT(CALLBACK* LPFNCCINFOW)(LPCCINFOW);

UINT CALLBACK CustomControlInfoA(LPCCINFOA acci);
UINT CALLBACK CustomControlInfoW(LPCCINFOW acci);

typedef __AW(CCSTYLE) CCSTYLE,*LPCCSTYLE;
typedef __AW(CCSTYLEFLAG) CCSTYLEFLAG,*LPCCSTYLEFLAG;
typedef __AW(CCINFO) CCINFO,*LPCCINFO;
#define LPFNCCSTYLE __AW(LPFNCCSTYLE)
#define LPFNCCSIZETOTEXT __AW(LPFNCCSIZETOTEXT)
#define LPFNCCINFO __AW(LPFNCCINFO)

#ifdef __cplusplus
}
#endif

#endif
