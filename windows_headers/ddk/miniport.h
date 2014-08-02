/**
 * @file miniport.h
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
#ifndef __MINIPORT_H
#define __MINIPORT_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Type definitions for miniport drivers
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"

#define EMULATOR_READ_ACCESS              0x01
#define EMULATOR_WRITE_ACCESS             0x02

typedef enum _EMULATOR_PORT_ACCESS_TYPE {
	Uchar,
	Ushort,
	Ulong
} EMULATOR_PORT_ACCESS_TYPE, *PEMULATOR_PORT_ACCESS_TYPE;


typedef struct _EMULATOR_ACCESS_ENTRY {
  ULONG  BasePort;
  ULONG  NumConsecutivePorts;
  EMULATOR_PORT_ACCESS_TYPE  AccessType;
  UCHAR  AccessMode;
  UCHAR  StringSupport;
  PVOID  Routine;
} EMULATOR_ACCESS_ENTRY, *PEMULATOR_ACCESS_ENTRY;

#ifndef VIDEO_ACCESS_RANGE_DEFINED /* also in video.h */
#define VIDEO_ACCESS_RANGE_DEFINED
typedef struct _VIDEO_ACCESS_RANGE {
  PHYSICAL_ADDRESS  RangeStart;
  ULONG  RangeLength;
  UCHAR  RangeInIoSpace;
  UCHAR  RangeVisible;
  UCHAR  RangeShareable;
  UCHAR  RangePassive;
} VIDEO_ACCESS_RANGE, *PVIDEO_ACCESS_RANGE;
#endif

typedef VOID DDKAPI
(*PBANKED_SECTION_ROUTINE)(
  /*IN*/ ULONG  ReadBank,
  /*IN*/ ULONG  WriteBank,
  /*IN*/ PVOID  Context);

#ifdef __cplusplus
}
#endif

#endif /* __MINIPORT_H */
