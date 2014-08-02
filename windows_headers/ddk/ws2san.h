/**
 * @file ws2san.h
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
/* Created by Casper S. Hornstrup <chorns@users.sourceforge.net> */
#ifndef __WS2SAN_H
#define __WS2SAN_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * WinSock Direct (SAN) support
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <winsock2.h>
#include "ntddk.h"


#define WSPAPI STDCALL

/* FIXME: Unknown definitions */
typedef PVOID LPWSPDATA;
typedef PDWORD LPWSATHREADID;
typedef PVOID LPWSPPROC_TABLE;
typedef struct _WSPUPCALLTABLEEX WSPUPCALLTABLEEX;
typedef WSPUPCALLTABLEEX *LPWSPUPCALLTABLEEX;

#define SO_MAX_RDMA_SIZE                  0x700D
#define SO_RDMA_THRESHOLD_SIZE            0x700E

#define WSAID_REGISTERMEMORY \
  {0xC0B422F5, 0xF58C, 0x11d1, {0xAD, 0x6C, 0x00, 0xC0, 0x4F, 0xA3, 0x4A, 0x2D}}

#define WSAID_DEREGISTERMEMORY \
  {0xC0B422F6, 0xF58C, 0x11d1, {0xAD, 0x6C, 0x00, 0xC0, 0x4F, 0xA3, 0x4A, 0x2D}}

#define WSAID_REGISTERRDMAMEMORY \
  {0xC0B422F7, 0xF58C, 0x11d1, {0xAD, 0x6C, 0x00, 0xC0, 0x4F, 0xA3, 0x4A, 0x2D}}

#define WSAID_DEREGISTERRDMAMEMORY \
  {0xC0B422F8, 0xF58C, 0x11d1, {0xAD, 0x6C, 0x00, 0xC0, 0x4F, 0xA3, 0x4A, 0x2D}}

#define WSAID_RDMAWRITE \
  {0xC0B422F9, 0xF58C, 0x11d1, {0xAD, 0x6C, 0x00, 0xC0, 0x4F, 0xA3, 0x4A, 0x2D}}

#define WSAID_RDMAREAD \
  {0xC0B422FA, 0xF58C, 0x11d1, {0xAD, 0x6C, 0x00, 0xC0, 0x4F, 0xA3, 0x4A, 0x2D}}

#define WSAID_MEMORYREGISTRATIONCACHECALLBACK \
  {0xE5DA4AF8, 0xD824, 0x48CD, {0xA7, 0x99, 0x63, 0x37, 0xA9, 0x8E, 0xD2, 0xAF}}

typedef struct _WSABUFEX {
  u_long  len; 
  char FAR  *buf; 
  HANDLE  handle; 
} WSABUFEX, FAR * LPWSABUFEX;

#if 0
typedef struct _WSPUPCALLTABLEEX {
  LPWPUCLOSEEVENT  lpWPUCloseEvent;
  LPWPUCLOSESOCKETHANDLE  lpWPUCloseSocketHandle;
  LPWPUCREATEEVENT  lpWPUCreateEvent;
  LPWPUCREATESOCKETHANDLE  lpWPUCreateSocketHandle;
  LPWPUFDISSET  lpWPUFDIsSet;
  LPWPUGETPROVIDERPATH  lpWPUGetProviderPath;
  LPWPUMODIFYIFSHANDLE  lpWPUModifyIFSHandle;
  LPWPUPOSTMESSAGE  lpWPUPostMessage;
  LPWPUQUERYBLOCKINGCALLBACK  lpWPUQueryBlockingCallback;
  LPWPUQUERYSOCKETHANDLECONTEXT  lpWPUQuerySocketHandleContext;
  LPWPUQUEUEAPC  lpWPUQueueApc;
  LPWPURESETEVENT  lpWPUResetEvent;
  LPWPUSETEVENT  lpWPUSetEvent;
  LPWPUOPENCURRENTTHREAD  lpWPUOpenCurrentThread;
  LPWPUCLOSETHREAD  lpWPUCloseThread;
  LPWPUCOMPLETEOVERLAPPEDREQUEST lpWPUCompleteOverlappedRequest;
} WSPUPCALLTABLEEX, FAR * LPWSPUPCALLTABLEEX;
#endif

int WSPAPI
WSPStartupEx(
  /*IN*/ WORD  wVersionRequested,
  /*OUT*/ LPWSPDATA  lpWSPData,
  /*IN*/ LPWSAPROTOCOL_INFOW  lpProtocolInfo,
  /*IN*/ LPWSPUPCALLTABLEEX  lpUpcallTable,
  /*OUT*/ LPWSPPROC_TABLE  lpProcTable);

typedef int WSPAPI
(*LPWSPSTARTUPEX)(
  /*IN*/ WORD  wVersionRequested,
  /*OUT*/ LPWSPDATA  lpWSPData,
  /*IN*/ LPWSAPROTOCOL_INFOW  lpProtocolInfo,
  /*IN*/ LPWSPUPCALLTABLEEX  lpUpcallTable,
  /*OUT*/ LPWSPPROC_TABLE  lpProcTable);

#define MEM_READ                          1
#define MEM_WRITE                         2
#define MEM_READWRITE                     3

int WSPAPI
WSPDeregisterMemory(
  /*IN*/ SOCKET  s,
  /*IN*/ HANDLE  Handle,
  /*OUT*/ LPINT  lpErrno);

typedef int WSPAPI
(*LPFN_WSPDEREGISTERMEMORY)(
  /*IN*/ SOCKET  s,
  /*IN*/ HANDLE  Handle,
  /*OUT*/ LPINT  lpErrno);

int WSPAPI
WSPDeregisterRdmaMemory(
  /*IN*/ SOCKET  s,
  /*IN*/ LPVOID  lpRdmaBufferDescriptor,
  /*IN*/ DWORD  dwDescriptorLength,
  /*OUT*/ LPINT  lpErrno);

typedef int WSPAPI
(*LPFN_WSPDEREGISTERRDMAMEMORY)(
  /*IN*/ SOCKET  s,
  /*IN*/ LPVOID  lpRdmaBufferDescriptor,
  /*IN*/ DWORD  dwDescriptorLength,
  /*OUT*/ LPINT  lpErrno);

int WSPAPI
WSPMemoryRegistrationCacheCallback(
  /*IN*/ PVOID  lpvAddress,
  /*IN*/ SIZE_T  Size,
  /*OUT*/ LPINT  lpErrno);

typedef int WSPAPI
(*LPFN_WSPMEMORYREGISTRATIONCACHECALLBACK)(
  /*IN*/ PVOID  lpvAddress,
  /*IN*/ SIZE_T  Size,
  /*OUT*/ LPINT  lpErrno);

int WSPAPI
WSPRdmaRead(
  /*IN*/ SOCKET  s,
  /*IN*/ LPWSABUFEX  lpBuffers,
  /*IN*/ DWORD  dwBufferCount,
  /*IN*/ LPVOID  lpTargetBufferDescriptor,
  /*IN*/ DWORD  dwTargetDescriptorLength,
  /*IN*/ DWORD  dwTargetBufferOffset,
  /*OUT*/ LPDWORD  lpdwNumberOfBytesRead,
  /*IN*/ DWORD  dwFlags,
  /*IN*/ LPWSAOVERLAPPED  lpOverlapped,
  /*IN*/ LPWSAOVERLAPPED_COMPLETION_ROUTINE  lpCompletionRoutine,
  /*IN*/ LPWSATHREADID  lpThreadId,
  /*OUT*/ LPINT  lpErrno);

typedef int WSPAPI
(*LPFN_WSPRDMAREAD)(
  /*IN*/ SOCKET  s,
  /*IN*/ LPWSABUFEX  lpBuffers,
  /*IN*/ DWORD  dwBufferCount,
  /*IN*/ LPVOID  lpTargetBufferDescriptor,
  /*IN*/ DWORD  dwTargetDescriptorLength,
  /*IN*/ DWORD  dwTargetBufferOffset,
  /*OUT*/ LPDWORD  lpdwNumberOfBytesRead,
  /*IN*/ DWORD  dwFlags,
  /*IN*/ LPWSAOVERLAPPED  lpOverlapped,
  /*IN*/ LPWSAOVERLAPPED_COMPLETION_ROUTINE  lpCompletionRoutine,
  /*IN*/ LPWSATHREADID  lpThreadId,
  /*OUT*/ LPINT  lpErrno);

int WSPAPI
WSPRdmaWrite(
  /*IN*/ SOCKET  s,
  /*IN*/ LPWSABUFEX  lpBuffers,
  /*IN*/ DWORD  dwBufferCount,
  /*IN*/ LPVOID  lpTargetBufferDescriptor,
  /*IN*/ DWORD  dwTargetDescriptorLength,
  /*IN*/ DWORD  dwTargetBufferOffset,
  /*OUT*/ LPDWORD  lpdwNumberOfBytesWritten,
  /*IN*/ DWORD  dwFlags,
  /*IN*/ LPWSAOVERLAPPED  lpOverlapped,
  /*IN*/ LPWSAOVERLAPPED_COMPLETION_ROUTINE  lpCompletionRoutine,
  /*IN*/ LPWSATHREADID  lpThreadId,
  /*OUT*/ LPINT  lpErrno);

typedef int WSPAPI
(*LPFN_WSPRDMAWRITE)(
  /*IN*/ SOCKET  s,
  /*IN*/ LPWSABUFEX  lpBuffers,
  /*IN*/ DWORD  dwBufferCount,
  /*IN*/ LPVOID  lpTargetBufferDescriptor,
  /*IN*/ DWORD  dwTargetDescriptorLength,
  /*IN*/ DWORD  dwTargetBufferOffset,
  /*OUT*/ LPDWORD  lpdwNumberOfBytesWritten,
  /*IN*/ DWORD  dwFlags,
  /*IN*/ LPWSAOVERLAPPED  lpOverlapped,
  /*IN*/ LPWSAOVERLAPPED_COMPLETION_ROUTINE  lpCompletionRoutine,
  /*IN*/ LPWSATHREADID  lpThreadId,
  /*OUT*/ LPINT  lpErrno);

HANDLE WSPAPI
WSPRegisterMemory(
  /*IN*/ SOCKET  s,
  /*IN*/ PVOID  lpBuffer,
  /*IN*/ DWORD  dwBufferLength,
  /*IN*/ DWORD  dwFlags,
  /*OUT*/ LPINT  lpErrno);

int WSPAPI
WSPRegisterRdmaMemory(
  /*IN*/ SOCKET  s,
  /*IN*/ PVOID  lpBuffer,
  /*IN*/ DWORD  dwBufferLength,
  /*IN*/ DWORD  dwFlags,
  /*OUT*/ LPVOID  lpRdmaBufferDescriptor,
  /*IN OUT*/ LPDWORD  lpdwDescriptorLength,
  /*OUT*/ LPINT  lpErrno);

typedef int WSPAPI
(*LPFN_WSPREGISTERRDMAMEMORY)(
  /*IN*/ SOCKET  s,
  /*IN*/ PVOID  lpBuffer,
  /*IN*/ DWORD  dwBufferLength,
  /*IN*/ DWORD  dwFlags,
  /*OUT*/ LPVOID  lpRdmaBufferDescriptor,
  /*IN OUT*/ LPDWORD  lpdwDescriptorLength,
  /*OUT*/ LPINT  lpErrno);

#ifdef __cplusplus
}
#endif

#endif /* __WS2SAN_H */
