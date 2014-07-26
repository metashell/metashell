/**
 * @file rapi.h
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
#ifndef _RAPI_H
#define _RAPI_H
#pragma GCC system_header
#include <_mingw.h>

typedef struct IRAPIStream
{
  struct IRAPIStreamVtbl * lpVtbl;
} IRAPIStream;

typedef struct IRAPIStreamVtbl IRAPIStreamVtbl;

typedef enum tagRAPISTREAMFLAG
{
  STREAM_TIMEOUT_READ
} RAPISTREAMFLAG;

struct IRAPIStreamVtbl
{
  HRESULT (__stdcall * SetRapiStat)( IRAPIStream *, RAPISTREAMFLAG, DWORD);
  HRESULT (__stdcall * GetRapiStat)( IRAPIStream *, RAPISTREAMFLAG, DWORD *);
};

typedef  HRESULT (STDAPICALLTYPE RAPIEXT)(DWORD, BYTE, DWORD, BYTE, IRAPIStream	*);

typedef struct _RAPIINIT
{
  DWORD cbSize;
  HANDLE heRapiInit;
  HRESULT hrRapiInit;
} RAPIINIT;

STDAPI CeRapiInit (void);
STDAPI CeRapiInitEx (RAPIINIT*);
STDAPI_(BOOL) CeCreateProcess (LPCWSTR, LPCWSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES,
			       BOOL, DWORD, LPVOID, LPWSTR, LPSTARTUPINFO, LPPROCESS_INFORMATION);
STDAPI CeRapiUninit (void);

STDAPI_(BOOL) CeWriteFile (HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
STDAPI_(HANDLE) CeCreateFile (LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE); 
STDAPI_(BOOL) CeCreateDirectory (LPCWSTR, LPSECURITY_ATTRIBUTES); 
STDAPI_(DWORD) CeGetLastError (void);
STDAPI_(BOOL) CeGetFileTime (HANDLE, LPFILETIME, LPFILETIME, LPFILETIME); 
STDAPI_(BOOL) CeCloseHandle (HANDLE); 

#endif /* _RAPI_H */
