/**
 * @file win2k.h
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
#ifndef __WIN2K_H
#define __WIN2K_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Definitions only used in Windows 2000 and earlier versions
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"

#pragma pack(push,4)

typedef enum _BUS_DATA_TYPE {
  ConfigurationSpaceUndefined = -1,
  Cmos,
  EisaConfiguration,
  Pos,
  CbusConfiguration,
  PCIConfiguration,
  VMEConfiguration,
  NuBusConfiguration,
  PCMCIAConfiguration,
  MPIConfiguration,
  MPSAConfiguration,
  PNPISAConfiguration,
  SgiInternalConfiguration,
  MaximumBusDataType
} BUS_DATA_TYPE, *PBUS_DATA_TYPE;

NTOSAPI
VOID
DDKAPI
ExReleaseResourceForThreadLite(
  /*IN*/ PERESOURCE  Resource,
  /*IN*/ ERESOURCE_THREAD  ResourceThreadId);

NTOSAPI
NTSTATUS
DDKAPI
IoReadPartitionTable(
  /*IN*/ PDEVICE_OBJECT  DeviceObject,
  /*IN*/ ULONG  SectorSize,
  /*IN*/ BOOLEAN  ReturnRecognizedPartitions,
  /*OUT*/ struct _DRIVE_LAYOUT_INFORMATION  **PartitionBuffer);

NTOSAPI
NTSTATUS
DDKAPI
IoSetPartitionInformation(
  /*IN*/ PDEVICE_OBJECT  DeviceObject,
  /*IN*/ ULONG  SectorSize,
  /*IN*/ ULONG  PartitionNumber,
  /*IN*/ ULONG  PartitionType);

NTOSAPI
NTSTATUS
DDKAPI
IoWritePartitionTable(
  /*IN*/ PDEVICE_OBJECT  DeviceObject,
  /*IN*/ ULONG  SectorSize,
  /*IN*/ ULONG  SectorsPerTrack,
  /*IN*/ ULONG  NumberOfHeads,
  /*IN*/ struct _DRIVE_LAYOUT_INFORMATION  *PartitionBuffer);

/*
 * PVOID MmGetSystemAddressForMdl(
 * IN PMDL  Mdl); 
 */
#define MmGetSystemAddressForMdl(Mdl) \
  (((Mdl)->MdlFlags & (MDL_MAPPED_TO_SYSTEM_VA | \
    MDL_SOURCE_IS_NONPAGED_POOL)) ? \
      ((Mdl)->MappedSystemVa) : \
      (MmMapLockedPages((Mdl), KernelMode)))

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __WIN2K_H */
