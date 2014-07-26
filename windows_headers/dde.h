/**
 * @file dde.h
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
#ifndef _DDE_H
#define _DDE_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif
#define WM_DDE_FIRST	0x03E0
#define WM_DDE_INITIATE	(WM_DDE_FIRST)
#define WM_DDE_TERMINATE	(WM_DDE_FIRST+1)
#define WM_DDE_ADVISE	(WM_DDE_FIRST+2)
#define WM_DDE_UNADVISE	(WM_DDE_FIRST+3)
#define WM_DDE_ACK	(WM_DDE_FIRST+4)
#define WM_DDE_DATA	(WM_DDE_FIRST+5)
#define WM_DDE_REQUEST	(WM_DDE_FIRST+6)
#define WM_DDE_POKE	(WM_DDE_FIRST+7)
#define WM_DDE_EXECUTE	(WM_DDE_FIRST+8)
#define WM_DDE_LAST	(WM_DDE_FIRST+8)

#ifndef RC_INVOKED
typedef struct {
	unsigned short bAppReturnCode:8,reserved:6,fBusy:1,fAck:1;
} DDEACK;
typedef struct {
	unsigned short reserved:14,fDeferUpd:1,fAckReq:1;
	short cfFormat;
} DDEADVISE;
typedef struct {
	unsigned short unused:12,fResponse:1,fRelease:1,reserved:1,fAckReq:1;
	short cfFormat;
	BYTE Value[1];
} DDEDATA;
typedef struct {
	unsigned short unused:13,fRelease:1,fReserved:2;
	short cfFormat;
	BYTE  Value[1];
} DDEPOKE;
typedef struct {
        unsigned short unused:13,
                 fRelease:1,
                 fDeferUpd:1,
         fAckReq:1;
    short    cfFormat;
} DDELN;

typedef struct {
	unsigned short unused:12,fAck:1,fRelease:1,fReserved:1,fAckReq:1;
    short cfFormat;
    BYTE rgb[1];
} DDEUP;
BOOL WINAPI DdeSetQualityOfService(HWND,const SECURITY_QUALITY_OF_SERVICE*,PSECURITY_QUALITY_OF_SERVICE);
BOOL WINAPI ImpersonateDdeClientWindow(HWND,HWND);
LPARAM WINAPI PackDDElParam(UINT,UINT_PTR,UINT_PTR); 
BOOL WINAPI UnpackDDElParam(UINT,LPARAM,PUINT_PTR,PUINT_PTR); 
BOOL WINAPI FreeDDElParam(UINT,LPARAM); 
LPARAM WINAPI ReuseDDElParam(LPARAM,UINT,UINT,UINT_PTR,UINT_PTR); 
#endif

#ifdef __cplusplus
}
#endif
#endif
