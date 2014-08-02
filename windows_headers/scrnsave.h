/**
 * @file scrnsave.h
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
/* Created by Anders Norlander <anorland@hem2.passagen.se> */
#ifndef _SCRNSAVE_H
#define _SCRNSAVE_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

/* configure dialog identifier */
#define DLG_SCRNSAVECONFIGURE 2003

#define idsIsPassword		1000
#define idsIniFile		1001
#define idsScreenSaver		1002
#define idsPassword		1003
#define idsDifferentPW		1004
#define idsChangePW		1005
#define idsBadOldPW		1006
#define idsAppName		1007
#define idsNoHelpMemory		1008
#define idsHelpFile		1009
#define idsDefKeyword		1010

#define IDS_DESCRIPTION 1
#define ID_APP 100

#define WS_GT (WS_GROUP | WS_TABSTOP)
#define SCRM_VERIFYPW		WM_APP
#define MAXFILELEN		13
#define TITLEBARNAMELEN		40
#define APPNAMEBUFFERLEN	40
#define BUFFLEN			255

#ifndef RC_INVOKED

/* functions provided by the aothor of the screen saver */
BOOL WINAPI ScreenSaverConfigureDialog(HWND,UINT,WPARAM,LPARAM);
BOOL WINAPI RegisterDialogClasses(HANDLE);
LONG WINAPI ScreenSaverProc(HWND,UINT,WPARAM,LPARAM);

/* Change name of function if we are using UNICODE */
/* Cannot use _AW() since there is no A version. */
#ifdef UNICODE
#define DefScreenSaverProc DefScreenSaverProcW
#endif

/* default screen saver proc; call instead of DefWindowProc */
LONG WINAPI DefScreenSaverProc(HWND,UINT,WPARAM,LPARAM);

/* change password */
void WINAPI ScreenSaverChangePassword(HWND);

/* globals that may be used by screen saver */
extern HINSTANCE	hMainInstance;
extern HWND		hMainWindow;
extern BOOL		fChildPreview;
extern TCHAR		szName[];
extern TCHAR		szAppName[];
extern TCHAR		szIniFile[];
extern TCHAR		szScreenSaver[];
extern TCHAR		szHelpFile[];
extern TCHAR		szNoHelpMemory[];
extern UINT		MyHelpMessage;

#endif /* RC_INVOKED */

#ifdef __cplusplus
}
#endif

#endif /* _SCRNSAVE_H */
