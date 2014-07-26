/**
 * @file specstrings.h
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
#ifndef _SPECSTRINGS_H
#define _SPECSTRINGS_H
#pragma GCC system_header
#include <_mingw.h>

/* These are built from primitives in Microsoft's Standard Annotation Language.
   Search for "Header Annotation" on MSDN site for their MS-specific meanings */

/* __in and __out currently conflict with libstdc++, use with caution */


#define __bcount(size)
#define __bcount_opt(size)
#define __deref_bcount(size)
#define __deref_bcount_opt(size)
#define __deref_ecount(size)
#define __deref_ecount_opt(size)
#define __deref_in
#define __deref_in_bcount(size)
#define __deref_in_bcount_opt(size)
#define __deref_in_ecount(size)
#define __deref_in_ecount_opt(size)
#define __deref_in_opt
#define __deref_inout
#define __deref_inout_bcount(size)
#define __deref_inout_bcount_full(size)
#define __deref_inout_bcount_full_opt(size)
#define __deref_inout_bcount_opt(size)
#define __deref_inout_bcount_part(size,length)
#define __deref_inout_bcount_part_opt(size,length)
#define __deref_inout_ecount(size)
#define __deref_inout_ecount_full(size)
#define __deref_inout_ecount_full_opt(size)
#define __deref_inout_ecount_opt(size)
#define __deref_inout_ecount_part(size,length)
#define __deref_inout_ecount_part_opt(size,length)
#define __deref_inout_opt
#define __deref_opt_bcount(size)
#define __deref_opt_bcount_opt(size)
#define __deref_opt_ecount(size)
#define __deref_opt_ecount_opt(size)
#define __deref_opt_in
#define __deref_opt_in_bcount(size)
#define __deref_opt_in_bcount_opt(size)
#define __deref_opt_in_ecount(size)
#define __deref_opt_in_ecount_opt(size)
#define __deref_opt_in_opt
#define __deref_opt_inout
#define __deref_opt_inout_bcount(size)
#define __deref_opt_inout_bcount_full(size)
#define __deref_opt_inout_bcount_full_opt(size)
#define __deref_opt_inout_bcount_opt(size)
#define __deref_opt_inout_bcount_part(size,length)
#define __deref_opt_inout_bcount_part_opt(size,length)
#define __deref_opt_inout_ecount(size)
#define __deref_opt_inout_ecount_full(size)
#define __deref_opt_inout_ecount_full_opt(size)
#define __deref_opt_inout_ecount_opt(size)
#define __deref_opt_inout_ecount_part(size,length)
#define __deref_opt_inout_ecount_part_opt(size,length)
#define __deref_opt_inout_opt
#define __deref_opt_out
#define __deref_opt_out_bcount(size)
#define __deref_opt_out_bcount_full(size)
#define __deref_opt_out_bcount_full_opt(size)
#define __deref_opt_out_bcount_opt(size)
#define __deref_opt_out_bcount_part(size,length)
#define __deref_opt_out_bcount_part_opt(size,length)
#define __deref_opt_out_ecount(size)
#define __deref_opt_out_ecount_full(size)
#define __deref_opt_out_ecount_full_opt(size)
#define __deref_opt_out_ecount_opt(size)
#define __deref_opt_out_ecount_part(size,length)
#define __deref_opt_out_ecount_part_opt(size,length)
#define __deref_opt_out_opt
#define __deref_out
#define __deref_out_bcount(size)
#define __deref_out_bcount_full(size)
#define __deref_out_bcount_full_opt(size)
#define __deref_out_bcount_opt(size)
#define __deref_out_bcount_part(size,length)
#define __deref_out_bcount_part_opt(size,length)
#define __deref_out_ecount(size)
#define __deref_out_ecount_full(size)
#define __deref_out_ecount_full_opt(size)
#define __deref_out_ecount_opt(size)
#define __deref_out_ecount_part(size,length)
#define __deref_out_ecount_part_opt(size,length)
#define __deref_out_opt
#define __ecount(size)
#define __ecount_opt(size)
#define __in
#define __in_bcount(size)
#define __in_bcount_opt(size)
#define __in_ecount(size)
#define __in_ecount_opt(size)
#define __in_opt
#define __inout
#define __inout_bcount(size)
#define __inout_bcount_full(size)
#define __inout_bcount_full_opt(size)
#define __inout_bcount_opt(size)
#define __inout_bcount_part(size,length)
#define __inout_bcount_part_opt(size,length)
#define __inout_ecount(size)
#define __inout_ecount_full(size)
#define __inout_ecount_full_opt(size)
#define __inout_ecount_opt(size)
#define __inout_ecount_part(size,length)
#define __inout_ecount_part_opt(size,length)
#define __inout_opt
#define __out
#define __out_bcount(size)
#define __out_bcount_full(size)
#define __out_bcount_full_opt(size)
#define __out_bcount_opt(size)
#define __out_bcount_part(size,length)
#define __out_bcount_part_opt(size,length)
#define __out_ecount(size)
#define __out_ecount_full(size)
#define __out_ecount_full_opt(size)
#define __out_ecount_opt(size)
#define __out_ecount_part(size,length)
#define __out_ecount_part_opt(size,length)
#define __out_opt 

#endif /*_SPECSTRINGS_H */
