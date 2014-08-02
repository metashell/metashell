/**
 * @file ntddpar.h
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
#ifndef __NTDDPAR_H
#define __NTDDPAR_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Parallel port driver interface
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"

/* Parallel port device GUIDs */

DEFINE_GUID (GUID_DEVINTERFACE_PARALLEL,
  0x97F76EF0, 0xF883, 0x11D0, 0xAF, 0x1F, 0x00, 0x00, 0xF8, 0x00, 0x84, 0x5C);
DEFINE_GUID (GUID_DEVINTERFACE_PARCLASS,
  0x811FC6A5, 0xF728, 0x11D0, 0xA5, 0x37, 0x00, 0x00, 0xF8, 0x75, 0x3E, 0xD1);

#define GUID_PARALLEL_DEVICE GUID_DEVINTERFACE_PARALLEL
#define GUID_PARCLASS_DEVICE GUID_DEVINTERFACE_PARCLASS


#define IOCTL_IEEE1284_GET_MODE \
  CTL_CODE (FILE_DEVICE_PARALLEL_PORT, 5, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IEEE1284_NEGOTIATE \
  CTL_CODE (FILE_DEVICE_PARALLEL_PORT, 6, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PAR_GET_DEFAULT_MODES \
  CTL_CODE (FILE_DEVICE_PARALLEL_PORT, 10, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PAR_GET_DEVICE_CAPS \
  CTL_CODE (FILE_DEVICE_PARALLEL_PORT, 9, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PAR_IS_PORT_FREE \
  CTL_CODE (FILE_DEVICE_PARALLEL_PORT, 21, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PAR_QUERY_DEVICE_ID \
  CTL_CODE (FILE_DEVICE_PARALLEL_PORT, 3, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PAR_QUERY_DEVICE_ID_SIZE \
  CTL_CODE (FILE_DEVICE_PARALLEL_PORT, 4, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PAR_QUERY_INFORMATION \
  CTL_CODE (FILE_DEVICE_PARALLEL_PORT, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PAR_QUERY_LOCATION \
  CTL_CODE (FILE_DEVICE_PARALLEL_PORT, 22, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PAR_QUERY_RAW_DEVICE_ID \
  CTL_CODE (FILE_DEVICE_PARALLEL_PORT, 12, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PAR_SET_INFORMATION \
  CTL_CODE (FILE_DEVICE_PARALLEL_PORT, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PAR_SET_READ_ADDRESS \
  CTL_CODE (FILE_DEVICE_PARALLEL_PORT, 8, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PAR_SET_WRITE_ADDRESS \
  CTL_CODE (FILE_DEVICE_PARALLEL_PORT, 7, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _PAR_DEVICE_ID_SIZE_INFORMATION {
  ULONG  DeviceIdSize;
} PAR_DEVICE_ID_SIZE_INFORMATION, *PPAR_DEVICE_ID_SIZE_INFORMATION;

#define PARALLEL_INIT                     0x01
#define PARALLEL_AUTOFEED                 0x02
#define PARALLEL_PAPER_EMPTY              0x04
#define PARALLEL_OFF_LINE                 0x08
#define PARALLEL_POWER_OFF                0x10
#define PARALLEL_NOT_CONNECTED            0x20
#define PARALLEL_BUSY                     0x40
#define PARALLEL_SELECTED                 0x80

typedef struct _PAR_QUERY_INFORMATION {
  UCHAR  Status;
} PAR_QUERY_INFORMATION, *PPAR_QUERY_INFORMATION;

typedef struct _PAR_SET_INFORMATION {
  UCHAR  Init;
} PAR_SET_INFORMATION, *PPAR_SET_INFORMATION;

typedef struct _PARCLASS_NEGOTIATION_MASK {
  USHORT  usReadMask;
  USHORT  usWriteMask;
} PARCLASS_NEGOTIATION_MASK, *PPARCLASS_NEGOTIATION_MASK;

#define NONE                              0x0000
#define CENTRONICS                        0x0001
#define IEEE_COMPATIBILITY                0x0002
#define NIBBLE                            0x0004
#define CHANNEL_NIBBLE                    0x0008
#define BYTE_BIDIR                        0x0010
#define EPP_HW                            0x0020
#define EPP_SW                            0x0040
#define EPP_ANY                           0x0060
#define BOUNDED_ECP                       0x0080
#define ECP_HW_NOIRQ                      0x0100
#define ECP_HW_IRQ                        0x0200
#define ECP_SW                            0x0400
#define ECP_ANY                           0x0780

#ifdef __cplusplus
}
#endif

#endif /* __NTDDPAR_H */
