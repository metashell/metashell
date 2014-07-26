/**
 * @file d4drvif.h
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
#ifndef __D4DRVIF_H
#define __D4DRVIF_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * DOT4 driver IOCTL interface
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"
#include "d4iface.h"

#define FILE_DEVICE_DOT4                  0x3a
#define IOCTL_DOT4_USER_BASE              2049

#define IOCTL_DOT4_ADD_ACTIVITY_BROADCAST \
  CTL_CODE(FILE_DEVICE_DOT4, IOCTL_DOT4_USER_BASE + 4, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_DOT4_CLOSE_CHANNEL \
  CTL_CODE(FILE_DEVICE_DOT4, IOCTL_DOT4_USER_BASE + 1, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_DOT4_CREATE_SOCKET \
  CTL_CODE(FILE_DEVICE_DOT4, IOCTL_DOT4_USER_BASE + 7, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_DOT4_DESTROY_SOCKET \
  CTL_CODE(FILE_DEVICE_DOT4, IOCTL_DOT4_USER_BASE + 9, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_DOT4_OPEN_CHANNEL \
  CTL_CODE(FILE_DEVICE_DOT4, IOCTL_DOT4_USER_BASE + 0, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_DOT4_READ \
  CTL_CODE(FILE_DEVICE_DOT4, IOCTL_DOT4_USER_BASE + 2, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_DOT4_REMOVE_ACTIVITY_BROADCAST \
  CTL_CODE(FILE_DEVICE_DOT4, IOCTL_DOT4_USER_BASE + 5, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_DOT4_WAIT_ACTIVITY_BROADCAST \
  CTL_CODE(FILE_DEVICE_DOT4, IOCTL_DOT4_USER_BASE + 6, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_DOT4_WAIT_FOR_CHANNEL \
  CTL_CODE(FILE_DEVICE_DOT4, IOCTL_DOT4_USER_BASE + 8, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#define IOCTL_DOT4_WRITE \
  CTL_CODE(FILE_DEVICE_DOT4, IOCTL_DOT4_USER_BASE + 3, METHOD_IN_DIRECT, FILE_ANY_ACCESS)


#define MAX_SERVICE_LENGTH                40

typedef struct _DOT4_DC_CREATE_DATA {
  unsigned char  bPsid;
  CHAR  pServiceName[MAX_SERVICE_LENGTH + 1];
  unsigned char  bType;
  ULONG  ulBufferSize;
  USHORT  usMaxHtoPPacketSize;
  USHORT  usMaxPtoHPacketSize;
  unsigned char bHsid;
} DOT4_DC_CREATE_DATA, *PDOT4_DC_CREATE_DATA;

typedef struct _DOT4_DC_DESTROY_DATA {
    unsigned char  bHsid;
} DOT4_DC_DESTROY_DATA, *PDOT4_DC_DESTROY_DATA;

typedef struct _DOT4_DC_OPEN_DATA {
  unsigned char  bHsid;
  unsigned char  fAddActivity;
  CHANNEL_HANDLE  hChannelHandle;
} DOT4_DC_OPEN_DATA, *PDOT4_DC_OPEN_DATA;

typedef struct _DOT4_DRIVER_CMD {
  CHANNEL_HANDLE  hChannelHandle;
  ULONG  ulSize;
  ULONG  ulOffset;
  ULONG  ulTimeout;
} DOT4_DRIVER_CMD, *PDOT4_DRIVER_CMD;

#ifdef __cplusplus
}
#endif

#endif /* __D4DRVIF_H */
