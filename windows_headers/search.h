/**
 * @file search.h
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
/* Created by Danny Smith  <dannysmith@users.sourceforge.net> */
#ifndef _SEARCH_H
#define _SEARCH_H
#pragma GCC system_header
#include <_mingw.h>

#ifndef RC_INVOKED

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _SIZE_T_DEFINED
typedef unsigned int size_t;
#define _SIZE_T_DEFINED
#endif

/* bsearch and qsort are also declared in stdlib.h */
_CRTIMP void* __cdecl bsearch (const void*, const void*, size_t, size_t, 
			       int (*)(const void*, const void*));
_CRTIMP void __cdecl qsort (void*, size_t, size_t,
			    int (*)(const void*, const void*));

_CRTIMP void* __cdecl _lfind (const void*, const void*, unsigned int*,
			      unsigned int, int (*)(const void*, const void*));
_CRTIMP void* __cdecl _lsearch (const void*, void*, unsigned int*, unsigned int,
				int (*)(const void*, const void*));
/*
Documentation for these POSIX definitions and prototypes can be found in 
The Open Group Base Specifications Issue 6
IEEE Std 1003.1, 2004 Edition.
eg:  http://www.opengroup.org/onlinepubs/009695399/functions/twalk.html
*/


typedef struct entry {
	char *key;
	void *data;
} ENTRY;

typedef enum {
	FIND,
	ENTER
} ACTION;

typedef enum {
	preorder,
	postorder,
	endorder,
	leaf
} VISIT;

#ifdef _SEARCH_PRIVATE
typedef struct node {
	char         *key;
	struct node  *llink, *rlink;
} node_t;
#endif

void * __cdecl tdelete (const void * __restrict__, void ** __restrict__,
			int (*)(const void *, const void *))
			__MINGW_ATTRIB_NONNULL (2)  __MINGW_ATTRIB_NONNULL (3);
void * __cdecl tfind (const void *, void * const *,
		      int (*)(const void *, const void *))
		      __MINGW_ATTRIB_NONNULL (2)  __MINGW_ATTRIB_NONNULL (3);
void * __cdecl tsearch (const void *, void **, 
			int (*)(const void *, const void *))
			__MINGW_ATTRIB_NONNULL (2)  __MINGW_ATTRIB_NONNULL (3);
void __cdecl twalk (const void *, void (*)(const void *, VISIT, int));

#ifndef	_NO_OLDNAMES
_CRTIMP void* __cdecl lfind (const void*, const void*, unsigned int*,
			     unsigned int, int (*)(const void*, const void*));
_CRTIMP void* __cdecl lsearch (const void*, void*, unsigned int*, unsigned int,
			       int (*)(const void*, const void*));
#endif

#ifdef __cplusplus
}
#endif

#endif /* RC_INVOKED */
#endif /*  _SEARCH_H */
