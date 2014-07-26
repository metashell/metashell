/**
 * @file svcguid.h
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
#ifndef _SVCGUID_H
#define _SVCGUID_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SVCID_NETWARE(_SapId) \
	{ (0x000B << 16) | (_SapId), 0, 0, { 0xC0,0,0,0,0,0,0,0x46 } }

#define SAPID_FROM_SVCID_NETWARE(_g) \
	((WORD)(_g->Data1 & 0xFFFF))

#define SET_NETWARE_SVCID(_g,_SapId) { \
	(_g)->Data1 = (0x000B << 16 ) | (_SapId); \
	(_g)->Data2 = 0; \
	(_g)->Data3 = 0; \
	(_g)->Data4[0] = 0xC0; \
	(_g)->Data4[1] = 0x0; \
	(_g)->Data4[2] = 0x0; \
	(_g)->Data4[3] = 0x0; \
	(_g)->Data4[4] = 0x0; \
	(_g)->Data4[5] = 0x0; \
	(_g)->Data4[6] = 0x0; \
	(_g)->Data4[7] = 0x46; }

#ifdef __cplusplus
}
#endif

#endif
