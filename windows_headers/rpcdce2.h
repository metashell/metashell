/**
 * @file rpcdce2.h
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
#ifndef _RPCDCE2_H
#define _RPCDCE2_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <rpcdce.h>

#define RPC_C_EP_ALL_ELTS 0
#define RPC_C_EP_MATCH_BY_IF 1
#define RPC_C_EP_MATCH_BY_OBJ 2
#define RPC_C_EP_MATCH_BY_BOTH 3
#define RPC_C_VERS_ALL 1
#define RPC_C_VERS_COMPATIBLE 2
#define RPC_C_VERS_EXACT 3
#define RPC_C_VERS_MAJOR_ONLY 4
#define RPC_C_VERS_UPTO 5
#define DCE_C_ERROR_STRING_LEN 256
#define RPC_C_MGMT_INQ_IF_IDS 0
#define RPC_C_MGMT_INQ_PRINC_NAME 1
#define RPC_C_MGMT_INQ_STATS 2
#define RPC_C_MGMT_IS_SERVER_LISTEN 3
#define RPC_C_MGMT_STOP_SERVER_LISTEN 4

int RPC_ENTRY UuidCompare(UUID*,UUID*,RPC_STATUS*);
RPC_STATUS RPC_ENTRY UuidCreateNil(UUID*);
int RPC_ENTRY UuidEqual(UUID*,UUID*,RPC_STATUS*);
unsigned short RPC_ENTRY UuidHash(UUID*,RPC_STATUS*);
int RPC_ENTRY UuidIsNil(UUID*,RPC_STATUS*);

#ifdef RPC_UNICODE_SUPPORTED
RPC_STATUS RPC_ENTRY DceErrorInqTextA(RPC_STATUS,unsigned char*);
RPC_STATUS RPC_ENTRY DceErrorInqTextW(RPC_STATUS,unsigned short*);
RPC_STATUS RPC_ENTRY RpcMgmtEpEltInqNextA(RPC_EP_INQ_HANDLE,RPC_IF_ID*,RPC_BINDING_HANDLE*,UUID*,unsigned char**);
RPC_STATUS RPC_ENTRY RpcMgmtEpEltInqNextW(RPC_EP_INQ_HANDLE,RPC_IF_ID*,RPC_BINDING_HANDLE*,UUID*,unsigned short**);

#define RpcMgmtEpEltInqNext __AW(RpcMgmtEpEltInqNext)
#define DceErrorInqText __AW(DceErrorInqText)

#else /* RPC_UNICODE_SUPPORTED */
RPC_STATUS RPC_ENTRY DceErrorInqText(RPC_STATUS,unsigned char*);
RPC_STATUS RPC_ENTRY RpcMgmtEpEltInqNext(RPC_EP_INQ_HANDLE,RPC_IF_ID*,RPC_BINDING_HANDLE*,UUID*,unsigned char**);
#endif

RPC_STATUS RPC_ENTRY RpcMgmtEpEltInqBegin(RPC_BINDING_HANDLE,unsigned long,RPC_IF_ID*,unsigned long,UUID*,RPC_EP_INQ_HANDLE*);
RPC_STATUS RPC_ENTRY RpcMgmtEpEltInqDone(RPC_EP_INQ_HANDLE*);
RPC_STATUS RPC_ENTRY RpcMgmtEpUnregister(RPC_BINDING_HANDLE,RPC_IF_ID*,RPC_BINDING_HANDLE,UUID*);
RPC_STATUS RPC_ENTRY RpcMgmtSetAuthorizationFn(RPC_MGMT_AUTHORIZATION_FN);

#ifdef __cplusplus
}
#endif

#endif
