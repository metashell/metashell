/**
 * @file hidsdi.h
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
#ifndef _HIDSDI_H
#define _HIDSDI_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <ddk/hidusage.h>
#include <ddk/hidpi.h>

#ifndef WINHIDSDI
#define WINHIDSDI DECLSPEC_IMPORT
#endif

#include <pshpack4.h>

typedef struct _HIDD_CONFIGURATION {
	PVOID cookie;
 	ULONG size;
	ULONG RingBufferSize;
} HIDD_CONFIGURATION, *PHIDD_CONFIGURATION;

typedef struct _HIDD_ATTRIBUTES {
	ULONG Size; 
	USHORT VendorID;
	USHORT ProductID;
	USHORT VersionNumber;
} HIDD_ATTRIBUTES, *PHIDD_ATTRIBUTES;

WINHIDSDI BOOL WINAPI HidD_GetAttributes (HANDLE, PHIDD_ATTRIBUTES);
WINHIDSDI VOID WINAPI HidD_GetHidGuid (LPGUID);
WINHIDSDI BOOL WINAPI HidD_GetPreparsedData(HANDLE, PHIDP_PREPARSED_DATA  *);
WINHIDSDI BOOL WINAPI HidD_FreePreparsedData(PHIDP_PREPARSED_DATA);
WINHIDSDI BOOL WINAPI HidD_FlushQueue (HANDLE);
WINHIDSDI BOOL WINAPI HidD_GetConfiguration (HANDLE, PHIDD_CONFIGURATION, ULONG);                
WINHIDSDI BOOL WINAPI HidD_SetConfiguration (HANDLE, PHIDD_CONFIGURATION, ULONG);
WINHIDSDI BOOL WINAPI HidD_GetFeature (HANDLE, PVOID, ULONG);
WINHIDSDI BOOL WINAPI HidD_SetFeature (HANDLE, PVOID, ULONG);
WINHIDSDI BOOL WINAPI HidD_GetNumInputBuffers (HANDLE, PULONG);
WINHIDSDI BOOL WINAPI HidD_SetNumInputBuffers (HANDLE HidDeviceObject, ULONG);
WINHIDSDI BOOL WINAPI HidD_GetPhysicalDescriptor (HANDLE, PVOID, ULONG);
WINHIDSDI BOOL WINAPI HidD_GetManufacturerString (HANDLE, PVOID, ULONG);
WINHIDSDI BOOL WINAPI HidD_GetProductString ( HANDLE, PVOID, ULONG);
WINHIDSDI BOOL WINAPI HidD_GetIndexedString ( HANDLE, ULONG, PVOID, ULONG);
WINHIDSDI BOOL WINAPI HidD_GetSerialNumberString (HANDLE, PVOID, ULONG);

#include <poppack.h>

#ifdef __cplusplus
}
#endif

#endif /* _HIDSDI_H */
