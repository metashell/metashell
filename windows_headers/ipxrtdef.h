/**
 * @file ipxrtdef.h
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
#ifndef _IPXRTDEF_H
#define _IPXRTDEF_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Routing and Remote Access Services
 */

#include <stm.h>
#include <ipxconst.h>

#ifdef __cplusplus
extern "C" {
#endif

/*--- Router Management Reference - Router Information Enumeration Types */
#define IPX_INTERFACE_INFO_TYPE 0x0001
#define IPX_STATIC_ROUTE_INFO_TYPE 0x0002
#define IPX_STATIC_SERVICE_INFO_TYPE 0x0003
#define IPX_SERVICE_FILTER_INFO_TYPE 0x0004
#define IPX_ROUTE_FILTER_INFO_TYPE 0x0005
#define IPX_IN_TRAFFIC_FILTER_INFO_TYPE 0x0006
#define IPX_ADAPTER_INFO_TYPE 0x0007
#define IPXWAN_INTERFACE_INFO_TYPE 0x0008
#define IPX_GLOBAL_INFO_TYPE 0x0009
#define IPX_STATIC_NETBIOS_NAME_INFO_TYPE 0x000A
#define IPX_IN_TRAFFIC_FILTER_GLOBAL_INFO_TYPE 0x000B
#define IPX_OUT_TRAFFIC_FILTER_INFO_TYPE 0x000C
#define IPX_OUT_TRAFFIC_FILTER_GLOBAL_INFO_TYPE 0x000D

/*--- Router Management Reference - Router Information Structures - IPX Information Structures */
#if (_WIN32_WINNT >= _WIN32_WINNT_WINXP)
typedef struct _IPX_IF_INFO {
	ULONG AdminState;
	ULONG NetbiosAccept;
	ULONG NetbiosDeliver;
} IPX_IF_INFO,*PIPX_IF_INFO;
typedef IPX_SERVER_ENTRY IPX_STATIC_SERVICE_INFO,*PIPX_STATIC_SERVICE_INFO;
typedef struct _IPXWAN_IF_INFO {
	ULONG AdminState;
} IPXWAN_IF_INFO,*PIPXWAN_IF_INFO;
#endif

#ifdef __cplusplus
}
#endif
#endif
