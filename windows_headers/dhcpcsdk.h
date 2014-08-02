/**
 * @file dhcpcsdk.h
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
#ifndef _DHCPCDSK_H
#define _DHCPCDSK_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (_WIN32_WINNT >= _WIN32_WINNT_WIN2K)
#define DHCPCAPI_REGISTER_HANDLE_EVENT 0x00000001
#define DHCPCAPI_REQUEST_PERSISTENT  0x00000001
#define DHCPCAPI_REQUEST_SYNCHRONOUS 0x00000002
typedef struct _DHCPAPI_CLASSID {
	ULONG Flags;
	LPBYTE Data;
	ULONG nBytesData;
} DHCPCAPI_CLASSID,*PDHCPCAPI_CLASSID,*LPDHCPCAPI_CLASSID;
typedef struct _DHCPAPI_PARAMS {
	ULONG Flags;
	ULONG OptionId;
	BOOL IsVendor;
	LPBYTE Data;
	DWORD nBytesData;
} DHCPAPI_PARAMS,*PDHCPAPI_PARAMS,*LPDHCPAPI_PARAMS;
typedef struct _DHCPAPI_PARAMS_ARRAY {
	ULONG nParams;
	LPDHCPAPI_PARAMS Params;
} DHCPCAPI_PARAMS_ARRAY,*PDHCPCAPI_PARAMS_ARRAY,*LPDHCPCAPI_PARAMS_ARRAY;
VOID WINAPI DhcpCApiCleanup(void);
DWORD WINAPI DhcpCApiInitialize(LPDWORD);
DWORD WINAPI DhcpDeRegisterParamChange(DWORD,LPVOID,LPVOID);
DWORD WINAPI DhcpRegisterParamChange(DWORD,LPVOID,PWSTR,LPDHCPCAPI_CLASSID,DHCPCAPI_PARAMS_ARRAY,LPVOID);
DWORD WINAPI DhcpRemoveDNSRegistrations(void);
DWORD WINAPI DhcpUndoRequestParams(DWORD,LPVOID,LPWSTR,LPWSTR);
#endif /* (_WIN32_WINNT >= _WIN32_WINNT_WIN2K) */

#ifdef __cplusplus
}
#endif
#endif
