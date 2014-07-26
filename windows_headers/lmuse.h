/**
 * @file lmuse.h
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
#ifndef _LMUSE_H
#define _LMUSE_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <lmuseflg.h>
#define USE_LOCAL_PARMNUM 1
#define USE_REMOTE_PARMNUM 2
#define USE_PASSWORD_PARMNUM 3
#define USE_ASGTYPE_PARMNUM 4
#define USE_USERNAME_PARMNUM 5
#define USE_DOMAINNAME_PARMNUM 6
#define USE_OK 0
#define USE_PAUSED 1
#define USE_SESSLOST 2
#define USE_DISCONN 2
#define USE_NETERR 3
#define USE_CONN 4
#define USE_RECONN 5
#define USE_WILDCARD ((DWORD)-1)
#define USE_DISKDEV 0
#define USE_SPOOLDEV 1
#define USE_CHARDEV 2
#define USE_IPC 3
typedef struct _USE_INFO_0 {
	LPWSTR ui0_local;
	LPWSTR ui0_remote;
}USE_INFO_0,*PUSE_INFO_0,*LPUSE_INFO_0;
typedef struct _USE_INFO_1 {
	LPWSTR ui1_local;
	LPWSTR ui1_remote;
	LPWSTR ui1_password;
	DWORD ui1_status;
	DWORD ui1_asg_type;
	DWORD ui1_refcount;
	DWORD ui1_usecount;
}USE_INFO_1,*PUSE_INFO_1,*LPUSE_INFO_1;
typedef struct _USE_INFO_2 {
	LPWSTR ui2_local;
	LPWSTR ui2_remote;
	LPWSTR ui2_password;
	DWORD ui2_status;
	DWORD ui2_asg_type;
	DWORD ui2_refcount;
	DWORD ui2_usecount;
	LPWSTR ui2_username;
	LPWSTR ui2_domainname;
}USE_INFO_2,*PUSE_INFO_2,*LPUSE_INFO_2;
NET_API_STATUS WINAPI NetUseAdd(LPWSTR,DWORD,PBYTE,PDWORD);
NET_API_STATUS WINAPI NetUseDel(LPWSTR,LPWSTR,DWORD);
NET_API_STATUS WINAPI NetUseEnum(LPWSTR,DWORD,PBYTE*,DWORD,PDWORD,PDWORD,PDWORD);
NET_API_STATUS WINAPI NetUseGetInfo(LPWSTR,LPWSTR,DWORD,PBYTE*);

#ifdef __cplusplus
}
#endif

#endif 
