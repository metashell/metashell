/**
 * @file d4iface.h
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
#ifndef __D4IFACE_H
#define __D4IFACE_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"

#define DOT4_MAX_CHANNELS                 128
#define NO_TIMEOUT                        0

#define DOT4_CHANNEL                      0
#define HP_MESSAGE_PROCESSOR              1
#define PRINTER_CHANNEL                   2
#define SCANNER_CHANNEL                   4
#define MIO_COMMAND_PROCESSOR             5
#define ECHO_CHANNEL                      6
#define FAX_SEND_CHANNEL                  7
#define FAX_RECV_CHANNEL                  8
#define DIAGNOSTIC_CHANNEL                9
#define HP_RESERVED                       10
#define IMAGE_DOWNLOAD                    11
#define HOST_DATASTORE_UPLOAD             12
#define HOST_DATASTORE_DOWNLOAD           13
#define CONFIG_UPLOAD                     14
#define CONFIG_DOWNLOAD                   15

#define STREAM_TYPE_CHANNEL               1
#define PACKET_TYPE_CHANNEL               2

/* DOT4_ACTIVITY.ulMessage flags */
#define DOT4_STREAM_RECEIVED              0x100
#define DOT4_STREAM_CREDITS               0x101
#define DOT4_MESSAGE_RECEIVED             0x102
#define DOT4_DISCONNECT                   0x103
#define DOT4_CHANNEL_CLOSED               0x105

typedef unsigned long CHANNEL_HANDLE, *PCHANNEL_HANDLE;

typedef struct _DOT4_ACTIVITY {
  ULONG  ulMessage;
  ULONG  ulByteCount;
  CHANNEL_HANDLE  hChannel;
} DOT4_ACTIVITY, *PDOT4_ACTIVITY;

typedef struct _DOT4_WMI_XFER_INFO {
  ULONG  ulStreamBytesWritten;
  ULONG  ulStreamBytesRead;
  ULONG  ulPacketBytesWritten;
  ULONG  ulPacketBytesRead;
} DOT4_WMI_XFER_INFO, *PDOT4_WMI_XFER_INFO;

#ifdef __cplusplus
}
#endif

#endif /* __D4IFACE_H */
