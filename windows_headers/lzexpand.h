/**
 * @file lzexpand.h
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
#ifndef _LZEXPAND_H
#define _LZEXPAND_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LZERROR_BADINHANDLE	(-1)
#define LZERROR_BADOUTHANDLE	(-2)
#define LZERROR_READ	(-3)
#define LZERROR_WRITE	(-4)
#define LZERROR_GLOBALLOC	(-5)
#define LZERROR_GLOBLOCK	(-6)
#define LZERROR_BADVALUE	(-7)
#define LZERROR_UNKNOWNALG	(-8)
LONG WINAPI CopyLZFile(INT,INT);
INT WINAPI GetExpandedNameA(LPSTR,LPSTR);
INT WINAPI GetExpandedNameW(LPWSTR,LPWSTR);
VOID APIENTRY LZClose(INT);
LONG APIENTRY LZCopy(INT,INT);
VOID WINAPI LZDone(VOID);
INT WINAPI LZInit(INT);
INT WINAPI LZOpenFileA(LPSTR,LPOFSTRUCT,WORD);
INT WINAPI LZOpenFileW(LPWSTR,LPOFSTRUCT,WORD);
INT WINAPI LZRead(INT,LPSTR,INT);
LONG WINAPI LZSeek(INT,LONG,INT);
INT WINAPI LZStart(VOID);
#define GetExpandedName __AW(GetExpandedName)
#define LZOpenFile  __AW(LZOpenFile)

#ifdef __cplusplus
}

#endif
#endif
