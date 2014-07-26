/**
 * @file malloc.h
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
#ifndef _MALLOC_H
#define _MALLOC_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Support for programs which want to use malloc.h to get memory management
 * functions. Unless you absolutely need some of these functions and they are
 * not in the ANSI headers you should use the ANSI standard header files
 * instead.
 */

#include <stdlib.h>

#ifndef RC_INVOKED

/*
 * The structure used to walk through the heap with _heapwalk.
 */
typedef	struct _heapinfo
{
	int*	_pentry;
	size_t	_size;
	int	_useflag;
} _HEAPINFO;

/* Values for _heapinfo.useflag */
#define _FREEENTRY 0
#define _USEDENTRY 1

/* Return codes for _heapwalk()  */
#define _HEAPEMPTY	(-1)
#define _HEAPOK		(-2)
#define _HEAPBADBEGIN	(-3)
#define _HEAPBADNODE	(-4)
#define _HEAPEND	(-5)
#define _HEAPBADPTR	(-6)

/* maximum size of a user request for memory */
#define _HEAP_MAXREQ  0xFFFFFFE0

#ifdef	__cplusplus
extern "C" {
#endif
/*
   The _heap* memory allocation functions are supported on NT
   but not W9x. On latter, they always set errno to ENOSYS.
*/
_CRTIMP int __cdecl __MINGW_NOTHROW _heapwalk (_HEAPINFO*);
#define _alloca(x) __builtin_alloca((x))

#ifndef	_NO_OLDNAMES
_CRTIMP int __cdecl __MINGW_NOTHROW heapwalk (_HEAPINFO*);
#define alloca(x) __builtin_alloca((x))
#endif	/* Not _NO_OLDNAMES */

_CRTIMP int __cdecl __MINGW_NOTHROW _heapchk (void);	/* Verify heap integrety. */
_CRTIMP int __cdecl __MINGW_NOTHROW _heapmin (void);	/* Return unused heap to the OS. */
_CRTIMP int __cdecl __MINGW_NOTHROW _heapset (unsigned int);

_CRTIMP size_t __cdecl __MINGW_NOTHROW _msize (void*);
_CRTIMP size_t __cdecl __MINGW_NOTHROW _get_sbh_threshold (void); 
_CRTIMP int __cdecl __MINGW_NOTHROW _set_sbh_threshold (size_t);
_CRTIMP void* __cdecl __MINGW_NOTHROW _expand (void*, size_t); 

_CRTIMP void * __cdecl __MINGW_NOTHROW _aligned_offset_malloc(size_t, size_t, size_t);
_CRTIMP void * __cdecl __MINGW_NOTHROW _aligned_offset_realloc(void*, size_t, size_t, size_t);
_CRTIMP void * __cdecl __MINGW_NOTHROW _aligned_offset_recalloc(void*, size_t, size_t, size_t, size_t);

_CRTIMP void * __cdecl __MINGW_NOTHROW _aligned_malloc (size_t, size_t);
_CRTIMP void * __cdecl __MINGW_NOTHROW _aligned_realloc (void*, size_t, size_t);
_CRTIMP void* __cdecl __MINGW_NOTHROW _aligned_recalloc(void*, size_t, size_t, size_t);
_CRTIMP void __cdecl __MINGW_NOTHROW _aligned_free (void*);

/* These require libmingwex.a. */ 
void * __cdecl __MINGW_NOTHROW __mingw_aligned_offset_malloc (size_t, size_t, size_t);
void * __cdecl __MINGW_NOTHROW __mingw_aligned_offset_realloc (void*, size_t, size_t, size_t);

void * __cdecl __MINGW_NOTHROW __mingw_aligned_malloc (size_t, size_t);
void * __cdecl __MINGW_NOTHROW __mingw_aligned_realloc (void*, size_t, size_t);
void __cdecl __MINGW_NOTHROW __mingw_aligned_free (void*);

#ifdef __cplusplus
}
#endif

#endif	/* RC_INVOKED */

#endif /* Not _MALLOC_H */
