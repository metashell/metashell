/**
 * @file aclapi.h
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
#ifndef _ACLAPI_H
#define _ACLAPI_H
#pragma GCC system_header
#include <_mingw.h>

#include <windows.h>
#include <accctrl.h>

#ifdef __cplusplus
extern "C" {
#endif

WINADVAPI VOID WINAPI BuildExplicitAccessWithNameA(PEXPLICIT_ACCESS_A,LPSTR,DWORD,ACCESS_MODE,DWORD);
WINADVAPI VOID WINAPI BuildExplicitAccessWithNameW(PEXPLICIT_ACCESS_W,LPWSTR,DWORD,ACCESS_MODE,DWORD);
WINADVAPI DWORD WINAPI BuildSecurityDescriptorA(PTRUSTEE_A,PTRUSTEE_A ,ULONG,PEXPLICIT_ACCESS_A,
  ULONG,PEXPLICIT_ACCESS_A,PSECURITY_DESCRIPTOR,PULONG,PSECURITY_DESCRIPTOR*);
WINADVAPI DWORD WINAPI BuildSecurityDescriptorW(PTRUSTEE_W,PTRUSTEE_W ,ULONG,PEXPLICIT_ACCESS_W,
  ULONG,PEXPLICIT_ACCESS_W,PSECURITY_DESCRIPTOR,PULONG,PSECURITY_DESCRIPTOR*);
WINADVAPI VOID WINAPI BuildTrusteeWithNameA(PTRUSTEE_A,LPSTR);
WINADVAPI VOID WINAPI BuildTrusteeWithNameW(PTRUSTEE_W,LPWSTR);
WINADVAPI VOID WINAPI BuildTrusteeWithObjectsAndNameA(PTRUSTEE_A,POBJECTS_AND_NAME_A,SE_OBJECT_TYPE,
  LPSTR,LPSTR,LPSTR);
WINADVAPI VOID WINAPI BuildTrusteeWithObjectsAndNameW(PTRUSTEE_W,POBJECTS_AND_NAME_W,SE_OBJECT_TYPE,
  LPWSTR,LPWSTR,LPWSTR);
WINADVAPI VOID WINAPI BuildTrusteeWithObjectsAndSidA(PTRUSTEE_A,POBJECTS_AND_SID,GUID*,GUID*,PSID);
WINADVAPI VOID WINAPI BuildTrusteeWithObjectsAndSidW(PTRUSTEE_W,POBJECTS_AND_SID,GUID*,GUID*,PSID);
WINADVAPI VOID WINAPI BuildTrusteeWithSidA(PTRUSTEE_A,PSID);
WINADVAPI VOID WINAPI BuildTrusteeWithSidW(PTRUSTEE_W,PSID);
WINADVAPI DWORD WINAPI GetAuditedPermissionsFromAclA(PACL,PTRUSTEE_A,PACCESS_MASK,PACCESS_MASK);
WINADVAPI DWORD WINAPI GetAuditedPermissionsFromAclW(PACL,PTRUSTEE_W,PACCESS_MASK,PACCESS_MASK);
WINADVAPI DWORD WINAPI GetEffectiveRightsFromAclA(PACL,PTRUSTEE_A,PACCESS_MASK);
WINADVAPI DWORD WINAPI GetEffectiveRightsFromAclW(PACL,PTRUSTEE_W,PACCESS_MASK);
WINADVAPI DWORD WINAPI GetExplicitEntriesFromAclA(PACL,PULONG,PEXPLICIT_ACCESS_A*);
WINADVAPI DWORD WINAPI GetExplicitEntriesFromAclW(PACL,PULONG,PEXPLICIT_ACCESS_W*);
WINADVAPI DWORD WINAPI GetNamedSecurityInfoA(LPSTR,SE_OBJECT_TYPE,SECURITY_INFORMATION,
  PSID*,PSID*,PACL*,PACL*,PSECURITY_DESCRIPTOR*);
WINADVAPI DWORD WINAPI GetNamedSecurityInfoW(LPWSTR,SE_OBJECT_TYPE,SECURITY_INFORMATION,
  PSID*,PSID*,PACL*,PACL*,PSECURITY_DESCRIPTOR*);
WINADVAPI DWORD WINAPI GetSecurityInfo(HANDLE,SE_OBJECT_TYPE,SECURITY_INFORMATION,
  PSID*,PSID*,PACL*,PACL*,PSECURITY_DESCRIPTOR*);
WINADVAPI TRUSTEE_FORM WINAPI GetTrusteeFormA(PTRUSTEE_A);
WINADVAPI TRUSTEE_FORM WINAPI GetTrusteeFormW(PTRUSTEE_W);
WINADVAPI LPSTR WINAPI GetTrusteeNameA(PTRUSTEE_A);
WINADVAPI LPWSTR WINAPI GetTrusteeNameW(PTRUSTEE_W);
WINADVAPI TRUSTEE_TYPE WINAPI GetTrusteeTypeA(PTRUSTEE_A);
WINADVAPI TRUSTEE_TYPE WINAPI GetTrusteeTypeW(PTRUSTEE_W);
WINADVAPI DWORD WINAPI LookupSecurityDescriptorPartsA(PTRUSTEE_A*,PTRUSTEE_A*,PULONG,PEXPLICIT_ACCESS_A*,
  PULONG,PEXPLICIT_ACCESS_A*,PSECURITY_DESCRIPTOR);
WINADVAPI DWORD WINAPI LookupSecurityDescriptorPartsW(PTRUSTEE_W*,PTRUSTEE_W*,PULONG,PEXPLICIT_ACCESS_W*,
  PULONG,PEXPLICIT_ACCESS_W*,PSECURITY_DESCRIPTOR);
WINADVAPI DWORD WINAPI SetEntriesInAclA(ULONG,PEXPLICIT_ACCESS_A,PACL,PACL*);
WINADVAPI DWORD WINAPI SetEntriesInAclW(ULONG,PEXPLICIT_ACCESS_W,PACL,PACL*);
WINADVAPI DWORD WINAPI SetNamedSecurityInfoA(LPSTR,SE_OBJECT_TYPE,SECURITY_INFORMATION,PSID,PSID,PACL,PACL);
WINADVAPI DWORD WINAPI SetNamedSecurityInfoW(LPWSTR,SE_OBJECT_TYPE,SECURITY_INFORMATION,PSID,PSID,PACL,PACL);
WINADVAPI DWORD WINAPI SetSecurityInfo(HANDLE,SE_OBJECT_TYPE,SECURITY_INFORMATION,PSID,PSID,PACL,PACL);
WINADVAPI VOID WINAPI BuildImpersonateExplicitAccessWithNameA(PEXPLICIT_ACCESS_A,LPSTR,PTRUSTEE_A,DWORD,ACCESS_MODE,DWORD);
WINADVAPI VOID WINAPI BuildImpersonateExplicitAccessWithNameW(PEXPLICIT_ACCESS_W,LPWSTR,PTRUSTEE_W,DWORD,ACCESS_MODE,DWORD);
WINADVAPI VOID WINAPI BuildImpersonateTrusteeA(PTRUSTEE_A,PTRUSTEE_A);
WINADVAPI VOID WINAPI BuildImpersonateTrusteeW(PTRUSTEE_W,PTRUSTEE_W);
WINADVAPI PTRUSTEE_A WINAPI GetMultipleTrusteeA(PTRUSTEE_A);
WINADVAPI PTRUSTEE_W WINAPI GetMultipleTrusteeW(PTRUSTEE_W);
WINADVAPI MULTIPLE_TRUSTEE_OPERATION WINAPI GetMultipleTrusteeOperationA(PTRUSTEE_A);
WINADVAPI MULTIPLE_TRUSTEE_OPERATION WINAPI GetMultipleTrusteeOperationW(PTRUSTEE_W);

#define BuildExplicitAccessWithName  __AW(BuildExplicitAccessWithName)
#define BuildSecurityDescriptor  __AW(BuildSecurityDescriptor)
#define BuildTrusteeWithName  __AW(BuildTrusteeWithName)
#define BuildTrusteeWithObjectsAndName  __AW(BuildTrusteeWithObjectsAndName)
#define BuildTrusteeWithObjectsAndSid  __AW(BuildTrusteeWithObjectsAndSid)
#define BuildTrusteeWithSid  __AW(BuildTrusteeWithSid)
#define GetAuditedPermissionsFromAcl  __AW(GetAuditedPermissionsFromAcl)
#define GetEffectiveRightsFromAcl  __AW(GetEffectiveRightsFromAcl)
#define GetExplicitEntriesFromAcl  __AW(GetExplicitEntriesFromAcl)
#define GetNamedSecurityInfo  __AW(GetNamedSecurityInfo)
#define GetTrusteeForm  __AW(GetTrusteeForm)
#define GetTrusteeName  __AW(GetTrusteeName)
#define GetTrusteeType  __AW(GetTrusteeType)
#define LookupSecurityDescriptorParts  __AW(LookupSecurityDescriptorParts)
#define SetEntriesInAcl  __AW(SetEntriesInAcl)
#define SetNamedSecurityInfo  __AW(SetNamedSecurityInfo)
#define BuildImpersonateExplicitAccessWithName  __AW(BuildImpersonateExplicitAccessWithName)
#define BuildImpersonateTrustee  __AW(BuildImpersonateTrustee)
#define GetMultipleTrustee  __AW(GetMultipleTrustee)
#define GetMultipleTrusteeOperation  __AW(GetMultipleTrusteeOperation)

#if (_WIN32_WINNT >= _WIN32_WINNT_WINXP)
WINADVAPI DWORD WINAPI GetInheritanceSourceA(LPSTR,SE_OBJECT_TYPE,SECURITY_INFORMATION,BOOL,GUID**,DWORD,PACL,void*,PGENERIC_MAPPING,PINHERITED_FROMA);
WINADVAPI DWORD WINAPI GetInheritanceSourceW(LPWSTR,SE_OBJECT_TYPE,SECURITY_INFORMATION,BOOL,GUID**,DWORD,PACL,void*,PGENERIC_MAPPING,PINHERITED_FROMW);
#endif

#ifdef __cplusplus
}
#endif
#endif
