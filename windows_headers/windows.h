/**
 * @file windows.h
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
/* Written by Anders Norlander <anorland@hem2.passagen.se> */
#ifndef _WINDOWS_H
#define _WINDOWS_H
#pragma GCC system_header
#include <_mingw.h>

/* translate GCC target defines to MS equivalents. Keep this synchronized
   with winnt.h. */
#if defined(__i686__) && !defined(_M_IX86)
#define _M_IX86 600
#elif defined(__i586__) && !defined(_M_IX86)
#define _M_IX86 500
#elif defined(__i486__) && !defined(_M_IX86)
#define _M_IX86 400
#elif defined(__i386__) && !defined(_M_IX86)
#define _M_IX86 300
#endif

#if defined(_M_IX86) && !defined(_X86_)
#define _X86_
#elif defined(_M_ALPHA) && !defined(_ALPHA_)
#define _ALPHA_
#elif defined(_M_PPC) && !defined(_PPC_)
#define _PPC_
#elif defined(_M_MRX000) && !defined(_MIPS_)
#define _MIPS_
#elif defined(_M_M68K) && !defined(_68K_)
#define _68K_
#endif

#ifdef RC_INVOKED
/* winresrc.h includes the necessary headers */
#include <winresrc.h>
#else

#include <stdarg.h>
#include <windef.h>
#include <wincon.h>
#include <winbase.h>
#ifndef NOGDI
#include <wingdi.h>
#endif
#include <winuser.h>
#include <winnls.h>
#include <winver.h>
#include <winnetwk.h>
#include <winreg.h>
#include <winsvc.h>

#ifndef WIN32_LEAN_AND_MEAN
#include <cderr.h>
#include <dde.h>
#include <ddeml.h>
#include <dlgs.h>
#include <imm.h>
#include <lzexpand.h>
#include <mmsystem.h>
#include <nb30.h>
#include <rpc.h>
#include <shellapi.h>
#include <winperf.h>

#ifndef NOGDI
#include <commdlg.h>
#include <winspool.h>
#endif

/* __USE_W32_SOCKETS is a __CYGWIN__ guard */
#if defined(__USE_W32_SOCKETS) || !(defined(__CYGWIN__) || defined(__MSYS__))
#include <winsock.h>
#endif

#ifndef NOGDI
/* In older versions we disallowed COM declarations in __OBJC__
   because of conflicts with @interface directive.  Define _OBJC_NO_COM
   to keep this behaviour.  */ 
#if !defined (_OBJC_NO_COM) 
#include <ole2.h>
#endif /* _OBJC_NO_COM */
#endif

#endif /* WIN32_LEAN_AND_MEAN */

#endif /* RC_INVOKED */

#ifdef __OBJC__
/* FIXME: Not undefining BOOL here causes all BOOLs to be WINBOOL (int),
   but undefining it causes trouble as well if a file is included after
   windows.h
*/
#undef BOOL
#endif

#endif
