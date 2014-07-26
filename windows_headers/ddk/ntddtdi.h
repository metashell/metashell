/**
 * @file ntddtdi.h
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
#ifndef __NTDDTDI_H
#define __NTDDTDI_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * TDI IOCTL interface
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"

#define DD_TDI_DEVICE_NAME                "\\Device\\UNKNOWN"
#define DD_TDI_DEVICE_NAME_U              L"\\Device\\UNKNOWN"

#define _TDI_CONTROL_CODE(Request, Method) \
  CTL_CODE(FILE_DEVICE_TRANSPORT, Request, Method, FILE_ANY_ACCESS)

#define IOCTL_TDI_ACCEPT                  _TDI_CONTROL_CODE(0,  METHOD_BUFFERED)
#define IOCTL_TDI_CONNECT                 _TDI_CONTROL_CODE(1,  METHOD_BUFFERED)
#define IOCTL_TDI_DISCONNECT              _TDI_CONTROL_CODE(2,  METHOD_BUFFERED)
#define IOCTL_TDI_LISTEN                  _TDI_CONTROL_CODE(3,  METHOD_BUFFERED)
#define IOCTL_TDI_QUERY_INFORMATION       _TDI_CONTROL_CODE(4,  METHOD_OUT_DIRECT)
#define IOCTL_TDI_RECEIVE                 _TDI_CONTROL_CODE(5,  METHOD_OUT_DIRECT)
#define IOCTL_TDI_RECEIVE_DATAGRAM        _TDI_CONTROL_CODE(6,  METHOD_OUT_DIRECT)
#define IOCTL_TDI_SEND                    _TDI_CONTROL_CODE(7,  METHOD_IN_DIRECT)
#define IOCTL_TDI_SEND_DATAGRAM           _TDI_CONTROL_CODE(8,  METHOD_IN_DIRECT)
#define IOCTL_TDI_SET_EVENT_HANDLER       _TDI_CONTROL_CODE(9,  METHOD_BUFFERED)
#define IOCTL_TDI_SET_INFORMATION         _TDI_CONTROL_CODE(10, METHOD_IN_DIRECT)
#define IOCTL_TDI_ASSOCIATE_ADDRESS       _TDI_CONTROL_CODE(11, METHOD_BUFFERED)
#define IOCTL_TDI_DISASSOCIATE_ADDRESS    _TDI_CONTROL_CODE(12, METHOD_BUFFERED)
#define IOCTL_TDI_ACTION                  _TDI_CONTROL_CODE(13, METHOD_OUT_DIRECT)

#ifdef __cplusplus
}
#endif

#endif /* __NTDDTDI_H */
