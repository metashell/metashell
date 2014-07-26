/**
 * @file ndiswan.h
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
#ifndef __NDISWAN_H
#define __NDISWAN_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Definitions for NDIS WAN miniport drivers
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ndis.h"

#define NDIS_USE_WAN_WRAPPER         			0x00000001

#define NDIS_STATUS_TAPI_INDICATION 			((NDIS_STATUS)0x40010080L)

/* NDIS_WAN_INFO.FramingBits constants */
#define RAS_FRAMING                     0x00000001
#define RAS_COMPRESSION                 0x00000002

#define ARAP_V1_FRAMING                 0x00000004
#define ARAP_V2_FRAMING                 0x00000008
#define ARAP_FRAMING                    (ARAP_V1_FRAMING | ARAP_V2_FRAMING)

#define PPP_MULTILINK_FRAMING           0x00000010
#define PPP_SHORT_SEQUENCE_HDR_FORMAT   0x00000020
#define PPP_MC_MULTILINK_FRAMING        0x00000040

#define PPP_FRAMING                     0x00000100
#define PPP_COMPRESS_ADDRESS_CONTROL    0x00000200
#define PPP_COMPRESS_PROTOCOL_FIELD     0x00000400
#define PPP_ACCM_SUPPORTED              0x00000800

#define SLIP_FRAMING                    0x00001000
#define SLIP_VJ_COMPRESSION             0x00002000
#define SLIP_VJ_AUTODETECT              0x00004000

#define MEDIA_NRZ_ENCODING              0x00010000
#define MEDIA_NRZI_ENCODING             0x00020000
#define MEDIA_NLPID                     0x00040000

#define RFC_1356_FRAMING                0x00100000
#define RFC_1483_FRAMING                0x00200000
#define RFC_1490_FRAMING                0x00400000
#define LLC_ENCAPSULATION               0x00800000

#define SHIVA_FRAMING                   0x01000000
#define NBF_PRESERVE_MAC_ADDRESS        0x01000000

#define PASS_THROUGH_MODE               0x10000000
#define RAW_PASS_THROUGH_MODE           0x20000000

#define TAPI_PROVIDER                   0x80000000

typedef struct _NDIS_WAN_INFO {
  /*OUT*/ ULONG  MaxFrameSize; 
  /*OUT*/ ULONG  MaxTransmit; 
  /*OUT*/ ULONG  HeaderPadding; 
  /*OUT*/ ULONG  TailPadding; 
  /*OUT*/ ULONG  Endpoints; 
  /*OUT*/ UINT  MemoryFlags; 
  /*OUT*/ NDIS_PHYSICAL_ADDRESS  HighestAcceptableAddress; 
  /*OUT*/ ULONG  FramingBits; 
  /*OUT*/ ULONG  DesiredACCM; 
} NDIS_WAN_INFO, *PNDIS_WAN_INFO;

typedef struct _NDIS_WAN_GET_LINK_INFO { 
  /*IN*/ NDIS_HANDLE  NdisLinkHandle; 
  /*OUT*/ ULONG  MaxSendFrameSize; 
  /*OUT*/ ULONG  MaxRecvFrameSize; 
  /*OUT*/ ULONG  HeaderPadding; 
  /*OUT*/ ULONG  TailPadding; 
  /*OUT*/ ULONG  SendFramingBits; 
  /*OUT*/ ULONG  RecvFramingBits; 
  /*OUT*/ ULONG  SendCompressionBits; 
  /*OUT*/ ULONG  RecvCompressionBits; 
  /*OUT*/ ULONG  SendACCM; 
  /*OUT*/ ULONG  RecvACCM; 
} NDIS_WAN_GET_LINK_INFO, *PNDIS_WAN_GET_LINK_INFO;

typedef struct _NDIS_WAN_SET_LINK_INFO { 
  /*IN*/ NDIS_HANDLE  NdisLinkHandle; 
  /*IN*/ ULONG  MaxSendFrameSize; 
  /*IN*/ ULONG  MaxRecvFrameSize; 
  /*IN*/ ULONG  HeaderPadding; 
  /*IN*/ ULONG  TailPadding; 
  /*IN*/ ULONG  SendFramingBits; 
  /*IN*/ ULONG  RecvFramingBits; 
  /*IN*/ ULONG  SendCompressionBits; 
  /*IN*/ ULONG  RecvCompressionBits; 
  /*IN*/ ULONG  SendACCM; 
  /*IN*/ ULONG  RecvACCM; 
} NDIS_WAN_SET_LINK_INFO, *PNDIS_WAN_SET_LINK_INFO; 

/* NDIS_WAN_COMPRESS_INFO.MSCompType constants */
#define NDISWAN_COMPRESSION            		0x00000001
#define NDISWAN_ENCRYPTION              	0x00000010
#define NDISWAN_40_ENCRYPTION           	0x00000020
#define NDISWAN_128_ENCRYPTION          	0x00000040
#define NDISWAN_56_ENCRYPTION           	0x00000080
#define NDISWAN_HISTORY_LESS            	0x01000000

/* NDIS_WAN_COMPRESS_INFO.CompType constants */
#define COMPTYPE_OUI											0
#define COMPTYPE_NT31RAS									254
#define COMPTYPE_NONE											255

typedef struct _NDIS_WAN_COMPRESS_INFO {
	UCHAR  SessionKey[8];
	ULONG  MSCompType;
	UCHAR  CompType;
	USHORT  CompLength;
	_ANONYMOUS_UNION union {
		struct {
			UCHAR  CompOUI[3];
			UCHAR  CompSubType;
			UCHAR  CompValues[32];
		} Proprietary;
		struct {
			UCHAR  CompValues[32];
		} Public;
	} DUMMYUNIONNAME;
} NDIS_WAN_COMPRESS_INFO;

typedef NDIS_WAN_COMPRESS_INFO *PNDIS_WAN_COMPRESS_INFO;

typedef struct _NDIS_WAN_GET_COMP_INFO { 
  /*IN*/ NDIS_HANDLE  NdisLinkHandle; 
  /*OUT*/ NDIS_WAN_COMPRESS_INFO  SendCapabilities; 
  /*OUT*/ NDIS_WAN_COMPRESS_INFO  RecvCapabilities; 
} NDIS_WAN_GET_COMP_INFO, *PNDIS_WAN_GET_COMP_INFO;

typedef struct _NDIS_WAN_SET_COMP_INFO {
  /*IN*/ NDIS_HANDLE  NdisLinkHandle;
  /*IN*/ NDIS_WAN_COMPRESS_INFO  SendCapabilities;
  /*IN*/ NDIS_WAN_COMPRESS_INFO  RecvCapabilities;
} NDIS_WAN_SET_COMP_INFO, *PNDIS_WAN_SET_COMP_INFO;

typedef struct _NDIS_WAN_GET_STATS_INFO {
  /*IN*/ NDIS_HANDLE  NdisLinkHandle;
  /*OUT*/ ULONG  BytesSent;
  /*OUT*/ ULONG  BytesRcvd;
  /*OUT*/ ULONG  FramesSent;
  /*OUT*/ ULONG  FramesRcvd;
  /*OUT*/ ULONG  CRCErrors;
  /*OUT*/ ULONG  TimeoutErrors;
  /*OUT*/ ULONG  AlignmentErrors;
  /*OUT*/ ULONG  SerialOverrunErrors;
  /*OUT*/ ULONG  FramingErrors;
  /*OUT*/ ULONG  BufferOverrunErrors;
  /*OUT*/ ULONG  BytesTransmittedUncompressed;
  /*OUT*/ ULONG  BytesReceivedUncompressed;
  /*OUT*/ ULONG  BytesTransmittedCompressed;
  /*OUT*/ ULONG  BytesReceivedCompressed;
} NDIS_WAN_GET_STATS_INFO, *PNDIS_WAN_GET_STATS_INFO;

#define NdisMWanInitializeWrapper(NdisWrapperHandle,	\
                                  SystemSpecific1,		\
                                  SystemSpecific2,		\
                                  SystemSpecific3)		\
{                                                     \
	NdisMInitializeWrapper(NdisWrapperHandle,          	\
                         SystemSpecific1,          		\
                         SystemSpecific2,          		\
                         SystemSpecific3);         		\
}

typedef struct _NDIS_WAN_CO_INFO {
  /*OUT*/ ULONG  MaxFrameSize;
  /*OUT*/ ULONG  MaxSendWindow;
  /*OUT*/ ULONG  FramingBits;
  /*OUT*/ ULONG  DesiredACCM;
} NDIS_WAN_CO_INFO, *PNDIS_WAN_CO_INFO;

typedef struct _NDIS_WAN_CO_GET_LINK_INFO {
  /*OUT*/ ULONG  MaxSendFrameSize;
  /*OUT*/ ULONG  MaxRecvFrameSize;
  /*OUT*/ ULONG  SendFramingBits;
  /*OUT*/ ULONG  RecvFramingBits;
  /*OUT*/ ULONG  SendCompressionBits;
  /*OUT*/ ULONG  RecvCompressionBits;
  /*OUT*/ ULONG  SendACCM;
  /*OUT*/ ULONG  RecvACCM;
} NDIS_WAN_CO_GET_LINK_INFO, *PNDIS_WAN_CO_GET_LINK_INFO;

typedef struct _NDIS_WAN_CO_SET_LINK_INFO {
  /*IN*/ ULONG  MaxSendFrameSize;
  /*IN*/ ULONG  MaxRecvFrameSize;
  /*IN*/ ULONG  SendFramingBits;
  /*IN*/ ULONG  RecvFramingBits;
  /*IN*/ ULONG  SendCompressionBits;
  /*IN*/ ULONG  RecvCompressionBits;
  /*IN*/ ULONG  SendACCM;
  /*IN*/ ULONG  RecvACCM;
} NDIS_WAN_CO_SET_LINK_INFO, *PNDIS_WAN_CO_SET_LINK_INFO;

typedef struct _NDIS_WAN_CO_GET_COMP_INFO { 
  /*OUT*/ NDIS_WAN_COMPRESS_INFO  SendCapabilities; 
  /*OUT*/ NDIS_WAN_COMPRESS_INFO  RecvCapabilities; 
} NDIS_WAN_CO_GET_COMP_INFO, *PNDIS_WAN_CO_GET_COMP_INFO; 

typedef struct _NDIS_WAN_CO_SET_COMP_INFO {
  /*IN*/ NDIS_WAN_COMPRESS_INFO  SendCapabilities;
  /*IN*/ NDIS_WAN_COMPRESS_INFO  RecvCapabilities;
} NDIS_WAN_CO_SET_COMP_INFO, *PNDIS_WAN_CO_SET_COMP_INFO;

typedef struct _NDIS_WAN_CO_GET_STATS_INFO {
  /*OUT*/ ULONG  BytesSent;
  /*OUT*/ ULONG  BytesRcvd;
  /*OUT*/ ULONG  FramesSent;
  /*OUT*/ ULONG  FramesRcvd;
  /*OUT*/ ULONG  CRCErrors;
  /*OUT*/ ULONG  TimeoutErrors;
  /*OUT*/ ULONG  AlignmentErrors;
  /*OUT*/ ULONG  SerialOverrunErrors;
  /*OUT*/ ULONG  FramingErrors;
  /*OUT*/ ULONG  BufferOverrunErrors;
  /*OUT*/ ULONG  BytesTransmittedUncompressed;
  /*OUT*/ ULONG  BytesReceivedUncompressed;
  /*OUT*/ ULONG  BytesTransmittedCompressed;
  /*OUT*/ ULONG  BytesReceivedCompressed;
} NDIS_WAN_CO_GET_STATS_INFO, *PNDIS_WAN_CO_GET_STATS_INFO;

#ifdef __cplusplus
}
#endif

#endif /* __NDISWAN_H */
