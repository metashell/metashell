/**
 * @file lmconfig.h
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
#ifndef _LMCONFIG_H
#define _LMCONFIG_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#define REVISED_CONFIG_APIS
typedef struct _CONFIG_INFO_0 {
	LPWSTR cfgi0_key;
	LPWSTR cfgi0_data;
} CONFIG_INFO_0,*PCONFIG_INFO_0,*LPCONFIG_INFO_0;
NET_API_STATUS WINAPI NetConfigGet(LPCWSTR,LPCWSTR,LPCWSTR,PBYTE*);
NET_API_STATUS WINAPI NetConfigGetAll(LPCWSTR,LPCWSTR,PBYTE*);
NET_API_STATUS WINAPI NetConfigSet(LPCWSTR,LPCWSTR,LPCWSTR,DWORD,DWORD,PBYTE,DWORD);

#ifdef __cplusplus
}
#endif

#endif 
