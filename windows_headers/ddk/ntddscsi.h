/**
 * @file ntddscsi.h
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
#ifndef __NTDDSCSI_H
#define __NTDDSCSI_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * SCSI port IOCTL interface.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"

#define DD_SCSI_DEVICE_NAME               "\\Device\\ScsiPort"
#define DD_SCSI_DEVICE_NAME_U             L"\\Device\\ScsiPort"

#define IOCTL_SCSI_BASE                   FILE_DEVICE_CONTROLLER

#define IOCTL_SCSI_GET_INQUIRY_DATA \
  CTL_CODE(IOCTL_SCSI_BASE, 0x0403, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SCSI_GET_CAPABILITIES \
  CTL_CODE(IOCTL_SCSI_BASE, 0x0404, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SCSI_GET_ADDRESS \
  CTL_CODE(IOCTL_SCSI_BASE, 0x0406, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_SCSI_MINIPORT \
  CTL_CODE(IOCTL_SCSI_BASE, 0x0402, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

#define IOCTL_SCSI_PASS_THROUGH \
  CTL_CODE(IOCTL_SCSI_BASE, 0x0401, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

#define IOCTL_SCSI_PASS_THROUGH_DIRECT \
  CTL_CODE(IOCTL_SCSI_BASE, 0x0405, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

#define IOCTL_SCSI_RESCAN_BUS \
  CTL_CODE(IOCTL_SCSI_BASE, 0x0407, METHOD_BUFFERED, FILE_ANY_ACCESS)


DEFINE_GUID(ScsiRawInterfaceGuid, \
  0x53f56309L, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);

DEFINE_GUID(WmiScsiAddressGuid, \
  0x53f5630fL, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);

typedef struct _SCSI_PASS_THROUGH {
  USHORT  Length;
  UCHAR  ScsiStatus;
  UCHAR  PathId;
  UCHAR  TargetId;
  UCHAR  Lun;
  UCHAR  CdbLength;
  UCHAR  SenseInfoLength;
  UCHAR  DataIn;
  ULONG  DataTransferLength;
  ULONG  TimeOutValue;
  ULONG_PTR DataBufferOffset;
  ULONG  SenseInfoOffset;
  UCHAR  Cdb[16];
} SCSI_PASS_THROUGH, *PSCSI_PASS_THROUGH;

typedef struct _SCSI_PASS_THROUGH_DIRECT {
  USHORT  Length;
  UCHAR  ScsiStatus;
  UCHAR  PathId;
  UCHAR  TargetId;
  UCHAR  Lun;
  UCHAR  CdbLength;
  UCHAR  SenseInfoLength;
  UCHAR  DataIn;
  ULONG  DataTransferLength;
  ULONG  TimeOutValue;
  PVOID  DataBuffer;
  ULONG  SenseInfoOffset;
  UCHAR  Cdb[16];
} SCSI_PASS_THROUGH_DIRECT, *PSCSI_PASS_THROUGH_DIRECT;

typedef struct _SRB_IO_CONTROL { 
  ULONG  HeaderLength; 
  UCHAR  Signature[8]; 
  ULONG  Timeout; 
  ULONG  ControlCode; 
  ULONG  ReturnCode; 
  ULONG  Length; 
} SRB_IO_CONTROL, *PSRB_IO_CONTROL; 

typedef struct _SCSI_ADDRESS {
	ULONG  Length;
	UCHAR  PortNumber;
	UCHAR  PathId;
	UCHAR  TargetId;
	UCHAR  Lun;
} SCSI_ADDRESS, *PSCSI_ADDRESS;

typedef struct _SCSI_BUS_DATA {
	UCHAR  NumberOfLogicalUnits;
	UCHAR  InitiatorBusId;
	ULONG  InquiryDataOffset;
}SCSI_BUS_DATA, *PSCSI_BUS_DATA;

typedef struct _SCSI_ADAPTER_BUS_INFO {
	UCHAR  NumberOfBuses;
	SCSI_BUS_DATA  BusData[1];
} SCSI_ADAPTER_BUS_INFO, *PSCSI_ADAPTER_BUS_INFO;

typedef struct _IO_SCSI_CAPABILITIES {
	ULONG  Length;
	ULONG  MaximumTransferLength;
	ULONG  MaximumPhysicalPages;
	ULONG  SupportedAsynchronousEvents;
	ULONG  AlignmentMask;
	BOOLEAN  TaggedQueuing;
	BOOLEAN  AdapterScansDown;
	BOOLEAN  AdapterUsesPio;
} IO_SCSI_CAPABILITIES, *PIO_SCSI_CAPABILITIES;

typedef struct _SCSI_INQUIRY_DATA {
	UCHAR  PathId;
	UCHAR  TargetId;
	UCHAR  Lun;
	BOOLEAN  DeviceClaimed;
	ULONG  InquiryDataLength;
	ULONG  NextInquiryDataOffset;
	UCHAR  InquiryData[1];
} SCSI_INQUIRY_DATA, *PSCSI_INQUIRY_DATA;

#define SCSI_IOCTL_DATA_OUT               0
#define SCSI_IOCTL_DATA_IN                1
#define SCSI_IOCTL_DATA_UNSPECIFIED       2

typedef struct _DUMP_POINTERS {
	PADAPTER_OBJECT  AdapterObject;
	PVOID  MappedRegisterBase;
	PVOID  DumpData;
	PVOID  CommonBufferVa;
	LARGE_INTEGER  CommonBufferPa;
	ULONG  CommonBufferSize;
	BOOLEAN  AllocateCommonBuffers;
	BOOLEAN  UseDiskDump;
	UCHAR  Spare1[2];
	PVOID  DeviceObject;
} DUMP_POINTERS, *PDUMP_POINTERS;

#ifdef __cplusplus
}
#endif

#endif /* __NTDDSCSI_H */
