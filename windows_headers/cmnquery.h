/**
 * @file cmnquery.h
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
#ifndef _CMNQUERY_H
#define _CMNQUERY_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

/*--- Active Directory Reference - Active Directory Structures - Active Directory Display Structures */
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN2K)
typedef struct {
	DWORD cbStruct;
	DWORD dwFlags;
	CLSID clsid;
	HICON hIcon;
	LPWSTR pszTitle;
} CQFORM,*LPCQFORM;
#define CQFF_NOGLOBALPAGES 0x00000001
#define CQFF_ISOPTIONAL 0x00000002
typedef struct {
	DWORD cbStruct;
	DWORD dwFlags;
	LPCQPAGEPROC pPageProc;
	HINSTANCE hInstance;
	INT idPageName;
	INT idPageTemplate;
	DLGPROC pDlgProc;
	LPARAM lParam;
} CQPAGE,*LPCQPAGE;
/*********
typedef struct {
	DWORD cbStruct;
	DWORD dwFlags;
	CLSID clsidHandler;
	LPVOID pHandlerParameters;
	CLSID clsidDefaultForm;
	IPersistQuery* pPersistQuery;
	union {
		void* pFormParameters;
		IPropertyBag* ppbFormParameters;
	};
} OPENQUERYWINDOW,*LPOPENQUERYWINDOW;
*********/
#define OQWF_OKCANCEL 0x00000001
#define OQWF_DEFAULTFORM 0x00000002
#define OQWF_SINGLESELECT 0x00000004
#define OQWF_LOADQUERY 0x00000008
#define OQWF_REMOVESCOPES 0x00000010
#define OQWF_REMOVEFORMS 0x00000020
#define OQWF_ISSUEONOPEN 0x00000040
#define OQWF_SHOWOPTIONAL 0x00000080
#define OQWF_SAVEQUERYONOK 0x00000200
#define OQWF_HIDEMENUS 0x00000400
#define OQWF_HIDESEARCHUI 0x00000800
#define OQWF_PARAMISPROPERTYBAG 0x80000000
/*--- Active Directory Reference - Active Directory Functions - Active Directory Display Functions */
typedef HRESULT (CALLBACK* CQAddFormsProc)(LPARAM,LPCQFORM);
typedef HRESULT (CALLBACK* CQAddPagesProc)(LPARAM,REFCLSID,LPCQPAGE);
typedef HRESULT (CALLBACK* CQPageProc)(LPCQPAGE,HWND,UINT,WPARAM,LPARAM);
#endif /* (_WIN32_WINNT >= _WIN32_WINNT_WIN2K) */

#ifdef __cplusplus
}
#endif
#endif
