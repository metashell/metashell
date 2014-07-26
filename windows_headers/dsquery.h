/**
 * @file dsquery.h
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
#ifndef _DSQUERY_H
#define _DSQUERY_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

/*--- Active Directory Reference - Active Directory Structures - Active Directory Display Structures */
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN2K)
typedef struct {
	DWORD dwFlags;
	INT fmt;
	INT cx;
	INT idsName;
	LONG offsetProperty;
	DWORD dwReserved;
} DSCOLUMN,*LPDSCOLUMN;
#define DSCOLUMNPROP_ADSPATH -1
#define DSCOLUMNPROP_OBJECTCLASS -2
typedef struct {
	DWORD cbStruct;
	LONG cClasses;
	DWORD offsetClass[1];
} DSQUERYCLASSLIST,*LPDSQUERYCLASSLIST;
typedef struct {
	DWORD cbStruct;
	DWORD dwFlags;
	LPWSTR pDefaultScope;
	LPWSTR pDefaultSaveLocation;
	LPWSTR pUserName;
	LPWSTR pPassword;
	LPWSTR pServer;
} DSQUERYINITPARAMS,*LPDSQUERYINITPARAMS;
#define DSQPF_NOSAVE 0x00000001
#define DSQPF_SAVELOCATION 0x00000002
#define DSQPF_SHOWHIDDENOBJECTS 0x00000004
#define DSQPF_ENABLEADMINFEATURES 0x00000008
#define DSQPF_ENABLEADVANCEDFEATURES 0x00000010
#define DSQPF_HASCREDENTIALS 0x00000020
#define DSQPF_NOCHOOSECOLUMNS 0x00000040
typedef struct {
	DWORD cbStruct;
	DWORD dwFlags;
	HINSTANCE hInstance;
	LONG offsetQuery;
	LONG iColumns;
	DWORD dwReserved;
	DSCOLUMN aColumns[1];
} DSQUERYPARAMS,*LPDSQUERYPARAMS;
#endif /* (_WIN32_WINNT >= _WIN32_WINNT_WIN2K) */

#ifdef __cplusplus
}
#endif
#endif
