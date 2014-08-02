/**
 * @file w32api.h
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
#ifndef _W32API_H
#define _W32API_H
#pragma GCC system_header
#warning WARNING - The w32api.h header file is deprecated and will be removed.

#define __W32API_VERSION 3.17
#define __W32API_MAJOR_VERSION 3
#define __W32API_MINOR_VERSION 17

/* The following defines are for documentation purposes.  The following defines
 * identify the versions of Windows and Internet Explorer.  They are not to be
 * used in the w32api library but may be used by a user to set the _WIN32_WINNT
 * or _WIN32_WINDOWS and the WINVER values to their minimum level of support.
 *
 * Similarly the user can use the Internet Explorer values to set the _WIN32_IE
 * value to their minimum level of support.
 */

/* Use these values to set _WIN32_WINDOWS and WINVER to your minimum support 
 * level */
#define Windows95    0x0400
#define Windows98    0x0410
#define WindowsME    0x0500

/* Use these values to set _WIN32_WINNT and WINVER to your mimimum support 
 * level. */
#define WindowsNT4   0x0400
#define Windows2000  0x0500
#define WindowsXP    0x0501
#define Windows2003  0x0502
#define WindowsVista 0x0600

/* Use these values to set _WIN32_IE to your minimum support level */
#define IE3	0x0300
#define IE301	0x0300
#define IE302	0x0300
#define IE4	0x0400
#define IE401	0x0401
#define IE5	0x0500
#define IE5a	0x0500
#define IE5b	0x0500
#define IE501	0x0501
#define IE55	0x0501
#define IE56	0x0560
#define IE6	0x0600
#define IE601	0x0601
#define IE602	0x0603
#define IE7	0x0700

#endif /* ndef _W32API_H */
