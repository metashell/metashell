/**
 * @file dsrole.h
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
#ifndef _DSROLE_H
#define _DSROLE_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (_WIN32_WINNT >= _WIN32_WINNT_WIN2K)
/*--- Active Directory Reference - Active Directory Enumerations */
typedef enum {
	DsRole_RoleStandaloneWorkstation,
	DsRole_RoleMemberWorkstation,
	DsRole_RoleStandaloneServer,
	DsRole_RoleMemberServer,
	DsRole_RoleBackupDomainController,
	DsRole_RolePrimaryDomainController
} DSROLE_MACHINE_ROLE;
typedef enum {
	DsRoleOperationIdle,
	DsRoleOperationActive,
	DsRoleOperationNeedReboot
} DSROLE_OPERATION_STATE;
typedef enum {
	DsRolePrimaryDomainInfoBasic,
	DsRoleUpgradeStatus,
	DsRoleOperationState
} DSROLE_PRIMARY_DOMAIN_INFO_LEVEL;
typedef enum {
	DsRoleServerUnknown,
	DsRoleServerPrimary,
	DsRoleServerBackup
} DSROLE_SERVER_STATE;
/*--- Active Directory Reference - Active Directory Structures - Directory Service Structures */
typedef struct {
	DSROLE_OPERATION_STATE OperationState;
} DSROLE_OPERATION_STATE_INFO;
typedef struct _DSROLE_PRIMARY_DOMAIN_INFO_BASIC {
	DSROLE_MACHINE_ROLE MachineRole;
	ULONG Flags;
	LPWSTR DomainNameFlat;
	LPWSTR DomainNameDns;
	LPWSTR DomainForestName;
	GUID DomainGuid;
} DSROLE_PRIMARY_DOMAIN_INFO_BASIC,*PDSROLE_PRIMARY_DOMAIN_INFO_BASIC;
#define DSROLE_PRIMARY_DS_RUNNING 0x00000001
#define DSROLE_PRIMARY_DS_MIXED_MODE 0x00000002
#define DSROLE_UPGRADE_IN_PROGRESS 0x00000004
#define DSROLE_PRIMARY_DOMAIN_GUID_PRESENT 0x01000000
typedef struct _DSROLE_UPGRADE_STATUS_INFO {
	ULONG OperationState;
	DSROLE_SERVER_STATE PreviousServerState;
} DSROLE_UPGRADE_STATUS_INFO,*PDSROLE_UPGRADE_STATUS_INFO;
#endif /* (_WIN32_WINNT >= _WIN32_WINNT_WIN2K) */

#ifdef __cplusplus
}
#endif
#endif
