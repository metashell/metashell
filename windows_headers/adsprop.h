/**
 * @file adsprop.h
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
#ifndef _ADSPROP_H
#define _ADSPROP_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

/*--- Active Directory Reference - Active Directory Messages */
#define WM_ADSPROP_NOTIFY_APPLY (WM_USER+1104)
#define WM_ADSPROP_NOTIFY_CHANGE (WM_USER+1103)
#define WM_ADSPROP_NOTIFY_ERROR (WM_USER+1110)
#define WM_ADSPROP_NOTIFY_EXIT (WM_USER+1107)
#define WM_ADSPROP_NOTIFY_FOREGROUND (WM_USER+1106)
#define WM_ADSPROP_NOTIFY_PAGEHWND (WM_USER+1102)
#define WM_ADSPROP_NOTIFY_PAGEINIT (WM_USER+1101)
#define WM_ADSPROP_NOTIFY_SETFOCUS (WM_USER+1105)
/*--- Active Directory Reference - Active Directory Structures - Active Directory MMC Property Page Structures */
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN2K)
typedef struct {
	DWORD dwSize;
	DWORD dwFlags;
	HRESULT hr;
	IDirectoryObject *pDsObj;
	LPWSTR pwzCN;
	PADS_ATTR_INFO pWritableAttrs;
} ADSPROPINITPARAMS,*PADSPROPINITPARAMS;
#endif /* (_WIN32_WINNT >= _WIN32_WINNT_WIN2K) */
#if (_WIN32_WINNT >= _WIN32_WINNT_WINXP)
typedef struct {
	HWND hwndPage;
	PWSTR pszPageTitle;
	PWSTR pszObjPath;
	PWSTR pszObjClass;
	HRESULT hr;
	PWSTR pszError;
} ADSPROPERROR,*PADSPROPERROR;
#endif /* (_WIN32_WINNT >= _WIN32_WINNT_WINXP) */

#ifdef __cplusplus
}
#endif
#endif
