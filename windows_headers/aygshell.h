/**
 * @file aygshell.h
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
#ifndef _AYGSHELL_H
#define _AYGSHELL_H
#pragma GCC system_header
#include <_mingw.h>

#if _WIN32_WCE >= 400

#include <windef.h>             /* shellapi needs this */
#include <basetyps.h>
#include <shellapi.h>           /* for WINSHELLAPI */

typedef struct tagSHMENUBARINFO {
  DWORD cbSize;
  HWND hwndParent;
  DWORD dwFlags;
  UINT nToolBarId;
  HINSTANCE hInstRes;
  int nBmpId;
  int cBmpImages;
  HWND hwndMB;
  COLORREF clrBk;
} SHMENUBARINFO, *PSHMENUBARINFO;

typedef struct tagSHACTIVATEINFO {
  DWORD cbSize;
  HWND hwndLastFocus;
  UINT fSipUp :1;
  UINT fSipOnDeactivation :1;
  UINT fActive :1;
  UINT fReserved :29;
} SHACTIVATEINFO, *PSHACTIVATEINFO;

WINSHELLAPI BOOL WINAPI SHCreateMenuBar(SHMENUBARINFO*);
WINSHELLAPI HWND WINAPI SHFindMenuBar(HWND);
WINSHELLAPI HRESULT WINAPI SHCreateNewItem(HWND,REFCLSID);
WINSHELLAPI BOOL WINAPI SHFullScreen(HWND,DWORD);
WINSHELLAPI BOOL WINAPI SHSipInfo(UINT,UINT,PVOID,UINT);
/* next exported by ordinal only: @84 */
WINSHELLAPI BOOL WINAPI SHHandleWMActivate(HWND,WPARAM,LPARAM,SHACTIVATEINFO*,DWORD);
/* next exported by ordinal only: @83 */
WINSHELLAPI BOOL WINAPI SHHandleWMSettingChange(HWND,WPARAM,LPARAM,SHACTIVATEINFO*);

/* The following are not in device ROMs. */
extern BOOL SHInvokeContextMenuCommand(HWND,UINT,HANDLE);

#endif /* _WIN32_WCE >= 400 */

#endif
