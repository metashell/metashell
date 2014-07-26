/**
 * @file rasdlg.h
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
#ifndef _RASDLG_H
#define _RASDLG_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <ras.h>

#define RASPBDEVENT_AddEntry	1
#define RASPBDEVENT_EditEntry	2
#define RASPBDEVENT_RemoveEntry	3
#define RASPBDEVENT_DialEntry	4
#define RASPBDEVENT_EditGlobals	5
#define RASPBDEVENT_NoUser	6
#define RASPBDEVENT_NoUserEdit	7
 
#define RASPBDFLAG_PositionDlg	1
#define RASPBDFLAG_ForceCloseOnDial	2
#define RASPBDFLAG_NoUser	16

#define RASEDFLAG_PositionDlg	1
#define RASEDFLAG_NewEntry	2
#define RASEDFLAG_CloneEntry	4

#define RASDDFLAG_PositionDlg	1

#ifndef RC_INVOKED
#include <pshpack4.h>

typedef struct tagRASENTRYDLGA
{
	DWORD dwSize;
	HWND  hwndOwner;
	DWORD dwFlags;
	LONG  xDlg;
	LONG  yDlg;
	CHAR  szEntry[RAS_MaxEntryName + 1];
	DWORD dwError;
	ULONG_PTR reserved;
	ULONG_PTR reserved2;
} RASENTRYDLGA, *LPRASENTRYDLGA;
typedef struct tagRASENTRYDLGW
{
	DWORD dwSize;
	HWND  hwndOwner;
	DWORD dwFlags;
	LONG  xDlg;
	LONG  yDlg;
	WCHAR szEntry[RAS_MaxEntryName + 1];
	DWORD dwError;
	ULONG_PTR reserved;
	ULONG_PTR reserved2;
} RASENTRYDLGW, *LPRASENTRYDLGW;

typedef struct tagRASDIALDLG
{
	DWORD dwSize;
	HWND  hwndOwner;
	DWORD dwFlags;
	LONG  xDlg;
	LONG  yDlg;
	DWORD dwSubEntry;
	DWORD dwError;
	ULONG_PTR reserved;
	ULONG_PTR reserved2;
} RASDIALDLG, *LPRASDIALDLG;

/* Application-defined callback functions */
typedef VOID (WINAPI* RASPBDLGFUNCW)(DWORD, DWORD, LPWSTR, LPVOID);
typedef VOID (WINAPI* RASPBDLGFUNCA)(DWORD, DWORD, LPSTR, LPVOID);

typedef struct tagRASPBDLGA
{
	DWORD         dwSize;
	HWND          hwndOwner;
	DWORD         dwFlags;
	LONG          xDlg;
	LONG          yDlg;
	ULONG_PTR     dwCallbackId;
	RASPBDLGFUNCA pCallback;
	DWORD         dwError;
	ULONG_PTR     reserved;
	ULONG_PTR     reserved2;
} RASPBDLGA, *LPRASPBDLGA;
typedef struct tagRASPBDLGW
{
	DWORD         dwSize;
	HWND          hwndOwner;
	DWORD         dwFlags;
	LONG          xDlg;
	LONG          yDlg;
	ULONG_PTR     dwCallbackId;
	RASPBDLGFUNCW pCallback;
	DWORD         dwError;
	ULONG_PTR     reserved;
	ULONG_PTR     reserved2;
} RASPBDLGW, *LPRASPBDLGW;

typedef struct tagRASNOUSERA
{
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwTimeoutMs;
	CHAR  szUserName[UNLEN + 1];
	CHAR  szPassword[PWLEN + 1];
	CHAR  szDomain[DNLEN + 1];
} RASNOUSERA, *LPRASNOUSERA;
typedef struct tagRASNOUSERW
{
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwTimeoutMs;
	WCHAR szUserName[UNLEN + 1];
	WCHAR szPassword[PWLEN + 1];
	WCHAR szDomain[DNLEN + 1];
} RASNOUSERW, *LPRASNOUSERW ;

#include <poppack.h>

BOOL APIENTRY RasDialDlgA(LPSTR,LPSTR,LPSTR,LPRASDIALDLG);
BOOL APIENTRY RasDialDlgW(LPWSTR,LPWSTR,LPWSTR,LPRASDIALDLG);
BOOL APIENTRY RasEntryDlgA(LPSTR,LPSTR,LPRASENTRYDLGA);
BOOL APIENTRY RasEntryDlgW(LPWSTR,LPWSTR,LPRASENTRYDLGW);
BOOL APIENTRY RasPhonebookDlgA(LPSTR,LPSTR,LPRASPBDLGA);
BOOL APIENTRY RasPhonebookDlgW(LPWSTR,LPWSTR,LPRASPBDLGW);

typedef __AW(RASENTRYDLG)	RASENTRYDLG, *LPRASENTRYDLG;
typedef __AW(RASPBDLG)	RASPBDLG, *LPRASPBDLG;
typedef __AW(RASNOUSER)	RASNOUSER, *LPRASNOUSER;
#define RasDialDlg	__AW(RasDialDlg)
#define RasEntryDlg	__AW(RasEntryDlg)
#define RasPhonebookDlg	__AW(RasPhonebookDlg)

#endif /* RC_INVOKED */

#ifdef __cplusplus
}
#endif

#endif
