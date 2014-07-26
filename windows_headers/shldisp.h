/**
 * @file shldisp.h
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
#ifndef _SHLDISP_H
#define _SHLDISP_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum tagAUTOCOMPLETEOPTIONS {
    ACO_NONE = 0x00,
    ACO_AUTOSUGGEST = 0x01,
    ACO_AUTOAPPEND = 0x02,
    ACO_SEARCH = 0x04,
    ACO_FILTERPREFIXES = 0x08,
    ACO_USETAB = 0x10,
    ACO_UPDOWNKEYDROPSLIST = 0x20,
    ACO_RTLREADING = 0x40,
#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
    ACO_WORD_FILTER = 0x80,
    ACO_NOPREFIXFILTERING = 0x100
#endif
} AUTOCOMPLETEOPTIONS;

#define INTERFACE IAutoComplete
DECLARE_INTERFACE_(IAutoComplete, IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(Init)(THIS_ HWND,IUnknown*,LPCOLESTR,LPCOLESTR) PURE;
	STDMETHOD(Enable)(THIS_ BOOL) PURE;
};
#undef INTERFACE
typedef IAutoComplete *LPAUTOCOMPLETE;

#ifdef COBJMACROS
#define IAutoComplete_QueryInterface(T,a,b) (T)->lpVtbl->QueryInterface(T,a,b)
#define IAutoComplete_AddRef(T) (T)->lpVtbl->AddRef(T)
#define IAutoComplete_Release(T) (T)->lpVtbl->Release(T)
#define IAutoComplete_Init(T,a,b,c,d) (T)->lpVtbl->Init(T,a,b,c,d)
#define IAutoComplete_Enable(T,a) (T)->lpVtbl->Enable(T,a)
#endif

#define INTERFACE IAutoComplete2
DECLARE_INTERFACE_(IAutoComplete2, IAutoComplete)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(Init)(THIS_ HWND,IUnknown*,LPCOLESTR,LPCOLESTR) PURE;
	STDMETHOD(Enable)(THIS_ BOOL) PURE;
	STDMETHOD(SetOptions)(THIS_ DWORD) PURE;
	STDMETHOD(GetOptions)(THIS_ DWORD*) PURE;
};
#undef INTERFACE
typedef IAutoComplete2 *LPAUTOCOMPLETE2;

#ifdef COBJMACROS
#define IAutoComplete2_QueryInterface(T,a,b) (T)->lpVtbl->QueryInterface(T,a,b)
#define IAutoComplete2_AddRef(T) (T)->lpVtbl->AddRef(T)
#define IAutoComplete2_Release(T) (T)->lpVtbl->Release(T)
#define IAutoComplete2_Init(T,a,b,c,d) (T)->lpVtbl->Init(T,a,b,c,d)
#define IAutoComplete2_Enable(T,a) (T)->lpVtbl->Enable(T,a)
#define IAutoComplete2_SetOptions(T,a) (T)->lpVtbl->Enable(T,a)
#define IAutoComplete2_GetOptions(T,a) (T)->lpVtbl->Enable(T,a)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _SHLDISP_H */
