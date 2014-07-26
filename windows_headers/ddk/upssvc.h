/**
 * @file upssvc.h
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
#ifndef __UPSSVC_H
#define __UPSSVC_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * UPS service interface
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"

#if defined(_APCUPS_)
  #define UPSAPI DECLSPEC_EXPORT
#else
  #define UPSAPI DECLSPEC_IMPORT
#endif


#define UPS_ONLINE                        1
#define UPS_ONBATTERY                     2
#define UPS_LOWBATTERY                    4
#define UPS_NOCOMM                        8
#define UPS_CRITICAL                      16

UPSAPI
VOID
DDKAPI
UPSCancelWait(VOID);

UPSAPI
DWORD
DDKAPI
UPSGetState(VOID);

#define UPS_INITUNKNOWNERROR              0
#define UPS_INITOK                        1
#define UPS_INITNOSUCHDRIVER              2
#define UPS_INITBADINTERFACE              3
#define UPS_INITREGISTRYERROR             4
#define UPS_INITCOMMOPENERROR             5
#define UPS_INITCOMMSETUPERROR            6

UPSAPI
DWORD
DDKAPI
UPSInit(VOID);

UPSAPI
VOID
DDKAPI
UPSStop(VOID);

UPSAPI
VOID
DDKAPI
UPSTurnOff(
  /*IN*/ DWORD  aTurnOffDelay);

UPSAPI
VOID
DDKAPI
UPSWaitForStateChange(
  /*IN*/ DWORD  aCurrentState,
  /*IN*/ DWORD  anInterval);

#ifdef __cplusplus
}
#endif

#endif /* __UPSSVC_H */
