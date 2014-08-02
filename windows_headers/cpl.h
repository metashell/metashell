/**
 * @file cpl.h
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
#ifndef _CPL_H
#define _CPL_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WM_CPL_LAUNCH (WM_USER+1000)
#define WM_CPL_LAUNCHED (WM_USER+1001)
#define CPL_DYNAMIC_RES 0
#define CPL_INIT 1
#define CPL_GETCOUNT 2
#define CPL_INQUIRE 3
#define CPL_SELECT 4
#define CPL_DBLCLK 5
#define CPL_STOP 6
#define CPL_EXIT 7
#define CPL_NEWINQUIRE 8
#define CPL_STARTWPARMSA 9
#define CPL_STARTWPARMSW 10
#define CPL_SETUP 200
typedef LONG(APIENTRY *APPLET_PROC)(HWND,UINT,LONG,LONG);
typedef struct tagCPLINFO {
	int idIcon;
	int idName;
	int idInfo;
	LONG lData;
} CPLINFO,*LPCPLINFO;
typedef struct tagNEWCPLINFOA {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwHelpContext;
	LONG lData;
	HICON hIcon;
	CHAR szName[32];
	CHAR szInfo[64];
	CHAR szHelpFile[128];
} NEWCPLINFOA,*LPNEWCPLINFOA;
typedef struct tagNEWCPLINFOW {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwHelpContext;
	LONG lData;
	HICON hIcon;
	WCHAR szName[32];
	WCHAR szInfo[64];
	WCHAR szHelpFile[128];
} NEWCPLINFOW,*LPNEWCPLINFOW;
#define CPL_STARTWPARMS __AW(CPL_STARTWPARMS)
typedef __AW(NEWCPLINFO) NEWCPLINFO,*LPNEWCPLINFO;

#ifdef __cplusplus
}
#endif

#endif
