/**
 * @file unknwn.h
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
#ifndef _UNKNWN_H
#define _UNKNWN_H
#pragma GCC system_header
#include <_mingw.h>

#ifndef COM_NO_WINDOWS_H
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C"{
#endif

#include <objfwd.h>
#include <wtypes.h>

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void*);
EXTERN_C const IID IID_IUnknown;
EXTERN_C const IID IID_IClassFactory;

#ifndef __IUnknown_INTERFACE_DEFINED__
#define __IUnknown_INTERFACE_DEFINED__
#define INTERFACE IUnknown
DECLARE_INTERFACE(IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
};
#undef INTERFACE
typedef IUnknown *LPUNKNOWN;
#endif

#ifndef __IClassFactory_INTERFACE_DEFINED__
#define __IClassFactory_INTERFACE_DEFINED__
#define INTERFACE IClassFactory
DECLARE_INTERFACE_(IClassFactory,IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(CreateInstance)(THIS_ LPUNKNOWN,REFIID,PVOID*) PURE;
	STDMETHOD(LockServer)(THIS_ BOOL) PURE;
};
#undef INTERFACE
typedef IClassFactory *LPCLASSFACTORY;
#endif

HRESULT STDMETHODCALLTYPE IUnknown_QueryInterface_Proxy(IUnknown*,REFIID,void**);
void __RPC_STUB IUnknown_QueryInterface_Stub(LPRPCSTUBBUFFER,LPRPCCHANNELBUFFER,PRPC_MESSAGE,PDWORD);
ULONG STDMETHODCALLTYPE IUnknown_AddRef_Proxy(IUnknown*);
void __RPC_STUB IUnknown_AddRef_Stub(LPRPCSTUBBUFFER,LPRPCCHANNELBUFFER,PRPC_MESSAGE,PDWORD);
ULONG STDMETHODCALLTYPE IUnknown_Release_Proxy(IUnknown*);
void __RPC_STUB IUnknown_Release_Stub(LPRPCSTUBBUFFER,LPRPCCHANNELBUFFER,PRPC_MESSAGE,PDWORD);
HRESULT STDMETHODCALLTYPE IClassFactory_RemoteCreateInstance_Proxy(IClassFactory*,REFIID,IUnknown**);
void __RPC_STUB IClassFactory_RemoteCreateInstance_Stub(LPRPCSTUBBUFFER,LPRPCCHANNELBUFFER,PRPC_MESSAGE,PDWORD);
HRESULT __stdcall IClassFactory_RemoteLockServer_Proxy(IClassFactory*,BOOL);
void __RPC_STUB IClassFactory_RemoteLockServer_Stub(LPRPCSTUBBUFFER,LPRPCCHANNELBUFFER,PRPC_MESSAGE,PDWORD);
HRESULT STDMETHODCALLTYPE IClassFactory_CreateInstance_Proxy(IClassFactory*,IUnknown*,REFIID,void**);
HRESULT STDMETHODCALLTYPE IClassFactory_CreateInstance_Stub(IClassFactory*,REFIID,IUnknown**);
HRESULT STDMETHODCALLTYPE IClassFactory_LockServer_Proxy(IClassFactory*,BOOL);
HRESULT STDMETHODCALLTYPE IClassFactory_LockServer_Stub(IClassFactory*,BOOL);

#if (!defined (__cplusplus) || defined (CINTERFACE)) \
    && defined (COBJMACROS)
#define IUnknown_QueryInterface(T,r,O) (T)->lpVtbl->QueryInterface(T,r,O)
#define IUnknown_AddRef(T) (T)->lpVtbl->AddRef(T)
#define IUnknown_Release(T) (T)->lpVtbl->Release(T)
#define IClassFactory_QueryInterface(T,r,O) (T)->lpVtbl->QueryInterface(T,r,O)
#define IClassFactory_AddRef(T) (T)->lpVtbl->AddRef(T)
#define IClassFactory_Release(T) (T)->lpVtbl->Release(T)
#define IClassFactory_CreateInstance(T,p,r,O) (T)->lpVtbl->CreateInstance(T,p,r,O)
#define IClassFactory_LockServer(T,f) (T)->lpVtbl->LockServer(T,f)
#endif /* COBJMACROS */

#ifdef __cplusplus
}
#endif

#endif
