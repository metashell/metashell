/**
 * @file ntddtape.h
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
#ifndef __NTDDTAPE_H
#define __NTDDTAPE_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Tape device IOCTL interface.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"
#include "ntddstor.h"

#define DD_TAPE_DEVICE_NAME               "\\Device\\Tape"
#define DD_TAPE_DEVICE_NAME_U             L"\\Device\\Tape"

#define IOCTL_TAPE_BASE                   FILE_DEVICE_TAPE

#define IOCTL_TAPE_CHECK_VERIFY         CTL_CODE(IOCTL_TAPE_BASE, 0x0200, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_TAPE_CREATE_PARTITION     CTL_CODE(IOCTL_TAPE_BASE, 0x000a, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_TAPE_ERASE                CTL_CODE(IOCTL_TAPE_BASE, 0x0000, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_TAPE_FIND_NEW_DEVICES     CTL_CODE(IOCTL_DISK_BASE, 0x0206, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_TAPE_GET_DRIVE_PARAMS     CTL_CODE(IOCTL_TAPE_BASE, 0x0005, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_TAPE_GET_MEDIA_PARAMS     CTL_CODE(IOCTL_TAPE_BASE, 0x0007, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_TAPE_GET_POSITION         CTL_CODE(IOCTL_TAPE_BASE, 0x0003, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_TAPE_GET_STATUS           CTL_CODE(IOCTL_TAPE_BASE, 0x0009, METHOD_BUFFERED, FILE_READ_ACCESS )

#define IOCTL_TAPE_PREPARE              CTL_CODE(IOCTL_TAPE_BASE, 0x0001, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_TAPE_SET_DRIVE_PARAMS     CTL_CODE(IOCTL_TAPE_BASE, 0x0006, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_TAPE_SET_MEDIA_PARAMS     CTL_CODE(IOCTL_TAPE_BASE, 0x0008, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_TAPE_SET_POSITION         CTL_CODE(IOCTL_TAPE_BASE, 0x0004, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_TAPE_WRITE_MARKS          CTL_CODE(IOCTL_TAPE_BASE, 0x0002, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)


typedef enum _TAPE_DRIVE_PROBLEM_TYPE {
  TapeDriveProblemNone, 
  TapeDriveReadWriteWarning,
  TapeDriveReadWriteError, 
  TapeDriveReadWarning,
  TapeDriveWriteWarning,
  TapeDriveReadError,
  TapeDriveWriteError, 
  TapeDriveHardwareError,
  TapeDriveUnsupportedMedia, 
  TapeDriveScsiConnectionError,
  TapeDriveTimetoClean, 
  TapeDriveCleanDriveNow,
  TapeDriveMediaLifeExpired, 
  TapeDriveSnappedTape
} TAPE_DRIVE_PROBLEM_TYPE;

#ifdef __cplusplus
}
#endif

#endif /* __NTDDTAPE_H */
