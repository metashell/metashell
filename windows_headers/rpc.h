/**
 * @file rpc.h
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
/* Include here to prevent circular dependencies if windows.h not already
 * included. ** DO NOT MOVE TO GUARD ** */
#ifndef RPC_NO_WINDOWS_H
#include <windows.h>
#endif

#ifndef _RPC_H
#define _RPC_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#define __RPC_WIN32__
#ifndef _WIN95
#define __RPC_NT__
#define RPC_UNICODE_SUPPORTED
#endif

#ifdef _RPCRT4_
#define RPCRTAPI DECLSPEC_EXPORT
#else
#define RPCRTAPI DECLSPEC_IMPORT
#endif

#ifndef __MIDL_USER_DEFINED
#define midl_user_allocate MIDL_user_allocate
#define midl_user_free     MIDL_user_free
#define __MIDL_USER_DEFINED
#endif
#define RPC_UNICODE_SUPPORTED
#define __RPC_FAR
#define __RPC_API  __stdcall
#define __RPC_USER __stdcall
#define __RPC_STUB __stdcall
#define RPC_ENTRY  __stdcall
typedef void *I_RPC_HANDLE;
typedef long RPC_STATUS;

#include <rpcdce.h>
#include <rpcnsi.h>
#include <rpcnterr.h>

#include <winerror.h>

/* SEH is not supported */
#if 0
#include <excpt.h>
#define RpcTryExcept __try {
#define RpcExcept(x) } __except (x) {
#define RpcEndExcept }
#define RpcTryFinally __try {
#define RpcFinally } __finally {
#define RpcEndFinally }
#define RpcExceptionCode() GetExceptionCode()
#define RpcAbnormalTermination() AbnormalTermination()
#endif /* 0 */

RPC_STATUS RPC_ENTRY RpcImpersonateClient(RPC_BINDING_HANDLE);
RPC_STATUS RPC_ENTRY RpcRevertToSelf(void);
long RPC_ENTRY I_RpcMapWin32Status(RPC_STATUS);

#ifdef __cplusplus
}
#endif

#endif
