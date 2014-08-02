/**
 * @file mountdev.h
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
#ifndef __MOUNTDEV_H
#define __MOUNTDEV_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Mount point manager/mounted devices interface.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"
#include "mountmgr.h"

#define IOCTL_MOUNTDEV_QUERY_DEVICE_NAME \
  CTL_CODE(MOUNTDEVCONTROLTYPE, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_MOUNTDEV_QUERY_UNIQUE_ID \
  CTL_CODE(MOUNTDEVCONTROLTYPE, 0, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_MOUNTDEV_UNIQUE_ID_CHANGE_NOTIFY \
  CTL_CODE(MOUNTDEVCONTROLTYPE, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_MOUNTDEV_QUERY_SUGGESTED_LINK_NAME \
  CTL_CODE(MOUNTDEVCONTROLTYPE, 3, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_MOUNTDEV_LINK_CREATED \
  CTL_CODE(MOUNTDEVCONTROLTYPE, 4, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_MOUNTDEV_LINK_DELETED \
  CTL_CODE(MOUNTDEVCONTROLTYPE, 5, METHOD_BUFFERED, FILE_ANY_ACCESS)


typedef struct _MOUNTDEV_SUGGESTED_LINK_NAME {
  BOOLEAN  UseOnlyIfThereAreNoOtherLinks;
  USHORT  NameLength;
  WCHAR  Name[1];
} MOUNTDEV_SUGGESTED_LINK_NAME, *PMOUNTDEV_SUGGESTED_LINK_NAME;

typedef struct _MOUNTDEV_UNIQUE_ID {
  USHORT  UniqueIdLength;
  UCHAR  UniqueId[1];
} MOUNTDEV_UNIQUE_ID, *PMOUNTDEV_UNIQUE_ID;

typedef struct _MOUNTDEV_UNIQUE_ID_CHANGE_NOTIFY_OUTPUT {
  ULONG  Size;
  USHORT  OldUniqueIdOffset;
  USHORT  OldUniqueIdLength;
  USHORT  NewUniqueIdOffset;
  USHORT  NewUniqueIdLength;
} MOUNTDEV_UNIQUE_ID_CHANGE_NOTIFY_OUTPUT;

#ifdef __cplusplus
}
#endif

#endif /* __MOUNTDEV_H */
