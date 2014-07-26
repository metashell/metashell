/**
 * @file servprov.h
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
#ifndef _SERVPROV_H
#define _SERVPROV_H
#define _OLEIDL_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

EXTERN_C const IID IID_IServiceProvider;
#define INTERFACE IServiceProvider
DECLARE_INTERFACE_(IServiceProvider,IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(QueryService)(THIS_ REFGUID,REFIID,void**) PURE;
};
#undef INTERFACE

#ifdef COBJMACROS
#define IServiceProvider_QueryInterface(T,a,b) (T)->lpVtbl->QueryInterface(T,a,b)
#define IServiceProvider_AddRef(T) (T)->lpVtbl->AddRef(T)
#define IServiceProvider_Release(T) (T)->lpVtbl->Release(T)
#define IServiceProvider_QueryService(T,a,b,c) (T)->lpVtbl->QueryService(T,a,b,c)
#endif

#ifdef __cplusplus
}
#endif

#endif
