/**
 * @file tdiinfo.h
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
#ifndef __TDIINFO_H
#define __TDIINFO_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * TDI set and query information interface
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"

typedef struct TDIEntityID {
  ULONG  tei_entity;
  ULONG  tei_instance;
} TDIEntityID;

#define	MAX_TDI_ENTITIES                  4096
#define INVALID_ENTITY_INSTANCE           -1
#define	GENERIC_ENTITY				            0
#define	ENTITY_LIST_ID				            0
#define	ENTITY_TYPE_ID				            1

#define	AT_ENTITY					                0x280
#define	CL_NL_ENTITY				              0x301
#define	CL_TL_ENTITY				              0x401
#define	CO_NL_ENTITY				              0x300
#define	CO_TL_ENTITY				              0x400
#define	ER_ENTITY					                0x380
#define	IF_ENTITY					                0x200

#define	AT_ARP						                0x280
#define	AT_NULL						                0x282
#define	CL_TL_NBF					                0x401
#define	CL_TL_UDP					                0x403
#define	CL_NL_IPX					                0x301
#define	CL_NL_IP					                0x303
#define	CO_TL_NBF					                0x400
#define	CO_TL_SPX					                0x402
#define	CO_TL_TCP					                0x404
#define	CO_TL_SPP					                0x406
#define	ER_ICMP						                0x380
#define	IF_GENERIC					              0x200
#define	IF_MIB						                0x202

/* TDIObjectID.toi_class constants */
#define	INFO_CLASS_GENERIC			          0x100
#define	INFO_CLASS_PROTOCOL			          0x200
#define	INFO_CLASS_IMPLEMENTATION	        0x300

/* TDIObjectID.toi_type constants */
#define	INFO_TYPE_PROVIDER			          0x100
#define	INFO_TYPE_ADDRESS_OBJECT	        0x200
#define	INFO_TYPE_CONNECTION		          0x300

typedef struct _TDIObjectID {
	TDIEntityID  toi_entity;
	ULONG  toi_class;
	ULONG  toi_type;
	ULONG  toi_id;
} TDIObjectID;

#define	CONTEXT_SIZE				              16

typedef struct _TCP_REQUEST_QUERY_INFORMATION_EX {
  TDIObjectID  ID;
  ULONG_PTR  Context[CONTEXT_SIZE / sizeof(ULONG_PTR)];
} TCP_REQUEST_QUERY_INFORMATION_EX, *PTCP_REQUEST_QUERY_INFORMATION_EX;

#if defined(_WIN64)
typedef struct _TCP_REQUEST_QUERY_INFORMATION_EX32 {
  TDIObjectID  ID;
  ULONG32  Context[CONTEXT_SIZE / sizeof(ULONG32)];
} TCP_REQUEST_QUERY_INFORMATION_EX32, *PTCP_REQUEST_QUERY_INFORMATION_EX32;
#endif /* _WIN64 */

typedef struct _TCP_REQUEST_SET_INFORMATION_EX {
  TDIObjectID  ID;
  UINT  BufferSize;
  UCHAR  Buffer[1];
} TCP_REQUEST_SET_INFORMATION_EX, *PTCP_REQUEST_SET_INFORMATION_EX;

#ifdef __cplusplus
}
#endif

#endif /* __TDIINFO_H */
