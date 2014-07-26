/**
 * @file usbiodef.h
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
#ifndef __USBIODEF_H
#define __USBIODEF_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * USB IOCTL definitions
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"

DEFINE_GUID(GUID_DEVINTERFACE_USB_HUB, \
  0xf18a0e88, 0xc30c, 0x11d0, 0x88, 0x15, 0x00, 0xa0, 0xc9, 0x06, 0xbe, 0xd8);

DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE,
  0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED);

DEFINE_GUID(GUID_DEVINTERFACE_USB_HOST_CONTROLLER,
  0x3abf6f2d, 0x71c4, 0x462a, 0x8a, 0x92, 0x1e, 0x68, 0x61, 0xe6, 0xaf, 0x27);
             
DEFINE_GUID(GUID_USB_WMI_STD_DATA,
  0x4E623B20L, 0xCB14, 0x11D1, 0xB3, 0x31, 0x00, 0xA0, 0xC9, 0x59, 0xBB, 0xD2);

DEFINE_GUID(GUID_USB_WMI_STD_NOTIFICATION,
  0x4E623B20L, 0xCB14, 0x11D1, 0xB3, 0x31, 0x00, 0xA0, 0xC9, 0x59, 0xBB, 0xD2);        

#define GUID_CLASS_USBHUB                 GUID_DEVINTERFACE_USB_HUB
#define GUID_CLASS_USB_DEVICE             GUID_DEVINTERFACE_USB_DEVICE
#define GUID_CLASS_USB_HOST_CONTROLLER    GUID_DEVINTERFACE_USB_HOST_CONTROLLER

#define USB_SUBMIT_URB                    0
#define USB_RESET_PORT                    1
#define USB_GET_ROOTHUB_PDO               3
#define USB_GET_PORT_STATUS               4
#define USB_ENABLE_PORT                   5
#define USB_GET_HUB_COUNT                 6
#define USB_CYCLE_PORT                    7
#define USB_GET_HUB_NAME                  8
#define USB_IDLE_NOTIFICATION             9
#define USB_GET_BUS_INFO                  264
#define USB_GET_CONTROLLER_NAME           265
#define USB_GET_BUSGUID_INFO              266
#define USB_GET_PARENT_HUB_INFO           267
#define USB_GET_DEVICE_HANDLE             268

#define HCD_GET_STATS_1                   255
#define HCD_DIAGNOSTIC_MODE_ON            256
#define HCD_DIAGNOSTIC_MODE_OFF           257
#define HCD_GET_ROOT_HUB_NAME             258
#define HCD_GET_DRIVERKEY_NAME            265
#define HCD_GET_STATS_2                   266
#define HCD_DISABLE_PORT                  268
#define HCD_ENABLE_PORT                   269
#define HCD_USER_REQUEST                  270

#define USB_GET_NODE_INFORMATION                258
#define USB_GET_NODE_CONNECTION_INFORMATION     259
#define USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION 260
#define USB_GET_NODE_CONNECTION_NAME            261
#define USB_DIAG_IGNORE_HUBS_ON                 262
#define USB_DIAG_IGNORE_HUBS_OFF                263
#define USB_GET_NODE_CONNECTION_DRIVERKEY_NAME  264
#define USB_GET_HUB_CAPABILITIES                271
#define USB_GET_NODE_CONNECTION_ATTRIBUTES      272

#define FILE_DEVICE_USB                   FILE_DEVICE_UNKNOWN

#define USB_CTL(id) CTL_CODE(FILE_DEVICE_USB, \
														 (id), \
														 METHOD_BUFFERED, \
														 FILE_ANY_ACCESS)

#define USB_KERNEL_CTL(id) CTL_CODE(FILE_DEVICE_USB, \
                                    (id), \
                                    METHOD_NEITHER, \
                                    FILE_ANY_ACCESS)

#ifdef __cplusplus
}
#endif

#endif /* __USBIODEF_H */
