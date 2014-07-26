/**
 * @file sddl.h
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
#ifndef _SDDL_H
#define _SDDL_H
#pragma GCC system_header
#include <_mingw.h>

#ifndef WINADVAPI
#define WINADVAPI
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if (WINVER >= _WIN32_WINNT_WIN2K)
WINADVAPI BOOL WINAPI ConvertSidToStringSidA(PSID Sid, LPSTR* StringSid);
WINADVAPI BOOL WINAPI ConvertSidToStringSidW(PSID Sid, LPWSTR* StringSid);
WINADVAPI BOOL WINAPI ConvertStringSidToSidA(LPSTR StringSid, PSID *Sid);
WINADVAPI BOOL WINAPI ConvertStringSidToSidW(LPWSTR StringSid, PSID *Sid);
#endif /* (WINVER >= _WIN32_WINNT_WIN2K) */

#define ConvertSidToStringSid __AW(ConvertSidToStringSid)
#define ConvertStringSidToSid __AW(ConvertStringSidToSid)

#ifdef __cplusplus
}
#endif

#endif /* _SDDL_H */
