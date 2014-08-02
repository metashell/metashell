/**
 * @file mgmtapi.h
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
#ifndef _MGMTAPI_H
#define _MGMTAPI_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * SNMP Management API
 */
#include <snmp.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SNMP_MGMTAPI_TIMEOUT	40
#define SNMP_MGMTAPI_SELECT_FDERRORS	41
#define SNMP_MGMTAPI_TRAP_ERRORS	42
#define SNMP_MGMTAPI_TRAP_DUPINIT	43
#define SNMP_MGMTAPI_NOTRAPS	44
#define SNMP_MGMTAPI_AGAIN	45
#define SNMP_MGMTAPI_INVALID_CTL	46
#define SNMP_MGMTAPI_INVALID_SESSION	47
#define SNMP_MGMTAPI_INVALID_BUFFER	48
#define MGMCTL_SETAGENTPORT	1

#ifndef RC_INVOKED

typedef PVOID LPSNMP_MGR_SESSION;

BOOL WINSNMPAPI SnmpMgrClose(LPSNMP_MGR_SESSION);
BOOL WINSNMPAPI SnmpMgrCtl(LPSNMP_MGR_SESSION,DWORD,LPVOID,DWORD,LPVOID,DWORD,LPDWORD);
BOOL WINSNMPAPI SnmpMgrGetTrap(AsnObjectIdentifier*,AsnNetworkAddress*,AsnInteger*,AsnInteger*,AsnTimeticks*,SnmpVarBindList*);
BOOL WINSNMPAPI SnmpMgrGetTrapEx(AsnObjectIdentifier*,AsnNetworkAddress*,AsnNetworkAddress*,AsnInteger*,AsnInteger*,AsnOctetString*,AsnTimeticks*,SnmpVarBindList*);
BOOL WINSNMPAPI SnmpMgrOidToStr(AsnObjectIdentifier*,LPSTR*);
LPSNMP_MGR_SESSION WINSNMPAPI SnmpMgrOpen(LPSTR,LPSTR,INT,INT);
INT WINSNMPAPI SnmpMgrRequest(LPSNMP_MGR_SESSION,BYTE,SnmpVarBindList*,AsnInteger*,AsnInteger*);
BOOL WINSNMPAPI SnmpMgrStrToOid(LPSTR,AsnObjectIdentifier*);
BOOL WINSNMPAPI SnmpMgrTrapListen(HANDLE*);

#endif /* RC_INVOKED */

#ifdef __cplusplus
}
#endif

#endif
