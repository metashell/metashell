/**
 * @file winber.h
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
/* Written by Filip Navara <xnavara@volny.cz> */
#ifndef _WINBER_H
#define _WINBER_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * LDAP Basic Encoding Rules API
 * References:
 *   The C LDAP Application Program Interface
 *   http://www.watersprings.org/pub/id/draft-ietf-ldapext-ldap-c-api-05.txt
 *
 *   Lightweight Directory Access Protocol Reference
 *   http://msdn.microsoft.com/library/en-us/netdir/ldap/ldap_reference.asp
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WINBERAPI
#define WINBERAPI DECLSPEC_IMPORT
#endif

typedef struct berelement BerElement;
typedef ULONG ber_len_t;
#include <pshpack4.h>
typedef struct berval {
	ber_len_t bv_len;
	char *bv_val;
} BerValue, LDAP_BERVAL, *PLDAP_BERVAL, BERVAL, *PBERVAL;
#include <poppack.h>

typedef ULONG ber_tag_t;
typedef INT ber_int_t;
typedef UINT ber_uint_t;
typedef INT ber_slen_t;

#define LBER_ERROR ((ber_tag_t)-1)
#define LBER_DEFAULT ((ber_tag_t)-1)
#define LBER_USE_DER 0x01

WINBERAPI BerElement *ber_init(const BerValue*);
WINBERAPI int ber_printf(BerElement*,const char*,...);
WINBERAPI int ber_flatten(BerElement*,BerValue**);
WINBERAPI ber_tag_t ber_scanf(BerElement*,const char*,...);
WINBERAPI ber_tag_t ber_peek_tag(BerElement*,ber_len_t*);
WINBERAPI ber_tag_t ber_skip_tag(BerElement*,ber_len_t*);
WINBERAPI ber_tag_t ber_first_element(BerElement*,ber_len_t*,char**);
WINBERAPI ber_tag_t ber_next_element(BerElement*,ber_len_t*,char*);
WINBERAPI void ber_bvfree(BerValue*);
WINBERAPI void ber_bvecfree(BerValue**);
WINBERAPI void ber_free(BerElement*,int);
WINBERAPI BerValue *ber_bvdup(BerValue*);
WINBERAPI BerElement *ber_alloc_t(int);

#ifdef __cplusplus
}
#endif

#endif /* _WINBER_H */
