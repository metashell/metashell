/**
 * @file assert.h
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

/* We should be able to include this file multiple times to allow the assert
 * macro to be enabled/disabled for different parts of code.  So don't add a
 * header guard.
 */ 
#pragma GCC system_header

/**
 * Define the assert macro for debug output.
 */


#ifndef RC_INVOKED

/* All the headers include this file. */
#include <_mingw.h>

#undef assert

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef NDEBUG
/*
 * If not debugging, assert does nothing.
 */
#define assert(x)	((void)0)

#else /* debugging enabled */

/*
 * The runtime nicely supplies a function which does the actual output and
 * call to abort.
 */
_CRTIMP void __cdecl __MINGW_NOTHROW _assert (const char*, const char*, int) __MINGW_ATTRIB_NORETURN;

/*
 * Definition of the assert macro.
 */
#define assert(e)       ((e) ? (void)0 : _assert(#e, __FILE__, __LINE__))

#endif	/* NDEBUG */

#ifdef	__cplusplus
}
#endif

#endif /* Not RC_INVOKED */
