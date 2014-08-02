/**
 * @file dsgetdc.h
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
#ifndef _DSGETDC_H
#define _DSGETDC_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

/*--- Active Directory Reference - Active Directory Structures - Directory Service Structures */
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN2K)
#define DS_FORCE_REDISCOVERY 0x00000001
#define DS_DIRECTORY_SERVICE_REQUIRED 0x00000010
#define DS_DIRECTORY_SERVICE_PREFERRED 0x00000020
#define DS_GC_SERVER_REQUIRED 0x00000040
#define DS_PDC_REQUIRED 0x00000080
#define DS_BACKGROUND_ONLY 0x00000100
#define DS_IP_REQUIRED 0x00000200
#define DS_KDC_REQUIRED 0x00000400
#define DS_TIMESERV_REQUIRED 0x00000800
#define DS_WRITABLE_REQUIRED 0x00001000
#define DS_GOOD_TIMESERV_PREFERRED 0x00002000
#define DS_AVOID_SELF 0x00004000
#define DS_ONLY_LDAP_NEEDED 0x00008000
#define DS_IS_FLAT_NAME 0x00010000
#define DS_IS_DNS_NAME 0x00020000
#define DS_RETURN_DNS_NAME 0x40000000
#define DS_RETURN_FLAT_NAME 0x80000000
typedef struct _DOMAIN_CONTROLLER_INFOA {
	LPSTR DomainControllerName;
	LPSTR DomainControllerAddress;
	ULONG DomainControllerAddressType;
	GUID DomainGuid;
	LPSTR DomainName;
	LPSTR DnsForestName;
	ULONG Flags;
	LPSTR DcSiteName;
	LPSTR ClientSiteName;
} DOMAIN_CONTROLLER_INFOA,*PDOMAIN_CONTROLLER_INFOA;
typedef struct _DOMAIN_CONTROLLER_INFOW {
	LPWSTR DomainControllerName;
	LPWSTR DomainControllerAddress;
	ULONG DomainControllerAddressType;
	GUID DomainGuid;
	LPWSTR DomainName;
	LPWSTR DnsForestName;
	ULONG Flags;
	LPWSTR DcSiteName;
	LPWSTR ClientSiteName;
} DOMAIN_CONTROLLER_INFOW,*PDOMAIN_CONTROLLER_INFOW;
#define DS_INET_ADDRESS 0x0001
#define DS_NETBIOS_ADDRESS 0x0002
#define DS_PDC_FLAG 0x00000001
#define DS_GC_FLAG 0x00000004
#define DS_DS_FLAG 0x00000010
#define DS_KDC_FLAG 0x00000020
#define DS_TIMESERV_FLAG 0x00000040
#define DS_WRITABLE_FLAG 0x00000100
#define DS_DNS_CONTROLLER_FLAG 0x20000000
#define DS_DNS_DOMAIN_FLAG 0x40000000
#define DS_DNS_FOREST_FLAG 0x80000000
typedef struct _DS_DOMAIN_TRUSTSA {
	LPSTR NetbiosDomainName;
	LPSTR DnsDomainName;
	ULONG Flags;
	ULONG ParentIndex;
	ULONG TrustType;
	ULONG TrustAttributes;
	PSID DomainSid;
	GUID DomainGuid;
} DS_DOMAIN_TRUSTSA,*PDS_DOMAIN_TRUSTSA;
typedef struct _DS_DOMAIN_TRUSTSW {
	LPWSTR NetbiosDomainName;
	LPWSTR DnsDomainName;
	ULONG Flags;
	ULONG ParentIndex;
	ULONG TrustType;
	ULONG TrustAttributes;
	PSID DomainSid;
	GUID DomainGuid;
} DS_DOMAIN_TRUSTSW,*PDS_DOMAIN_TRUSTSW;
#define DS_DOMAIN_IN_FOREST 0x0001
#define DS_DOMAIN_DIRECT_OUTBOUND 0x0002
#define DS_DOMAIN_TREE_ROOT 0x0004
#define DS_DOMAIN_PRIMARY 0x0008
#define DS_DOMAIN_NATIVE_MODE 0x0010
#define DS_DOMAIN_DIRECT_INBOUND 0x0020

DWORD WINAPI DsGetDcNameW(LPCWSTR, LPCWSTR, GUID*, LPCWSTR, ULONG, PDOMAIN_CONTROLLER_INFOW*);
DWORD WINAPI DsGetDcNameA(LPCSTR, LPCSTR, GUID*, LPCSTR, ULONG, PDOMAIN_CONTROLLER_INFOA*);

typedef __AW(DOMAIN_CONTROLLER_INFO) DOMAIN_CONTROLLER_INFO,*PDOMAIN_CONTROLLER_INFO;
typedef __AW(DS_DOMAIN_TRUSTS) DS_DOMAIN_TRUSTS,*PDS_DOMAIN_TRUSTS;
#define DsGetDcName __AW(DsGetDcName)
#endif /* (_WIN32_WINNT >= _WIN32_WINNT_WIN2K) */

#ifdef __cplusplus
}
#endif
#endif
