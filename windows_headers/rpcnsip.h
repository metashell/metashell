/**
 * @file rpcnsip.h
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
#ifndef _RPCNSIP_H
#define _RPCNSIP_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	RPC_NS_HANDLE LookupContext;
	RPC_BINDING_HANDLE ProposedHandle;
	RPC_BINDING_VECTOR *Bindings;
} RPC_IMPORT_CONTEXT_P,*PRPC_IMPORT_CONTEXT_P;
RPC_STATUS RPC_ENTRY I_RpcNsGetBuffer(/*IN*/ PRPC_MESSAGE);
RPC_STATUS RPC_ENTRY I_RpcNsSendReceive(/*IN*/ PRPC_MESSAGE,/*OUT*/ RPC_BINDING_HANDLE*);
void RPC_ENTRY I_RpcNsRaiseException(/*IN*/ PRPC_MESSAGE,/*IN*/ RPC_STATUS);
RPC_STATUS RPC_ENTRY I_RpcReBindBuffer(/*IN*/ PRPC_MESSAGE);
RPC_STATUS RPC_ENTRY I_NsServerBindSearch(void);
RPC_STATUS RPC_ENTRY I_NsClientBindSearch(void);
void RPC_ENTRY I_NsClientBindDone(void);

#ifdef __cplusplus
}
#endif

#endif
