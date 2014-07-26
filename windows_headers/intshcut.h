/**
 * @file intshcut.h
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
#ifndef _INTSHCUT_H
#define _INTSHCUT_H
#pragma GCC system_header
#include <_mingw.h>

#include <isguids.h>

#ifdef __cplusplus
extern "C" { 
#endif 

#define INTSHCUTAPI DECLSPEC_IMPORT
#define E_FLAGS MAKE_SCODE(SEVERITY_ERROR,FACILITY_ITF,0x1000)
#define IS_E_EXEC_FAILED MAKE_SCODE(SEVERITY_ERROR,FACILITY_ITF,0x2002)
#define URL_E_INVALID_SYNTAX MAKE_SCODE(SEVERITY_ERROR,FACILITY_ITF,0x1001)
#define URL_E_UNREGISTERED_PROTOCOL MAKE_SCODE(SEVERITY_ERROR,FACILITY_ITF,0x1002)
typedef enum iurl_seturl_flags {
 IURL_SETURL_FL_GUESS_PROTOCOL=1,
 IURL_SETURL_FL_USE_DEFAULT_PROTOCOL,
 ALL_IURL_SETURL_FLAGS=(IURL_SETURL_FL_GUESS_PROTOCOL|IURL_SETURL_FL_USE_DEFAULT_PROTOCOL)
} IURL_SETURL_FLAGS;
typedef enum iurl_invokecommand_flags {
 IURL_INVOKECOMMAND_FL_ALLOW_UI=1,
 IURL_INVOKECOMMAND_FL_USE_DEFAULT_VERB,
 ALL_IURL_INVOKECOMMAND_FLAGS=(IURL_INVOKECOMMAND_FL_ALLOW_UI|IURL_INVOKECOMMAND_FL_USE_DEFAULT_VERB)
} IURL_INVOKECOMMAND_FLAGS;
typedef enum translateurl_in_flags {
 TRANSLATEURL_FL_GUESS_PROTOCOL=1,
 TRANSLATEURL_FL_USE_DEFAULT_PROTOCOL,
 ALL_TRANSLATEURL_FLAGS=(TRANSLATEURL_FL_GUESS_PROTOCOL|TRANSLATEURL_FL_USE_DEFAULT_PROTOCOL)
} TRANSLATEURL_IN_FLAGS;
typedef enum urlassociationdialog_in_flags {
 URLASSOCDLG_FL_USE_DEFAULT_NAME=1,
 URLASSOCDLG_FL_REGISTER_ASSOC,
 ALL_URLASSOCDLG_FLAGS=(URLASSOCDLG_FL_USE_DEFAULT_NAME|URLASSOCDLG_FL_REGISTER_ASSOC)
} URLASSOCIATIONDIALOG_IN_FLAGS;
typedef enum mimeassociationdialog_in_flags {
 MIMEASSOCDLG_FL_REGISTER_ASSOC=1,
 ALL_MIMEASSOCDLG_FLAGS=MIMEASSOCDLG_FL_REGISTER_ASSOC
} MIMEASSOCIATIONDIALOG_IN_FLAGS;
typedef struct urlinvokecommandinfo {
 DWORD dwcbSize;
 DWORD dwFlags;
 HWND hwndParent;
 PCSTR pcszVerb;
} URLINVOKECOMMANDINFO,*PURLINVOKECOMMANDINFO;
typedef const URLINVOKECOMMANDINFO CURLINVOKECOMMANDINFO;
typedef const URLINVOKECOMMANDINFO *PCURLINVOKECOMMANDINFO;
#define INTERFACE IUniformResourceLocator
DECLARE_INTERFACE_(IUniformResourceLocator,IUnknown)
{
 STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
 STDMETHOD_(ULONG,AddRef)(THIS) PURE;
 STDMETHOD_(ULONG,Release)(THIS) PURE;
 STDMETHOD(SetURL)(THIS_ PCSTR,DWORD) PURE;
 STDMETHOD(GetURL)(THIS_ PSTR*) PURE;
 STDMETHOD(InvokeCommand)(THIS_ PURLINVOKECOMMANDINFO) PURE;
};
#undef INTERFACE
typedef IUniformResourceLocator *PIUniformResourceLocator;
typedef const IUniformResourceLocator CIUniformResourceLocator;
typedef const IUniformResourceLocator *PCIUniformResourceLocator;

BOOL WINAPI InetIsOffline(DWORD);
HRESULT WINAPI MIMEAssociationDialogA(HWND,DWORD,PCSTR,PCSTR,PSTR,UINT);
HRESULT WINAPI MIMEAssociationDialogW(HWND,DWORD,PCWSTR,PCWSTR,PWSTR,UINT);
HRESULT WINAPI TranslateURLA(PCSTR,DWORD,PSTR*);
HRESULT WINAPI TranslateURLW(PCWSTR,DWORD,PWSTR*);
HRESULT WINAPI URLAssociationDialogA(HWND,DWORD,PCSTR,PCSTR,PSTR,UINT);
HRESULT WINAPI URLAssociationDialogW(HWND,DWORD,PCWSTR,PCWSTR,PWSTR,UINT);
#define TranslateURL __AW(TranslateURL)
#define MIMEAssociationDialog __AW(MIMEAssociationDialog)
#define URLAssociationDialog __AW(URLAssociationDialog)

#ifdef __cplusplus
} 
#endif 

#endif 
