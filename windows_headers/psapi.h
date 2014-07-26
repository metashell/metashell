/**
 * @file psapi.h
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
/* Written by Mumit Khan <khan@nanotech.wisc.edu> */
#ifndef _PSAPI_H
#define _PSAPI_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RC_INVOKED

typedef struct _MODULEINFO {
	LPVOID lpBaseOfDll;
	DWORD SizeOfImage;
	LPVOID EntryPoint;
} MODULEINFO,*LPMODULEINFO;

typedef struct _PSAPI_WS_WATCH_INFORMATION {
	LPVOID FaultingPc;
	LPVOID FaultingVa;
} PSAPI_WS_WATCH_INFORMATION,*PPSAPI_WS_WATCH_INFORMATION;

typedef struct _PROCESS_MEMORY_COUNTERS {
	DWORD cb;
	DWORD PageFaultCount;
	DWORD PeakWorkingSetSize;
	DWORD WorkingSetSize;
	DWORD QuotaPeakPagedPoolUsage;
	DWORD QuotaPagedPoolUsage;
	DWORD QuotaPeakNonPagedPoolUsage;
	DWORD QuotaNonPagedPoolUsage;
	DWORD PagefileUsage;
	DWORD PeakPagefileUsage;
} PROCESS_MEMORY_COUNTERS,*PPROCESS_MEMORY_COUNTERS;

typedef struct _PROCESS_MEMORY_COUNTERS_EX {
	DWORD cb;
	DWORD PageFaultCount;
	DWORD PeakWorkingSetSize;
	DWORD WorkingSetSize;
	DWORD QuotaPeakPagedPoolUsage;
	DWORD QuotaPagedPoolUsage;
	DWORD QuotaPeakNonPagedPoolUsage;
	DWORD QuotaNonPagedPoolUsage;
	DWORD PagefileUsage;
	DWORD PeakPagefileUsage;
	DWORD PrivateUsage;
} PROCESS_MEMORY_COUNTERS_EX,*PPROCESS_MEMORY_COUNTERS_EX;

typedef struct _PERFORMANCE_INFORMATION {
  DWORD cb;
  DWORD CommitTotal;
  DWORD CommitLimit;
  DWORD CommitPeak;
  DWORD PhysicalTotal;
  DWORD PhysicalAvailable;
  DWORD SystemCache;
  DWORD KernelTotal;
  DWORD KernelPaged;
  DWORD KernelNonpaged;
  DWORD PageSize;
  DWORD HandleCount;
  DWORD ProcessCount;
  DWORD ThreadCount;
} PERFORMANCE_INFORMATION, *PPERFORMANCE_INFORMATION;

typedef union _PSAPI_WORKING_SET_BLOCK {
  ULONG_PTR Flags;
  struct {
    ULONG_PTR Protection  :5;
    ULONG_PTR ShareCount  :3;
    ULONG_PTR Shared  :1;
    ULONG_PTR Reserved  :3;
    ULONG_PTR VirtualPage  :20;
  } ;
} PSAPI_WORKING_SET_BLOCK, *PPSAPI_WORKING_SET_BLOCK;

typedef struct _PSAPI_WORKING_SET_INFORMATION {
  ULONG_PTR               NumberOfEntries;
  PSAPI_WORKING_SET_BLOCK WorkingSetInfo[1];
} PSAPI_WORKING_SET_INFORMATION, *PPSAPI_WORKING_SET_INFORMATION;

/* Grouped by application,not in alphabetical order. */
BOOL WINAPI EnumProcesses(DWORD *,DWORD,DWORD *);
BOOL WINAPI EnumProcessModules(HANDLE,HMODULE *,DWORD,LPDWORD);
DWORD WINAPI GetModuleBaseNameA(HANDLE,HMODULE,LPSTR,DWORD);
DWORD WINAPI GetModuleBaseNameW(HANDLE,HMODULE,LPWSTR,DWORD);
DWORD WINAPI GetModuleFileNameExA(HANDLE,HMODULE,LPSTR,DWORD);
DWORD WINAPI GetModuleFileNameExW(HANDLE,HMODULE,LPWSTR,DWORD);
BOOL WINAPI GetModuleInformation(HANDLE,HMODULE,LPMODULEINFO,DWORD);
BOOL WINAPI EmptyWorkingSet(HANDLE);
BOOL WINAPI QueryWorkingSet(HANDLE,PVOID,DWORD);
BOOL WINAPI InitializeProcessForWsWatch(HANDLE);
BOOL WINAPI GetWsChanges(HANDLE,PPSAPI_WS_WATCH_INFORMATION,DWORD);
DWORD WINAPI GetMappedFileNameW(HANDLE,LPVOID,LPWSTR,DWORD);
DWORD WINAPI GetMappedFileNameA(HANDLE,LPVOID,LPSTR,DWORD);
BOOL WINAPI EnumDeviceDrivers(LPVOID *,DWORD,LPDWORD);
DWORD WINAPI GetDeviceDriverBaseNameA(LPVOID,LPSTR,DWORD);
DWORD WINAPI GetDeviceDriverBaseNameW(LPVOID,LPWSTR,DWORD);
DWORD WINAPI GetDeviceDriverFileNameA(LPVOID,LPSTR,DWORD);
DWORD WINAPI GetDeviceDriverFileNameW(LPVOID,LPWSTR,DWORD);
BOOL WINAPI GetProcessMemoryInfo(HANDLE,PPROCESS_MEMORY_COUNTERS,DWORD);
BOOL WINAPI GetPerformanceInfo(PPERFORMANCE_INFORMATION,DWORD);

#define GetModuleBaseName __AW(GetModuleBaseName)
#define GetModuleFileNameEx __AW(GetModuleFileNameEx)
#define GetMappedFileName __AW(GetMappedFileName)
#define GetDeviceDriverBaseName __AW(GetDeviceDriverBaseName)
#define GetDeviceDriverFileName __AW(GetDeviceDriverFileName)

#if (_WIN32_WINNT >= _WIN32_WINNT_WINXP)
DWORD WINAPI GetProcessImageFileNameA(HANDLE,LPSTR,DWORD);
DWORD WINAPI GetProcessImageFileNameW(HANDLE,LPWSTR,DWORD);
#define GetProcessImageFileName __AW(GetProcessImageFileName)
#endif /* _WIN32_WINNT >= _WIN32_WINNT_WINXP */

#endif /* not RC_INVOKED */

#ifdef __cplusplus
}
#endif

#endif /* _PSAPI_H */

