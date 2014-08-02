/**
 * @file lmmsg.h
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
#ifndef _LMMSG_H
#define _LMMSG_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MSGNAME_NOT_FORWARDED 0
#define MSGNAME_FORWARDED_TO 4
#define MSGNAME_FORWARDED_FROM 16
typedef struct _MSG_INFO_0 { LPWSTR msgi0_name; }MSG_INFO_0,*PMSG_INFO_0,*LPMSG_INFO_0;
typedef struct _MSG_INFO_1 {
	LPWSTR msgi1_name;
	DWORD msgi1_forward_flag;
	LPWSTR msgi1_forward;
}MSG_INFO_1,*PMSG_INFO_1,*LPMSG_INFO_1;
NET_API_STATUS WINAPI NetMessageBufferSend(LPCWSTR,LPCWSTR,LPCWSTR,PBYTE,DWORD);
NET_API_STATUS WINAPI NetMessageNameAdd(LPCWSTR,LPCWSTR);
NET_API_STATUS WINAPI NetMessageNameDel(LPCWSTR,LPCWSTR);
NET_API_STATUS WINAPI NetMessageNameEnum(LPCWSTR,DWORD,PBYTE*,DWORD,PDWORD,PDWORD,PDWORD);
NET_API_STATUS WINAPI NetMessageNameGetInfo(LPCWSTR,LPCWSTR,DWORD,PBYTE*);

#ifdef __cplusplus
}
#endif

#endif 
