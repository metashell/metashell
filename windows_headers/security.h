/**
 * @file security.h
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
#ifndef _SECURITY_H
#define _SECURITY_H
#pragma GCC system_header
#include <_mingw.h>

#define SEC_E_OK 0
#define SEC_E_CERT_EXPIRED (-2146893016)
#define SEC_E_INCOMPLETE_MESSAGE (-2146893032)
#define SEC_E_INSUFFICIENT_MEMORY (-2146893056)
#define SEC_E_INTERNAL_ERROR (-2146893052)
#define SEC_E_INVALID_HANDLE (-2146893055)
#define SEC_E_INVALID_TOKEN (-2146893048)
#define SEC_E_LOGON_DENIED (-2146893044)
#define SEC_E_NO_AUTHENTICATING_AUTHORITY (-2146893039)
#define SEC_E_NO_CREDENTIALS (-2146893042)
#define SEC_E_TARGET_UNKNOWN (-2146893053)
#define SEC_E_UNSUPPORTED_FUNCTION (-2146893054)
#define SEC_E_UNTRUSTED_ROOT (-2146893019)
#define SEC_E_WRONG_PRINCIPAL (-2146893022)
#define SEC_E_SECPKG_NOT_FOUND (-2146893051)
#define SEC_E_QOP_NOT_SUPPORTED (-2146893046)
#define SEC_E_UNKNOWN_CREDENTIALS (-2146893043)
#define SEC_E_NOT_OWNER (-2146893050)
#define SEC_I_RENEGOTIATE 590625
#define SEC_I_COMPLETE_AND_CONTINUE 590612
#define SEC_I_COMPLETE_NEEDED 590611
#define SEC_I_CONTINUE_NEEDED 590610
#define SEC_I_INCOMPLETE_CREDENTIALS 590624

/* always a char */
typedef char SEC_CHAR;
typedef wchar_t SEC_WCHAR;
typedef long SECURITY_STATUS;
#define SEC_FAR

#include <sspi.h>
#include <ntsecpkg.h>
#include <secext.h>

#endif /* _SECURITY_H */
