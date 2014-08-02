/**
 * @file lmat.h
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
#ifndef _LMAT_H
#define _LMAT_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#define JOB_RUN_PERIODICALLY	1
#define JOB_EXEC_ERROR	2
#define JOB_RUNS_TODAY	4
#define JOB_ADD_CURRENT_DATE	8
#define JOB_NONINTERACTIVE	16
#define JOB_INPUT_FLAGS	(JOB_RUN_PERIODICALLY|JOB_ADD_CURRENT_DATE|JOB_NONINTERACTIVE)
#define JOB_OUTPUT_FLAGS (JOB_RUN_PERIODICALLY|JOB_EXEC_ERROR|JOB_RUNS_TODAY|JOB_NONINTERACTIVE)
typedef struct _AT_ENUM {
	DWORD JobId;
	DWORD JobTime;
	DWORD DaysOfMonth;
	UCHAR DaysOfWeek;
	UCHAR Flags;
	LPWSTR Command;
} AT_ENUM,*PAT_ENUM,*LPAT_ENUM;
typedef struct _AT_INFO {
	DWORD JobTime;
	DWORD DaysOfMonth;
	UCHAR DaysOfWeek;
	UCHAR Flags;
	LPWSTR Command;
} AT_INFO,*PAT_INFO,*LPAT_INFO;
NET_API_STATUS WINAPI NetScheduleJobAdd(LPWSTR,PBYTE,LPDWORD);
NET_API_STATUS WINAPI NetScheduleJobDel(LPWSTR,DWORD,DWORD);
NET_API_STATUS WINAPI NetScheduleJobEnum(LPWSTR,PBYTE*,DWORD,PDWORD,PDWORD,PDWORD);
NET_API_STATUS WINAPI NetScheduleJobGetInfo(LPWSTR,DWORD,PBYTE*);

#ifdef __cplusplus
}
#endif

#endif
