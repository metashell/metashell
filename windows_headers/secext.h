/**
 * @file secext.h
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
#ifndef _SECEXT_H
#define _SECEXT_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RC_INVOKED
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN2K)
typedef enum 
{
  NameUnknown = 0, 
  NameFullyQualifiedDN = 1, 
  NameSamCompatible = 2, 
  NameDisplay = 3, 
  NameUniqueId = 6, 
  NameCanonical = 7, 
  NameUserPrincipal = 8, 
  NameCanonicalEx = 9, 
  NameServicePrincipal = 10, 
  NameDnsDomain = 12
} EXTENDED_NAME_FORMAT, *PEXTENDED_NAME_FORMAT;

BOOLEAN WINAPI GetComputerObjectNameA(EXTENDED_NAME_FORMAT,LPSTR,PULONG);
BOOLEAN WINAPI GetComputerObjectNameW(EXTENDED_NAME_FORMAT,LPWSTR,PULONG);
BOOLEAN WINAPI GetUserNameExA(EXTENDED_NAME_FORMAT,LPSTR,PULONG);
BOOLEAN WINAPI GetUserNameExW(EXTENDED_NAME_FORMAT,LPWSTR,PULONG);
BOOLEAN WINAPI TranslateNameA(LPCSTR,EXTENDED_NAME_FORMAT,EXTENDED_NAME_FORMAT,LPSTR,PULONG);
BOOLEAN WINAPI TranslateNameW(LPCWSTR,EXTENDED_NAME_FORMAT,EXTENDED_NAME_FORMAT,LPWSTR,PULONG);

#define GetComputerObjectName __AW(GetComputerObjectName)
#define GetUserNameEx __AW(GetUserNameEx)
#define TranslateName __AW(TranslateName)

#endif /* ! RC_INVOKED */
#endif /* _WIN32_WINNT >= _WIN32_WINNT_WIN2K */

#ifdef __cplusplus
}
#endif

#endif /* ! _SECEXT_H */
