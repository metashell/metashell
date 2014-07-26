/**
 * @file storport.h
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
#ifndef __STORPORT_H
#define __STORPORT_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"
#include "srb.h"

#if defined(_STORPORT_)
  #define STORPORTAPI DECLSPEC_EXPORT
#else
  #define STORPORTAPI DECLSPEC_IMPORT
#endif


typedef PHYSICAL_ADDRESS STOR_PHYSICAL_ADDRESS;

typedef struct _STOR_SCATTER_GATHER_ELEMENT {
	STOR_PHYSICAL_ADDRESS  PhysicalAddress;
	ULONG  Length;
	ULONG_PTR  Reserved;
} STOR_SCATTER_GATHER_ELEMENT, *PSTOR_SCATTER_GATHER_ELEMENT;

typedef struct _STOR_SCATTER_GATHER_LIST {
    ULONG  NumberOfElements;
    ULONG_PTR  Reserved;
    STOR_SCATTER_GATHER_ELEMENT  List[0];
} STOR_SCATTER_GATHER_LIST, *PSTOR_SCATTER_GATHER_LIST;

typedef struct _SCSI_WMI_REQUEST_BLOCK {
  USHORT  Length;
  UCHAR  Function;
  UCHAR  SrbStatus;
  UCHAR  WMISubFunction;
  UCHAR  PathId;
  UCHAR  TargetId;
  UCHAR  Lun;
  UCHAR  Reserved1;
  UCHAR  WMIFlags;
  UCHAR  Reserved2[2];
  ULONG  SrbFlags;
  ULONG  DataTransferLength;
  ULONG  TimeOutValue;
  PVOID  DataBuffer;
  PVOID  DataPath;
  PVOID  Reserved3;
  PVOID  OriginalRequest;
  PVOID  SrbExtension;
  ULONG  Reserved4;
  UCHAR  Reserved5[16];
} SCSI_WMI_REQUEST_BLOCK, *PSCSI_WMI_REQUEST_BLOCK;


STORPORTAPI
ULONG
DDKAPI
StorPortInitialize(
  /*IN*/ PVOID  Argument1,
  /*IN*/ PVOID  Argument2,
  /*IN*/ PHW_INITIALIZATION_DATA  HwInitializationData,
  /*IN*/ PVOID  Unused);

STORPORTAPI
VOID
DDKAPI
StorPortFreeDeviceBase(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ PVOID  MappedAddress);

STORPORTAPI
ULONG
DDKAPI
StorPortGetBusData(
  /*IN*/ PVOID  DeviceExtension,
  /*IN*/ ULONG  BusDataType,
  /*IN*/ ULONG  SystemIoBusNumber,
  /*IN*/ ULONG  SlotNumber,
  /*IN*/ PVOID  Buffer,
  /*IN*/ ULONG  Length);

STORPORTAPI
ULONG
DDKAPI
StorPortSetBusDataByOffset(
  /*IN*/ PVOID  DeviceExtension,
  /*IN*/ ULONG   BusDataType,
  /*IN*/ ULONG  SystemIoBusNumber,
  /*IN*/ ULONG  SlotNumber,
  /*IN*/ PVOID  Buffer,
  /*IN*/ ULONG  Offset,
  /*IN*/ ULONG  Length);

STORPORTAPI
PVOID
DDKAPI
StorPortGetDeviceBase(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ INTERFACE_TYPE  BusType,
  /*IN*/ ULONG  SystemIoBusNumber,
  /*IN*/ SCSI_PHYSICAL_ADDRESS  IoAddress,
  /*IN*/ ULONG  NumberOfBytes,
  /*IN*/ BOOLEAN  InIoSpace);

STORPORTAPI
PVOID
DDKAPI
StorPortGetLogicalUnit(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ UCHAR  PathId,
  /*IN*/ UCHAR  TargetId,
  /*IN*/ UCHAR  Lun);

STORPORTAPI
PSCSI_REQUEST_BLOCK
DDKAPI
StorPortGetSrb(
  /*IN*/ PVOID  DeviceExtension,
  /*IN*/ UCHAR  PathId,
  /*IN*/ UCHAR  TargetId,
  /*IN*/ UCHAR  Lun,
  /*IN*/ LONG  QueueTag);

STORPORTAPI
STOR_PHYSICAL_ADDRESS
DDKAPI
StorPortGetPhysicalAddress(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ PSCSI_REQUEST_BLOCK  Srb,
  /*IN*/ PVOID  VirtualAddress,
  /*OUT*/ ULONG  *Length);

STORPORTAPI
PVOID
DDKAPI
StorPortGetVirtualAddress(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ STOR_PHYSICAL_ADDRESS  PhysicalAddress);

STORPORTAPI
PVOID
DDKAPI
StorPortGetUncachedExtension(
  /*IN*/ PVOID HwDeviceExtension,
  /*IN*/ PPORT_CONFIGURATION_INFORMATION ConfigInfo,
  /*IN*/ ULONG NumberOfBytes);

STORPORTAPI
VOID
DDKCDECLAPI
StorPortNotification(
  /*IN*/ SCSI_NOTIFICATION_TYPE  NotificationType,
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ ...);

STORPORTAPI
VOID
DDKAPI
StorPortLogError(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ PSCSI_REQUEST_BLOCK  Srb /*OPTIONAL*/,
  /*IN*/ UCHAR  PathId,
  /*IN*/ UCHAR  TargetId,
  /*IN*/ UCHAR  Lun,
  /*IN*/ ULONG  ErrorCode,
  /*IN*/ ULONG  UniqueId);

STORPORTAPI
VOID
DDKAPI
StorPortCompleteRequest(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ UCHAR  PathId,
  /*IN*/ UCHAR  TargetId,
  /*IN*/ UCHAR  Lun,
  /*IN*/ UCHAR  SrbStatus);

STORPORTAPI
VOID
DDKAPI
StorPortMoveMemory(
  /*IN*/ PVOID  WriteBuffer,
  /*IN*/ PVOID  ReadBuffer,
  /*IN*/ ULONG  Length);

STORPORTAPI
VOID
DDKAPI
StorPortStallExecution(
  /*IN*/ ULONG  Delay);

STORPORTAPI
STOR_PHYSICAL_ADDRESS
DDKAPI
StorPortConvertUlong64ToPhysicalAddress(
  /*IN*/ ULONG64  UlongAddress);

STORPORTAPI
ULONG64
DDKAPI
StorPortConvertPhysicalAddressToUlong64(
  /*IN*/ STOR_PHYSICAL_ADDRESS  Address);

STORPORTAPI
BOOLEAN
DDKAPI
StorPortValidateRange(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ INTERFACE_TYPE  BusType,
  /*IN*/ ULONG  SystemIoBusNumber,
  /*IN*/ STOR_PHYSICAL_ADDRESS  IoAddress,
  /*IN*/ ULONG  NumberOfBytes,
  /*IN*/ BOOLEAN  InIoSpace);

STORPORTAPI
VOID
DDKCDECLAPI
StorPortDebugPrint(
  /*IN*/ ULONG  DebugPrintLevel,
  /*IN*/ PCCHAR  DebugMessage,
  /*IN*/ ...);

STORPORTAPI
UCHAR
DDKAPI
StorPortReadPortUchar(
  /*IN*/ PUCHAR  Port);

STORPORTAPI
ULONG
DDKAPI
StorPortReadPortUlong(
  /*IN*/ PULONG  Port);

STORPORTAPI
USHORT
DDKAPI
StorPortReadPortUshort(
  /*IN*/ PUSHORT  Port);

STORPORTAPI
UCHAR
DDKAPI
StorPortReadRegisterUchar(
  /*IN*/ PUCHAR  Register);

STORPORTAPI
ULONG
DDKAPI
StorPortReadRegisterUlong(
  /*IN*/ PULONG  Register);

STORPORTAPI
USHORT
DDKAPI
StorPortReadRegisterUshort(
  /*IN*/ PUSHORT  Register);

STORPORTAPI
VOID
DDKAPI
StorPortWritePortUchar(
  /*IN*/ PUCHAR  Port,
  /*IN*/ UCHAR  Value);

STORPORTAPI
VOID
DDKAPI
StorPortWritePortUlong(
  /*IN*/ PULONG  Port,
  /*IN*/ ULONG  Value);

STORPORTAPI
VOID
DDKAPI
StorPortWritePortUshort(
  /*IN*/ PUSHORT  Port,
  /*IN*/ USHORT  Value);

STORPORTAPI
VOID
DDKAPI
StorPortWriteRegisterUchar(
  /*IN*/ PUCHAR  Port,
  /*IN*/ UCHAR  Value);

STORPORTAPI
VOID
DDKAPI
StorPortWriteRegisterUlong(
  /*IN*/ PULONG  Port,
  /*IN*/ ULONG  Value);

STORPORTAPI
VOID
DDKAPI
StorPortWriteRegisterUshort(
  /*IN*/ PUSHORT  Port,
  /*IN*/ USHORT  Value);

STORPORTAPI
BOOLEAN
DDKAPI
StorPortPauseDevice(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ UCHAR  PathId,
  /*IN*/ UCHAR  TargetId,
  /*IN*/ UCHAR  Lun,
  /*IN*/ ULONG  TimeOut);

STORPORTAPI
BOOLEAN
DDKAPI
StorPortResumeDevice(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ UCHAR  PathId,
  /*IN*/ UCHAR  TargetId,
  /*IN*/ UCHAR  Lun);

STORPORTAPI
BOOLEAN
DDKAPI
StorPortPause(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ ULONG  TimeOut);

STORPORTAPI
BOOLEAN
DDKAPI
StorPortResume(
  /*IN*/ PVOID  HwDeviceExtension);

STORPORTAPI
BOOLEAN
DDKAPI
StorPortDeviceBusy(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ UCHAR  PathId,
  /*IN*/ UCHAR  TargetId,
  /*IN*/ UCHAR  Lun,
  /*IN*/ ULONG  RequestsToComplete);

STORPORTAPI
BOOLEAN
DDKAPI
StorPortDeviceReady(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ UCHAR  PathId,
  /*IN*/ UCHAR  TargetId,
  /*IN*/ UCHAR  Lun);

STORPORTAPI
BOOLEAN
DDKAPI
StorPortBusy(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ ULONG  RequestsToComplete);

STORPORTAPI
BOOLEAN
DDKAPI
StorPortReady(
  /*IN*/ PVOID  HwDeviceExtension);

STORPORTAPI
PSTOR_SCATTER_GATHER_LIST
DDKAPI
StorPortGetScatterGatherList(
  /*IN*/ PVOID  DeviceExtension,
  /*IN*/ PSCSI_REQUEST_BLOCK  Srb);

typedef BOOLEAN DDKAPI
(*PSTOR_SYNCHRONIZED_ACCESS)(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ PVOID  Context);

STORPORTAPI
VOID
DDKAPI
StorPortSynchronizeAccess(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ PSTOR_SYNCHRONIZED_ACCESS  SynchronizedAccessRoutine,
  /*IN*/ PVOID  Context);

#ifdef DBG
#define DebugPrint(x) StorPortDebugPrint x
#else
#define DebugPrint(x)
#endif

#ifdef __cplusplus
}
#endif

#endif /* __STORPORT_H */
