/**
 * @file fltdefs.h
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
#ifndef _FLTDEFS_H
#define _FLTDEFS_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Routing and Remote Access Services
 */

#ifdef __cplusplus
extern "C" {
#endif

/*--- Packet Filtering Reference - Packet Filtering Enumerated Types */
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN2K)
typedef enum _GlobalFilter {
	GF_FRAGMENTS = 2,
	GF_STRONGHOST = 8,
	GF_FRAGCACHE = 9
} GLOBAL_FILTER,*PGLOBAL_FILTER;
typedef enum _PfAddresType {
	PF_IPV4,
	PF_IPV6
} PFADDRESSTYPE,*PPFADDRESSTYPE;
typedef enum _PfForwardAction {
	PF_ACTION_FORWARD = 0,
	PF_ACTION_DROP
} PFFORWARD_ACTION,*PPFFORWARD_ACTION;
typedef enum _PfFrameType {
	PFFT_FILTER = 1,
	PFFT_FRAG = 2,
	PFFT_SPOOF = 3
} PFFRAMETYPE,*PPFFRAMETYPE;
#endif /* (_WIN32_WINNT >= _WIN32_WINNT_WIN2K) */

#ifdef __cplusplus
}
#endif
#endif
