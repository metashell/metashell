/**
 * @file rpcnsi.h
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
#ifndef _RPCNSI_H
#define _RPCNSI_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *RPC_NS_HANDLE;
#define RPC_C_NS_SYNTAX_DEFAULT 0
#define RPC_C_NS_SYNTAX_DCE 3
#define RPC_C_PROFILE_DEFAULT_ELT 0
#define RPC_C_PROFILE_ALL_ELT 1
#define RPC_C_PROFILE_MATCH_BY_IF 2
#define RPC_C_PROFILE_MATCH_BY_MBR 3
#define RPC_C_PROFILE_MATCH_BY_BOTH 4
#define RPC_C_NS_DEFAULT_EXP_AGE -1

RPC_STATUS RPC_ENTRY RpcNsBindingExportA(unsigned long,unsigned char*,RPC_IF_HANDLE,RPC_BINDING_VECTOR*,UUID_VECTOR*);
RPC_STATUS RPC_ENTRY RpcNsBindingUnexportA(unsigned long,unsigned char*,RPC_IF_HANDLE,UUID_VECTOR*);
RPC_STATUS RPC_ENTRY RpcNsBindingLookupBeginA(unsigned long,unsigned char*,RPC_IF_HANDLE,UUID*,unsigned long,RPC_NS_HANDLE*);
RPC_STATUS RPC_ENTRY RpcNsBindingLookupNext(RPC_NS_HANDLE,RPC_BINDING_VECTOR**);
RPC_STATUS RPC_ENTRY RpcNsBindingLookupDone(RPC_NS_HANDLE*);
RPC_STATUS RPC_ENTRY RpcNsGroupDeleteA(unsigned long,unsigned char*);
RPC_STATUS RPC_ENTRY RpcNsGroupMbrAddA(unsigned long,unsigned char*,unsigned long,unsigned char*);
RPC_STATUS RPC_ENTRY RpcNsGroupMbrRemoveA(unsigned long,unsigned char*,unsigned long,unsigned char*);
RPC_STATUS RPC_ENTRY RpcNsGroupMbrInqBeginA(unsigned long,unsigned char*,unsigned long,RPC_NS_HANDLE*);
RPC_STATUS RPC_ENTRY RpcNsGroupMbrInqNextA(RPC_NS_HANDLE,unsigned char**);
RPC_STATUS RPC_ENTRY RpcNsGroupMbrInqDone(RPC_NS_HANDLE*);
RPC_STATUS RPC_ENTRY RpcNsProfileDeleteA(unsigned long,unsigned char*);
RPC_STATUS RPC_ENTRY RpcNsProfileEltAddA(unsigned long,unsigned char*,RPC_IF_ID*,unsigned long,unsigned char*,unsigned long,unsigned char*);
RPC_STATUS RPC_ENTRY RpcNsProfileEltRemoveA(unsigned long,unsigned char*,RPC_IF_ID*,unsigned long,unsigned char*);
RPC_STATUS RPC_ENTRY RpcNsProfileEltInqBeginA(unsigned long,unsigned char*,unsigned long,RPC_IF_ID*,unsigned long,unsigned long,unsigned char*,RPC_NS_HANDLE*);
RPC_STATUS RPC_ENTRY RpcNsProfileEltInqNextA(RPC_NS_HANDLE,RPC_IF_ID*,unsigned char**,unsigned long*,unsigned char**);
RPC_STATUS RPC_ENTRY RpcNsProfileEltInqDone(RPC_NS_HANDLE*);
RPC_STATUS RPC_ENTRY RpcNsEntryObjectInqNext(/*IN*/ RPC_NS_HANDLE,/*OUT*/ UUID*);
RPC_STATUS RPC_ENTRY RpcNsEntryObjectInqDone(/*IN OUT*/ RPC_NS_HANDLE*);
RPC_STATUS RPC_ENTRY RpcNsEntryExpandNameA(unsigned long,unsigned char*,unsigned char**);
RPC_STATUS RPC_ENTRY RpcNsMgmtBindingUnexportA(unsigned long,unsigned char*,RPC_IF_ID*,unsigned long,UUID_VECTOR*);
RPC_STATUS RPC_ENTRY RpcNsMgmtEntryCreateA(unsigned long,unsigned char*);
RPC_STATUS RPC_ENTRY RpcNsMgmtEntryDeleteA(unsigned long,unsigned char*);
RPC_STATUS RPC_ENTRY RpcNsMgmtEntryInqIfIdsA(unsigned long,unsigned char*,RPC_IF_ID_VECTOR**);
RPC_STATUS RPC_ENTRY RpcNsMgmtHandleSetExpAge(RPC_NS_HANDLE,unsigned long);
RPC_STATUS RPC_ENTRY RpcNsMgmtInqExpAge(unsigned long*);
RPC_STATUS RPC_ENTRY RpcNsMgmtSetExpAge(unsigned long);
RPC_STATUS RPC_ENTRY RpcNsBindingImportNext(RPC_NS_HANDLE,RPC_BINDING_HANDLE*);
RPC_STATUS RPC_ENTRY RpcNsBindingImportDone(RPC_NS_HANDLE*);
RPC_STATUS RPC_ENTRY RpcNsBindingSelect(RPC_BINDING_VECTOR*,RPC_BINDING_HANDLE*);
#ifndef UNICODE_ONLY
RPC_STATUS RPC_ENTRY RpcNsEntryObjectInqBeginA(unsigned long,unsigned char*,RPC_NS_HANDLE*);
RPC_STATUS RPC_ENTRY RpcNsBindingImportBeginA(unsigned long,unsigned char*,RPC_IF_HANDLE,UUID*,RPC_NS_HANDLE*);
#endif
#ifdef RPC_UNICODE_SUPPORTED
RPC_STATUS RPC_ENTRY RpcNsBindingExportW(unsigned long,unsigned short*,RPC_IF_HANDLE,RPC_BINDING_VECTOR*,UUID_VECTOR*);
RPC_STATUS RPC_ENTRY RpcNsBindingUnexportW(unsigned long,unsigned short*,RPC_IF_HANDLE,UUID_VECTOR*);
RPC_STATUS RPC_ENTRY RpcNsBindingLookupBeginW(unsigned long,unsigned short*,RPC_IF_HANDLE,UUID*,unsigned long,RPC_NS_HANDLE*);
RPC_STATUS RPC_ENTRY RpcNsGroupDeleteW(unsigned long,unsigned short*);
RPC_STATUS RPC_ENTRY RpcNsGroupMbrAddW(unsigned long,unsigned short*,unsigned long,unsigned short*);
RPC_STATUS RPC_ENTRY RpcNsGroupMbrRemoveW(unsigned long,unsigned short*,unsigned long,unsigned short*);
RPC_STATUS RPC_ENTRY RpcNsGroupMbrInqBeginW(unsigned long,unsigned short*,unsigned long,RPC_NS_HANDLE*);
RPC_STATUS RPC_ENTRY RpcNsGroupMbrInqNextW(RPC_NS_HANDLE,unsigned short**);
RPC_STATUS RPC_ENTRY RpcNsProfileDeleteW(unsigned long,unsigned short*);
RPC_STATUS RPC_ENTRY RpcNsProfileEltAddW(unsigned long,unsigned short*, RPC_IF_ID*,unsigned long,unsigned short*,unsigned long,unsigned short*);
RPC_STATUS RPC_ENTRY RpcNsProfileEltRemoveW(unsigned long,unsigned short*, RPC_IF_ID*,unsigned long,unsigned short*);
RPC_STATUS RPC_ENTRY RpcNsProfileEltInqBeginW(unsigned long,unsigned short*, unsigned long,RPC_IF_ID*,unsigned long,unsigned long,unsigned short*, RPC_NS_HANDLE*);
RPC_STATUS RPC_ENTRY RpcNsProfileEltInqNextW(RPC_NS_HANDLE,RPC_IF_ID*, unsigned short**,unsigned long*,unsigned short**);
RPC_STATUS RPC_ENTRY RpcNsEntryObjectInqBeginW(unsigned long,unsigned short*,RPC_NS_HANDLE*);
RPC_STATUS RPC_ENTRY RpcNsEntryExpandNameW(unsigned long,unsigned short*,unsigned short**);
RPC_STATUS RPC_ENTRY RpcNsMgmtBindingUnexportW(unsigned long,unsigned short*,RPC_IF_ID*,unsigned long,UUID_VECTOR*);
RPC_STATUS RPC_ENTRY RpcNsMgmtEntryCreateW(unsigned long,unsigned short*);
RPC_STATUS RPC_ENTRY RpcNsMgmtEntryDeleteW(unsigned long,unsigned short*);
RPC_STATUS RPC_ENTRY RpcNsMgmtEntryInqIfIdsW(unsigned long,unsigned short , RPC_IF_ID_VECTOR**);
RPC_STATUS RPC_ENTRY RpcNsBindingImportBeginW(unsigned long,unsigned short*,RPC_IF_HANDLE,UUID*,RPC_NS_HANDLE*);
#endif /* RPC_UNICODE_SUPPORTED */

#define RpcNsBindingLookupBegin __AW(RpcNsBindingLookupBegin)
#define RpcNsBindingImportBegin __AW(RpcNsBindingImportBegin)
#define RpcNsBindingExport __AW(RpcNsBindingExport)
#define RpcNsBindingUnexport __AW(RpcNsBindingUnexport)
#define RpcNsGroupDelete __AW(RpcNsGroupDelete)
#define RpcNsGroupMbrAdd __AW(RpcNsGroupMbrAdd)
#define RpcNsGroupMbrRemove __AW(RpcNsGroupMbrRemove)
#define RpcNsGroupMbrInqBegin __AW(RpcNsGroupMbrInqBegin)
#define RpcNsGroupMbrInqNext __AW(RpcNsGroupMbrInqNext)
#define RpcNsEntryExpandName __AW(RpcNsEntryExpandName)
#define RpcNsEntryObjectInqBegin __AW(RpcNsEntryObjectInqBegin)
#define RpcNsMgmtBindingUnexport __AW(RpcNsMgmtBindingUnexport)
#define RpcNsMgmtEntryCreate __AW(RpcNsMgmtEntryCreate)
#define RpcNsMgmtEntryDelete __AW(RpcNsMgmtEntryDelete)
#define RpcNsMgmtEntryInqIfIds __AW(RpcNsMgmtEntryInqIfIds)
#define RpcNsProfileDelete __AW(RpcNsProfileDelete)
#define RpcNsProfileEltAdd __AW(RpcNsProfileEltAdd)
#define RpcNsProfileEltRemove __AW(RpcNsProfileEltRemove)
#define RpcNsProfileEltInqBegin __AW(RpcNsProfileEltInqBegin)
#define RpcNsProfileEltInqNext __AW(RpcNsProfileEltInqNext)

#ifdef __cplusplus
}
#endif

#endif
