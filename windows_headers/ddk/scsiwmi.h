/**
 * @file scsiwmi.h
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
#ifndef __SCSIWMI_H
#define __SCSIWMI_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * SCSI WMILIB interface.
 */

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push,4)

#include "ntddk.h"
#include "srb.h"


typedef struct _SCSIWMI_REQUEST_CONTEXT {
  PVOID  UserContext;
  ULONG  BufferSize;
  PUCHAR  Buffer;
  UCHAR  MinorFunction;
  UCHAR  ReturnStatus;
  ULONG  ReturnSize;
} SCSIWMI_REQUEST_CONTEXT, *PSCSIWMI_REQUEST_CONTEXT;

#ifdef _GUID_DEFINED
# warning _GUID_DEFINED is deprecated, use GUID_DEFINED instead
#endif

#if ! (defined _GUID_DEFINED || defined GUID_DEFINED)
#define GUID_DEFINED
typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} GUID;
#endif

#ifndef _LPCGUID_DEFINED
#define _LPCGUID_DEFINED
typedef const GUID *LPCGUID;
#endif

typedef struct _SCSIWMIGUIDREGINFO {
  LPCGUID  Guid;
  ULONG  InstanceCount;
  ULONG  Flags;
} SCSIWMIGUIDREGINFO, *PSCSIWMIGUIDREGINFO;

typedef UCHAR DDKAPI
(*PSCSIWMI_QUERY_REGINFO)(
  /*IN*/ PVOID  DeviceContext,
  /*IN*/ PSCSIWMI_REQUEST_CONTEXT  RequestContext,
  /*OUT*/ PWCHAR  *MofResourceName);

typedef BOOLEAN DDKAPI
(*PSCSIWMI_QUERY_DATABLOCK)(
  /*IN*/ PVOID  Context,
  /*IN*/ PSCSIWMI_REQUEST_CONTEXT  DispatchContext,
  /*IN*/ ULONG  GuidIndex,
  /*IN*/ ULONG  InstanceIndex,
  /*IN*/ ULONG  InstanceCount,
  /*IN OUT*/ PULONG  InstanceLengthArray,
  /*IN*/ ULONG  BufferAvail,
  /*OUT*/ PUCHAR  Buffer);

typedef BOOLEAN DDKAPI
(*PSCSIWMI_SET_DATABLOCK)(
  /*IN*/ PVOID  DeviceContext,
  /*IN*/ PSCSIWMI_REQUEST_CONTEXT  RequestContext,
  /*IN*/ ULONG  GuidIndex,
  /*IN*/ ULONG  InstanceIndex,
  /*IN*/ ULONG  BufferSize,
  /*IN*/ PUCHAR  Buffer);

typedef BOOLEAN DDKAPI
(*PSCSIWMI_SET_DATAITEM)(
  /*IN*/ PVOID  DeviceContext,
  /*IN*/ PSCSIWMI_REQUEST_CONTEXT  RequestContext,
  /*IN*/ ULONG  GuidIndex,
  /*IN*/ ULONG  InstanceIndex,
  /*IN*/ ULONG  DataItemId,
  /*IN*/ ULONG  BufferSize,
  /*IN*/ PUCHAR  Buffer);

typedef BOOLEAN DDKAPI
(*PSCSIWMI_EXECUTE_METHOD)(
  /*IN*/ PVOID  DeviceContext,
  /*IN*/ PSCSIWMI_REQUEST_CONTEXT  RequestContext,
  /*IN*/ ULONG  GuidIndex,
  /*IN*/ ULONG  InstanceIndex,
  /*IN*/ ULONG  MethodId,
  /*IN*/ ULONG  InBufferSize,
  /*IN*/ ULONG  OutBufferSize,
  /*IN OUT*/ PUCHAR  Buffer);

typedef enum _SCSIWMI_ENABLE_DISABLE_CONTROL {
	ScsiWmiEventControl,
	ScsiWmiDataBlockControl
} SCSIWMI_ENABLE_DISABLE_CONTROL;

typedef BOOLEAN DDKAPI
(*PSCSIWMI_FUNCTION_CONTROL)(
  /*IN*/ PVOID  DeviceContext,
  /*IN*/ PSCSIWMI_REQUEST_CONTEXT  RequestContext,
  /*IN*/ ULONG  GuidIndex,
  /*IN*/ SCSIWMI_ENABLE_DISABLE_CONTROL  Function,
  /*IN*/ BOOLEAN  Enable);

typedef struct _SCSIWMILIB_CONTEXT {
  ULONG  GuidCount;
  PSCSIWMIGUIDREGINFO  GuidList;
  PSCSIWMI_QUERY_REGINFO  QueryWmiRegInfo;
  PSCSIWMI_QUERY_DATABLOCK  QueryWmiDataBlock;
  PSCSIWMI_SET_DATABLOCK  SetWmiDataBlock;
  PSCSIWMI_SET_DATAITEM  SetWmiDataItem;
  PSCSIWMI_EXECUTE_METHOD  ExecuteWmiMethod;
  PSCSIWMI_FUNCTION_CONTROL  WmiFunctionControl;
} SCSI_WMILIB_CONTEXT, *PSCSI_WMILIB_CONTEXT;

SCSIPORTAPI
BOOLEAN
DDKAPI
ScsiPortWmiDispatchFunction(
  /*IN*/ PSCSI_WMILIB_CONTEXT  WmiLibInfo,
  /*IN*/ UCHAR  MinorFunction,
  /*IN*/ PVOID  DeviceContext,
  /*IN*/ PSCSIWMI_REQUEST_CONTEXT  RequestContext,
  /*IN*/ PVOID  DataPath,
  /*IN*/ ULONG  BufferSize,
  /*IN*/ PVOID  Buffer);

#define ScsiPortWmiFireAdapterEvent(  \
  HwDeviceExtension,                  \
  Guid,                               \
  InstanceIndex,                      \
  EventDataSize,                      \
  EventData)                          \
    ScsiPortWmiFireLogicalUnitEvent(  \
	  HwDeviceExtension,                \
	  0xff,                             \
	  0,                                \
	  0,                                \
	  Guid,                             \
	  InstanceIndex,                    \
	  EventDataSize,                    \
	  EventData)

/*
 * ULONG
 * ScsiPortWmiGetReturnSize(
 *   PSCSIWMI_REQUEST_CONTEXT  RequestContext);
 */
#define ScsiPortWmiGetReturnSize(RequestContext) \
  ((RequestContext)->ReturnSize)

/* UCHAR 
 * ScsiPortWmiGetReturnStatus(
 *   PSCSIWMI_REQUEST_CONTEXT  RequestContext);
 */ 
#define ScsiPortWmiGetReturnStatus(RequestContext) \
  ((RequestContext)->ReturnStatus)

SCSIPORTAPI
VOID
DDKAPI
ScsiPortWmiPostProcess(
  /*IN*/ PSCSIWMI_REQUEST_CONTEXT  RequestContext,
  /*IN*/ UCHAR  SrbStatus,
  /*IN*/ ULONG  BufferUsed);

SCSIPORTAPI
VOID
DDKAPI
ScsiPortWmiFireLogicalUnitEvent(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ UCHAR  PathId,
  /*IN*/ UCHAR  TargetId,
  /*IN*/ UCHAR  Lun,
  /*IN*/ LPGUID  Guid,
  /*IN*/ ULONG  InstanceIndex,
  /*IN*/ ULONG  EventDataSize,
  /*IN*/ PVOID  EventData);

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __SCSIWMI_H */
