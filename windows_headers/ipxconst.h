/**
 * @file ipxconst.h
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
#ifndef _IPXCONST_H
#define _IPXCONST_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Routing and Remote Access Services
 */

#ifdef __cplusplus
extern "C" {
#endif

/*--- Router Management Reference - Router Information Structures - IPX Information Structures */
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN2K)
#define ADMIN_STATE_DISABLED 0x00000001
#define ADMIN_STATE_ENABLED 0x00000002
#define ADMIN_STATE_ENABLED_ONLY_FOR_NETBIOS_STATIC_ROUTING 0x00000003
#define ADMIN_STATE_ENABLED_ONLY_FOR_OPER_STATE_UP 0x00000004
#endif /* (_WIN32_WINNT >= _WIN32_WINNT_WIN2K) */

#ifdef __cplusplus
}
#endif
#endif
