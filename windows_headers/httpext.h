/**
 * @file httpext.h
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
#ifndef _HTTPEXT_H
#define _HTTPEXT_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * ISAPI extensions.
 */

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define	HSE_VERSION_MAJOR	2
#define	HSE_VERSION_MINOR	0
#define	HSE_LOG_BUFFER_LEN	80
#define	HSE_MAX_EXT_DLL_NAME_LEN	256
#define	HSE_STATUS_SUCCESS	1
#define	HSE_STATUS_SUCCESS_AND_KEEP_CONN	2
#define	HSE_STATUS_PENDING	3
#define	HSE_STATUS_ERROR	4
#define	HSE_REQ_BASE	0
#define	HSE_REQ_SEND_URL_REDIRECT_RESP	1
#define	HSE_REQ_SEND_URL	2
#define	HSE_REQ_SEND_RESPONSE_HEADER	3
#define	HSE_REQ_DONE_WITH_SESSION	4
#define	HSE_REQ_SEND_RESPONSE_HEADER_EX	1016
#define	HSE_REQ_END_RESERVED	1000
#define	HSE_TERM_ADVISORY_UNLOAD	0x00000001
#define	HSE_TERM_MUST_UNLOAD	0x00000002
#define	HSE_IO_SYNC	0x00000001
#define	HSE_IO_ASYNC	0x00000002
#define	HSE_IO_DISCONNECT_AFTER_SEND	0x00000004
#define	HSE_IO_SEND_HEADERS	0x00000008
typedef	LPVOID HCONN;
typedef	struct _HSE_VERSION_INFO {
	DWORD	dwExtensionVersion;
	CHAR	lpszExtensionDesc[HSE_MAX_EXT_DLL_NAME_LEN];
} HSE_VERSION_INFO, *LPHSE_VERSION_INFO;
typedef	struct _EXTENSION_CONTROL_BLOCK	{
	DWORD	cbSize;
	DWORD	dwVersion;
	HCONN	ConnID;
	DWORD	dwHttpStatusCode;
	CHAR	lpszLogData[HSE_LOG_BUFFER_LEN];
	LPSTR	lpszMethod;
	LPSTR	lpszQueryString;
	LPSTR	lpszPathInfo;
	LPSTR	lpszPathTranslated;
	DWORD	cbTotalBytes;
	DWORD	cbAvailable;
	LPBYTE	lpbData;
	LPSTR	lpszContentType;
	BOOL	(PASCAL	*GetServerVariable)(HCONN,LPSTR,LPVOID,LPDWORD);
	BOOL	(PASCAL	*WriteClient)(HCONN,LPVOID,LPDWORD,DWORD);
	BOOL	(PASCAL	*ReadClient)(HCONN,LPVOID,LPDWORD);
	BOOL	(PASCAL	*ServerSupportFunction)(HCONN,DWORD,LPVOID,LPDWORD,LPDWORD);
} EXTENSION_CONTROL_BLOCK, *LPEXTENSION_CONTROL_BLOCK;
typedef	BOOL	(PASCAL	*PFN_GETEXTENSIONVERSION)(HSE_VERSION_INFO*);
typedef	DWORD	(PASCAL	*PFN_HTTPEXTENSIONPROC)(EXTENSION_CONTROL_BLOCK*);
typedef	BOOL	(PASCAL	*PFN_TERMINATEEXTENSION)(DWORD);
typedef	VOID	(PASCAL	*PFN_HSE_IO_COMPLETION)(EXTENSION_CONTROL_BLOCK*,PVOID,DWORD,DWORD);
typedef struct _HSE_TF_INFO {
	PFN_HSE_IO_COMPLETION pfnHseIO;
	PVOID pContext;
	HANDLE hFile;
	LPCSTR pszStatusCode;
	DWORD BytesToWrite;
	DWORD Offset;
	PVOID pHead;
	DWORD HeadLength;
	PVOID pTail;
	DWORD TailLength;
	DWORD dwFlags;
} HSE_TF_INFO, *LPHSE_TF_INFO;
typedef struct _HSE_SEND_HEADER_EX_INFO {
	LPCSTR pszStatus;
	LPCSTR pszHeader;
	DWORD cchStatus;
	DWORD cchHeader;
	BOOL fKeepConn;
} HSE_SEND_HEADER_EX_INFO, * LPHSE_SEND_HEADER_EX_INF;
BOOL	PASCAL	GetExtensionVersion(HSE_VERSION_INFO*);
DWORD	PASCAL	HttpExtensionProc(EXTENSION_CONTROL_BLOCK*);
BOOL	PASCAL	TerminateExtension(DWORD);
#ifdef __cplusplus
}
#endif
#endif /* _HTTPEXT_H */
