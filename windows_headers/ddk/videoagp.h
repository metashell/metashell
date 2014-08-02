/**
 * @file videoagp.h
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
#ifndef __VIDEOAGP_H
#define __VIDEOAGP_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Video miniport AGP interface
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"
#include "video.h"


#define VIDEO_AGP_RATE_1X                 0x00000001
#define VIDEO_AGP_RATE_2X                 0x00000002
#define VIDEO_AGP_RATE_4X                 0x00000004
#define VIDEO_AGP_RATE_8X                 0x00000008

typedef enum _VIDEO_PORT_CACHE_TYPE {
	VpNonCached = 0,
	VpWriteCombined,
	VpCached
} VIDEO_PORT_CACHE_TYPE;

typedef BOOLEAN DDKAPI
(*PAGP_COMMIT_PHYSICAL)(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ PVOID  PhysicalReserveContext,
  /*IN*/ ULONG  Pages,
  /*IN*/ ULONG  Offset);

typedef PVOID DDKAPI
(*PAGP_COMMIT_VIRTUAL)(
  /*IN*/  PVOID  HwDeviceExtension,
  /*IN*/  PVOID  VirtualReserveContext,
  /*IN*/  ULONG  Pages,
  /*IN*/  ULONG  Offset);

typedef VOID DDKAPI
(*PAGP_FREE_PHYSICAL)(
  /*IN*/  PVOID  HwDeviceExtension,
  /*IN*/  PVOID  PhysicalReserveContext,
  /*IN*/  ULONG  Pages,
  /*IN*/  ULONG  Offset);

typedef VOID DDKAPI
(*PAGP_FREE_VIRTUAL)(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ PVOID  VirtualReserveContext,
  /*IN*/ ULONG  Pages,
  /*IN*/ ULONG  Offset);

typedef VOID DDKAPI
(*PAGP_RELEASE_PHYSICAL)(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ PVOID  PhysicalReserveContext);

typedef VOID DDKAPI
(*PAGP_RELEASE_VIRTUAL)(
  /*IN*/  PVOID  HwDeviceExtension,
  /*IN*/  PVOID  VirtualReserveContext);

typedef PHYSICAL_ADDRESS DDKAPI
(*PAGP_RESERVE_PHYSICAL)(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ ULONG  Pages,
  /*IN*/ VIDEO_PORT_CACHE_TYPE  Caching,
  /*OUT*/ PVOID  *PhysicalReserveContext);

typedef PVOID DDKAPI
(*PAGP_RESERVE_VIRTUAL)(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ HANDLE  ProcessHandle,
  /*IN*/ PVOID  PhysicalReserveContext,
  /*OUT*/ PVOID  *VirtualReserveContext);

typedef BOOLEAN DDKAPI
(*PAGP_SET_RATE)(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ ULONG  AgpRate);

typedef struct _VIDEO_PORT_AGP_SERVICES {
  PAGP_RESERVE_PHYSICAL  AgpReservePhysical;
  PAGP_RELEASE_PHYSICAL  AgpReleasePhysical;
  PAGP_COMMIT_PHYSICAL  AgpCommitPhysical;
  PAGP_FREE_PHYSICAL  AgpFreePhysical;
  PAGP_RESERVE_VIRTUAL  AgpReserveVirtual;
  PAGP_RELEASE_VIRTUAL  AgpReleaseVirtual;
  PAGP_COMMIT_VIRTUAL  AgpCommitVirtual;
  PAGP_FREE_VIRTUAL  AgpFreeVirtual;
  ULONGLONG  AllocationLimit;
} VIDEO_PORT_AGP_SERVICES, *PVIDEO_PORT_AGP_SERVICES;

VPAPI
BOOLEAN
DDKAPI
VideoPortGetAgpServices(
  /*IN*/ PVOID  HwDeviceExtension,
  /*IN*/ PVIDEO_PORT_AGP_SERVICES  AgpServices);

#ifdef __cplusplus
}
#endif

#endif /* __VIDEOAGP_H */
