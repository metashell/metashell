/**
 * @file schannel.h
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
#ifndef _SCHANNEL_H
#define _SCHANNEL_H
#pragma GCC system_header
#include <_mingw.h>

#include <wincrypt.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCHANNEL_CRED_VERSION 4
#define SCHANNEL_SHUTDOWN 1
/* ? Do these belong here or in wincrypt.h */
#define AUTHTYPE_CLIENT 1
#define AUTHTYPE_SERVER 2

#define SP_PROT_TLS1_CLIENT 128
#define SP_PROT_TLS1_SERVER 64
#define SP_PROT_SSL3_CLIENT 32
#define SP_PROT_SSL3_SERVER 16
#define SP_PROT_SSL2_CLIENT 8
#define SP_PROT_SSL2_SERVER 4
#define SP_PROT_PCT1_SERVER 1
#define SP_PROT_PCT1_CLIENT 2

#define SP_PROT_PCT1 (SP_PROT_PCT1_CLIENT|SP_PROT_PCT1_SERVER)
#define SP_PROT_TLS1 (SP_PROT_TLS1_CLIENT|SP_PROT_TLS1_SERVER)
#define SP_PROT_SSL2 (SP_PROT_SSL2_CLIENT|SP_PROT_SSL2_SERVER)
#define SP_PROT_SSL3 (SP_PROT_SSL3_CLIENT|SP_PROT_SSL3_SERVER)

#define SCH_CRED_NO_SYSTEM_MAPPER 2
#define SCH_CRED_NO_SERVERNAME_CHECK 4
#define SCH_CRED_MANUAL_CRED_VALIDATION 8
#define SCH_CRED_NO_DEFAULT_CREDS 16
#define SCH_CRED_AUTO_CRED_VALIDATION 32
#define SCH_CRED_REVOCATION_CHECK_CHAIN 512
#define SCH_CRED_REVOCATION_CHECK_END_CERT 256
#define SCH_CRED_REVOCATION_CHECK_CHAIN_EXCLUDE_ROOT 1024
#define SCH_CRED_IGNORE_NO_REVOCATION_CHECK 2048
#define SCH_CRED_IGNORE_REVOCATION_OFFLINE 4096
#define SCH_CRED_USE_DEFAULT_CREDS 64

typedef struct _SCHANNEL_CRED {
	DWORD dwVersion;
	DWORD cCreds;
	PCCERT_CONTEXT* paCred;
	HCERTSTORE hRootStore;
	DWORD cMappers;
	struct _HMAPPER** aphMappers;
	DWORD cSupportedAlgs;
	ALG_ID* palgSupportedAlgs;
	DWORD grbitEnabledProtocols;
	DWORD dwMinimumCypherStrength;
	DWORD dwMaximumCypherStrength;
	DWORD dwSessionLifespan;
	DWORD dwFlags;
	DWORD reserved;
} SCHANNEL_CRED, *PSCHANNEL_CRED;
typedef struct _SecPkgCred_SupportedAlgs {
	DWORD cSupportedAlgs;
	ALG_ID* palgSupportedAlgs;
} SecPkgCred_SupportedAlgs, *PSecPkgCred_SupportedAlgs;
typedef struct _SecPkgCred_CypherStrengths {
	DWORD dwMinimumCypherStrength;
	DWORD dwMaximumCypherStrength;
} SecPkgCred_CypherStrengths, *PSecPkgCred_CypherStrengths;
typedef struct _SecPkgCred_SupportedProtocols {
	DWORD grbitProtocol;
} SecPkgCred_SupportedProtocols, *PSecPkgCred_SupportedProtocols;
typedef struct _SecPkgContext_IssuerListInfoEx {
	PCERT_NAME_BLOB aIssuers;
	DWORD cIssuers;
} SecPkgContext_IssuerListInfoEx, *PSecPkgContext_IssuerListInfoEx;
typedef struct _SecPkgContext_ConnectionInfo {
	DWORD dwProtocol;
	ALG_ID aiCipher;
	DWORD dwCipherStrength;
	ALG_ID aiHash;
	DWORD dwHashStrength;
	ALG_ID aiExch;
	DWORD dwExchStrength;
} SecPkgContext_ConnectionInfo, *PSecPkgContext_ConnectionInfo;

#ifdef __cplusplus
}
#endif

#endif /* _SCHANNEL_H */
