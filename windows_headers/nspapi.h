/**
 * @file nspapi.h
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
#ifndef _NSPAPI_H
#define _NSPAPI_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NS_ALL		0

#define NS_SAP		1
#define NS_NDS		2
#define NS_PEER_BROWSE	3

#define NS_TCPIP_LOCAL	10
#define NS_TCPIP_HOSTS	11
#define NS_DNS		12
#define NS_NETBT	13
#define NS_WINS		14

#define NS_NBP		20

#define NS_MS		30
#define NS_STDA		31
#define NS_NTDS		32

#define NS_X500		40
#define NS_NIS		41
#define NS_NISPLUS	42

#define NS_WRQ		50

#define SERVICE_REGISTER	1
#define SERVICE_DEREGISTER	2
#define SERVICE_FLUSH		3
#define SERVICE_FLAG_HARD	0x00000002

#ifndef RC_INVOKED

#if defined (_WINSOCK_H) || defined (_WINSOCK2_H) /* needed for LPSOCKADDR */
#ifndef __CSADDR_T_DEFINED /* also in winsock2.h, but not in winsock.h */
#define __CSADDR_T_DEFINED
typedef struct _SOCKET_ADDRESS {
	LPSOCKADDR lpSockaddr;
	INT iSockaddrLength;
} SOCKET_ADDRESS,*PSOCKET_ADDRESS,*LPSOCKET_ADDRESS;
typedef struct _CSADDR_INFO {
	SOCKET_ADDRESS LocalAddr;
	SOCKET_ADDRESS RemoteAddr;
	INT iSocketType;
	INT iProtocol;
} CSADDR_INFO,*PCSADDR_INFO,*LPCSADDR_INFO;
#endif
#endif

#ifndef __BLOB_T_DEFINED /* also in wtypes.h and winsock2.h */
#define __BLOB_T_DEFINED
typedef struct _BLOB {
	ULONG	cbSize;
	BYTE	*pBlobData;
} BLOB,*PBLOB,*LPBLOB;
#endif

typedef struct _SERVICE_ADDRESS {
	DWORD dwAddressType;
	DWORD dwAddressFlags;
	DWORD dwAddressLength;
	DWORD dwPrincipalLength;
	BYTE *lpAddress;
	BYTE *lpPrincipal;
} SERVICE_ADDRESS;
typedef struct _SERVICE_ADDRESSES {
	DWORD dwAddressCount;
	SERVICE_ADDRESS Addresses[1];
} SERVICE_ADDRESSES, *PSERVICE_ADDRESSES, *LPSERVICE_ADDRESSES;
typedef struct _SERVICE_INFOA {
	LPGUID lpServiceType;
	LPSTR lpServiceName;
	LPSTR lpComment;
	LPSTR lpLocale;
	DWORD dwDisplayHint;
	DWORD dwVersion;
	DWORD dwTime;
	LPSTR lpMachineName;
	LPSERVICE_ADDRESSES lpServiceAddress;
	BLOB ServiceSpecificInfo;
} SERVICE_INFOA, *LPSERVICE_INFOA;
typedef struct _SERVICE_INFOW {
	LPGUID lpServiceType;
	LPWSTR lpServiceName;
	LPWSTR lpComment;
	LPWSTR lpLocale;
	DWORD dwDisplayHint;
	DWORD dwVersion;
	DWORD dwTime;
	LPWSTR lpMachineName;
	LPSERVICE_ADDRESSES lpServiceAddress;
	BLOB ServiceSpecificInfo;
} SERVICE_INFOW, *LPSERVICE_INFOW;

typedef void *LPSERVICE_ASYNC_INFO;
INT WINAPI SetServiceA(DWORD,DWORD,DWORD,LPSERVICE_INFOA,LPSERVICE_ASYNC_INFO,LPDWORD);
INT WINAPI SetServiceW(DWORD,DWORD,DWORD,LPSERVICE_INFOW,LPSERVICE_ASYNC_INFO,LPDWORD);
INT WINAPI GetAddressByNameA(DWORD,LPGUID,LPSTR,LPINT,DWORD,LPSERVICE_ASYNC_INFO,LPVOID,LPDWORD,LPSTR,LPDWORD);
INT WINAPI GetAddressByNameW(DWORD,LPGUID,LPWSTR,LPINT,DWORD,LPSERVICE_ASYNC_INFO,LPVOID,LPDWORD,LPWSTR,LPDWORD);

typedef __AW(SERVICE_INFO) SERVICE_INFO, *LPSERVICE_INFO;
#define _SERVICE_INFO __AW(SERVICE_INFO)
#define SetService __AW(SetService)
#define GetAddressByName __AW(GetAddressByName)

#endif /* RC_INVOKED */

#ifdef __cplusplus
}
#endif

#endif /* _NSPAPI_H */
