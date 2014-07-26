/**
 * @file mcd.h
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
#ifndef __MCD_H
#define __MCD_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Media changer driver interface
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"
#include "srb.h"
#include "scsi.h"
#include "ntddchgr.h"

#if defined(_MCD_)
  #define CHANGERAPI DECLSPEC_EXPORT
#else
  #define CHANGERAPI DECLSPEC_IMPORT
#endif

#ifdef DebugPrint
#undef DebugPrint
#endif

#ifdef DBG
#define DebugPrint(x) ChangerClassDebugPrint x
#else
#define DebugPrint(x)
#endif

#define MAXIMUM_CHANGER_INQUIRY_DATA			252

CHANGERAPI
PVOID
DDKAPI
ChangerClassAllocatePool(
  /*IN*/ POOL_TYPE  PoolType,
  /*IN*/ ULONG  NumberOfBytes);

VOID
ChangerClassDebugPrint(
  ULONG  DebugPrintLevel,
  PCCHAR  DebugMessage,
  ...);

CHANGERAPI
PVOID
DDKAPI
ChangerClassFreePool(
  /*IN*/ PVOID  PoolToFree);

CHANGERAPI
NTSTATUS
DDKAPI
ChangerClassSendSrbSynchronous(
  /*IN*/ PDEVICE_OBJECT  DeviceObject,
  /*IN*/ PSCSI_REQUEST_BLOCK  Srb,
  /*IN*/ PVOID  Buffer,
  /*IN*/ ULONG  BufferSize,
  /*IN*/ BOOLEAN  WriteToDevice);


typedef NTSTATUS DDKAPI
(*CHANGER_INITIALIZE)(
  /*IN*/ PDEVICE_OBJECT  DeviceObject);

typedef ULONG DDKAPI
(*CHANGER_EXTENSION_SIZE)(
  VOID);

typedef VOID DDKAPI
(*CHANGER_ERROR_ROUTINE)(
  PDEVICE_OBJECT  DeviceObject,
  PSCSI_REQUEST_BLOCK  Srb,
  NTSTATUS  *Status,
  BOOLEAN  *Retry);

typedef NTSTATUS DDKAPI
(*CHANGER_COMMAND_ROUTINE)(
  /*IN*/ PDEVICE_OBJECT  DeviceObject,
  /*IN*/ PIRP  Irp);

typedef NTSTATUS DDKAPI
(*CHANGER_PERFORM_DIAGNOSTICS)(
  /*IN*/ PDEVICE_OBJECT  DeviceObject,
  /*OUT*/ PWMI_CHANGER_PROBLEM_DEVICE_ERROR  ChangerDeviceError);

typedef struct _MCD_INIT_DATA {
  ULONG  InitDataSize;
  CHANGER_EXTENSION_SIZE  ChangerAdditionalExtensionSize;
  CHANGER_INITIALIZE  ChangerInitialize;
  CHANGER_ERROR_ROUTINE  ChangerError;
  CHANGER_PERFORM_DIAGNOSTICS  ChangerPerformDiagnostics;
  CHANGER_COMMAND_ROUTINE  ChangerGetParameters;
  CHANGER_COMMAND_ROUTINE  ChangerGetStatus;
  CHANGER_COMMAND_ROUTINE  ChangerGetProductData;
  CHANGER_COMMAND_ROUTINE  ChangerSetAccess;
  CHANGER_COMMAND_ROUTINE  ChangerGetElementStatus;
  CHANGER_COMMAND_ROUTINE  ChangerInitializeElementStatus;
  CHANGER_COMMAND_ROUTINE  ChangerSetPosition;
  CHANGER_COMMAND_ROUTINE  ChangerExchangeMedium;
  CHANGER_COMMAND_ROUTINE  ChangerMoveMedium;
  CHANGER_COMMAND_ROUTINE  ChangerReinitializeUnit;
  CHANGER_COMMAND_ROUTINE  ChangerQueryVolumeTags;
} MCD_INIT_DATA, *PMCD_INIT_DATA;

CHANGERAPI
NTSTATUS
DDKAPI
ChangerClassInitialize(
  /*IN*/ PDRIVER_OBJECT  DriverObject,
  /*IN*/ PUNICODE_STRING  RegistryPath,
  /*IN*/ PMCD_INIT_DATA  MCDInitData);

#ifdef __cplusplus
}
#endif

#endif /* __MCD_H */
