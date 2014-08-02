/**
 * @file errorrep.h
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
#ifndef _ERRORREP_H
#define _ERRORREP_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (_WIN32_WINNT >= _WIN32_WINNT_WINXP)
typedef enum {
	frrvErr = 3,
	frrvErrNoDW = 4,
	frrvErrTimeout = 5,
	frrvLaunchDebugger = 6,
	frrvOk = 0,
	frrvOkHeadless = 7,
	frrvOkManifest = 1,
	frrvOkQueued = 2
} EFaultRepRetVal;
BOOL WINAPI AddERExcludedApplicationA(LPCSTR);
BOOL WINAPI AddERExcludedApplicationW(LPCWSTR);
EFaultRepRetVal WINAPI ReportFault(LPEXCEPTION_POINTERS,DWORD);
#define AddERExcludedApplication __AW(AddERExcludedApplication)
#endif

#ifdef __cplusplus
}
#endif
#endif
