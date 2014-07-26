/**
 * @file netpnp.h
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
#ifndef __NETPNP_H
#define __NETPNP_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Network Plug and Play event support
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _NET_PNP_EVENT_CODE {
  NetEventSetPower,
  NetEventQueryPower,
  NetEventQueryRemoveDevice,
  NetEventCancelRemoveDevice,
  NetEventReconfigure,
  NetEventBindList,
  NetEventBindsComplete,
  NetEventPnPCapabilities,
  NetEventMaximum
} NET_PNP_EVENT_CODE, *PNET_PNP_EVENT_CODE;

typedef struct _NET_PNP_EVENT {
  NET_PNP_EVENT_CODE  NetEvent;
  PVOID  Buffer;
  ULONG  BufferLength;
  ULONG_PTR  NdisReserved[4];
  ULONG_PTR  TransportReserved[4];
  ULONG_PTR  TdiReserved[4];
  ULONG_PTR  TdiClientReserved[4];
} NET_PNP_EVENT, *PNET_PNP_EVENT;

typedef enum _NET_DEVICE_POWER_STATE {
  NetDeviceStateUnspecified,
  NetDeviceStateD0,
  NetDeviceStateD1,
  NetDeviceStateD2,
  NetDeviceStateD3,
  NetDeviceStateMaximum
} NET_DEVICE_POWER_STATE, *PNET_DEVICE_POWER_STATE;

#ifdef __cplusplus
}
#endif

#endif /* __NETPNP_H */
