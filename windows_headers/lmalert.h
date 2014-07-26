/**
 * @file lmalert.h
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
#ifndef _LMALERT_H
#define _LMALERT_H
#pragma GCC system_header
#include <_mingw.h>

#define ALERTER_MAILSLOT TEXT("\\\\.\\MAILSLOT\\Alerter")
#define ALERT_PRINT_EVENT TEXT("PRINTING")
#define ALERT_MESSAGE_EVENT TEXT("MESSAGE")
#define ALERT_ERRORLOG_EVENT TEXT("ERRORLOG")
#define ALERT_ADMIN_EVENT TEXT("ADMIN")
#define ALERT_USER_EVENT TEXT("USER")
#define ALERT_OTHER_INFO(x) ((PBYTE)(x)+sizeof(STD_ALERT))
#define ALERT_VAR_DATA(p) ((PBYTE)(p)+sizeof(*p))
#define PRJOB_QSTATUS 3
#define PRJOB_DEVSTATUS 508
#define PRJOB_COMPLETE 4
#define PRJOB_INTERV 8
#define PRJOB_ 16
#define PRJOB_DESTOFFLINE 32
#define PRJOB_DESTPAUSED 64
#define PRJOB_NOTIFY 128
#define PRJOB_DESTNOPAPER 256
#define PRJOB_DELETED 32768
#define PRJOB_QS_QUEUED 0
#define PRJOB_QS_PAUSED 1
#define PRJOB_QS_SPOOLING 2
#define PRJOB_QS_PRINTING 3

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ADMIN_OTHER_INFO {
	DWORD alrtad_errcode;
	DWORD alrtad_numstrings;
}ADMIN_OTHER_INFO,*PADMIN_OTHER_INFO,*LPADMIN_OTHER_INFO;
typedef struct _STD_ALERT {
	DWORD alrt_timestamp;
	TCHAR alrt_eventname[EVLEN+1];
	TCHAR alrt_servicename[SNLEN+1];
}STD_ALERT,*PSTD_ALERT,*LPSTD_ALERT;
typedef struct _ERRLOG_OTHER_INFO {
	DWORD alrter_errcode;
	DWORD alrter_offset;
}ERRLOG_OTHER_INFO,*PERRLOG_OTHER_INFO,*LPERRLOG_OTHER_INFO;
typedef struct _PRINT_OTHER_INFO {
	DWORD alrtpr_jobid;
	DWORD alrtpr_status;
	DWORD alrtpr_submitted;
	DWORD alrtpr_size;
}PRINT_OTHER_INFO,*PPRINT_OTHER_INFO,*LPPRINT_OTHER_INFO;
typedef struct _USER_OTHER_INFO {
	DWORD alrtus_errcode;
	DWORD alrtus_numstrings;
}USER_OTHER_INFO,*PUSER_OTHER_INFO,*LPUSER_OTHER_INFO;
NET_API_STATUS WINAPI NetAlertRaise(LPCWSTR,PVOID,DWORD);
NET_API_STATUS WINAPI NetAlertRaiseEx(LPCWSTR,PVOID,DWORD,LPCWSTR);

#ifdef __cplusplus
}
#endif

#endif
