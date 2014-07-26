/**
 * @file ipxtfflt.h
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
#ifndef _IPXTFFLT_H
#define _IPXTFFLT_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Routing and Remote Access Services
 */

#ifdef __cplusplus
extern "C" {
#endif

/*--- Router Management Reference - Router Information Structures - IPX Information Structures */
typedef struct _IPX_TRAFFIC_FILTER_INFO {
  ULONG FilterDefinition;
  UCHAR DestinationNetwork[4];
  UCHAR DestinationNetworkMask[4];
  UCHAR DestinationNode[6];
  UCHAR DestinationSocket[2];
  UCHAR SourceNetwork[4];
  UCHAR SourceNetworkMask[4];
  UCHAR SourceNode[6];
  UCHAR SourceSocket[2];
  ULONG PacketType;
} IPX_TRAFFIC_FILTER_INFO, *PIPX_TRAFFIC_FILTER_INFO;
typedef struct _IPX_TRAFFIC_FILTER_GLOBAL_INFO {
  ULONG FilterAction;
} IPX_TRAFFIC_FILTER_GLOBAL_INFO, *PIPX_TRAFFIC_FILTER_GLOBAL_INFO;

#ifdef __cplusplus
}
#endif
#endif
