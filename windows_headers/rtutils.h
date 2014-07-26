/**
 * @file rtutils.h
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
#ifndef _RTUTILS_H
#define _RTUTILS_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

/*--- Tracing Reference */
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN2K)
DWORD WINAPI TraceDeregisterA(DWORD);
DWORD WINAPI TraceDeregisterW(DWORD);
DWORD WINAPI TraceDeregisterExA(DWORD,DWORD);
DWORD WINAPI TraceDeregisterExW(DWORD,DWORD);
#define TRACE_NO_SYNCH 0x00000004
#define TraceDumpA(a,b,c,d,e,f) TraceDumpExA((a),0,(b),(c),(d),(e),(f))
#define TraceDumpW(a,b,c,d,e,f) TraceDumpExW((a),0,(b),(c),(d),(e),(f))
DWORD WINAPI TraceDumpExA(DWORD,DWORD,LPBYTE,DWORD,DWORD,BOOL,LPCSTR);
DWORD WINAPI TraceDumpExW(DWORD,DWORD,LPBYTE,DWORD,DWORD,BOOL,LPCWSTR);
DWORD WINAPI TracePrintfA(DWORD,LPCSTR,...);
DWORD WINAPI TracePrintfW(DWORD,LPCWSTR,...);
DWORD WINAPI TracePrintfExA(DWORD,DWORD,LPCSTR,...);
DWORD WINAPI TracePrintfExW(DWORD,DWORD,LPCWSTR,...);
#define TracePutsA(a,b) TracePutsExA((a),0,(b))
#define TracePutsW(a,b) TracePutsExW((a),0,(b))
DWORD WINAPI TracePutsExA(DWORD,DWORD,LPCSTR);
DWORD WINAPI TracePutsExW(DWORD,DWORD,LPCWSTR);
#define TraceRegisterA(a) TraceRegisterExA((a),0)
#define TraceRegisterW(a) TraceRegisterExW((a),0)
DWORD WINAPI TraceRegisterExA(LPCSTR,DWORD);
DWORD WINAPI TraceRegisterExW(LPCWSTR,DWORD);
#define TRACE_USE_FILE 0x00000001
#define TRACE_USE_CONSOLE 0x00000002
#define INVALID_TRACEID 0xFFFFFFFF
#define TraceVprintfA(a,b,c) TraceVprintfExA((a),0,(b),(c))
#define TraceVprintfW(a,b,c) TraceVprintfExW((a),0,(b),(c))
DWORD WINAPI TraceVprintfExA(DWORD,DWORD,LPCSTR,va_list);
DWORD WINAPI TraceVprintfExW(DWORD,DWORD,LPCWSTR,va_list);
#define TRACE_NO_STDINFO 0x00000001
#define TRACE_USE_MASK 0x00000002
#define TRACE_USE_MSEC 0x00000004

#define TraceDeregister __AW(TraceDeregister)
#define TraceDeregisterEx __AW(TraceDeregisterEx)
#define TraceDump __AW(TraceDump)
#define TraceDumpEx __AW(TraceDumpEx)
#define TracePrintf __AW(TracePrintf)
#define TracePrintfEx __AW(TracePrintfEx)
#define TracePuts __AW(TracePuts)
#define TracePutsEx __AW(TracePutsEx)
#define TraceRegister __AW(TraceRegister)
#define TraceRegisterEx __AW(TraceRegisterEx)
#define TraceVprintf __AW(TraceVprintf)
#define TraceVprintfEx __AW(TraceVprintfEx)

#endif /* (_WIN32_WINNT >= _WIN32_WINNT_WIN2K) */

#ifdef __cplusplus
}
#endif

#endif
