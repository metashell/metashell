/**
 * @file locale.h
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
#ifndef	_LOCALE_H
#define	_LOCALE_H
#pragma GCC system_header
#include <_mingw.h>

/* 
 * Functions and types for localization (ie. changing the appearance of
 * output based on the standards of a certain country).
 */

/*
 * NOTE: I have tried to test this, but I am limited by my knowledge of
 *       locale issues. The structure does not bomb if you look at the
 *       values, and 'decimal_point' even seems to be correct. But the
 *       rest of the values are, by default, not particularly useful
 *       (read meaningless and not related to the international settings
 *       of the system).
 */

#define	LC_ALL		0
#define LC_COLLATE	1
#define LC_CTYPE	2
#define	LC_MONETARY	3
#define	LC_NUMERIC	4
#define	LC_TIME		5
#define	LC_MIN		LC_ALL
#define	LC_MAX		LC_TIME

#ifndef RC_INVOKED

/* According to C89 std, NULL is defined in locale.h too.  */
#define __need_NULL
#include <stddef.h>

/*
 * The structure returned by 'localeconv'.
 */
struct lconv
{
	char*	decimal_point;
	char*	thousands_sep;
	char*	grouping;
	char*	int_curr_symbol;
	char*	currency_symbol;
	char*	mon_decimal_point;
	char*	mon_thousands_sep;
	char*	mon_grouping;
	char*	positive_sign;
	char*	negative_sign;
	char	int_frac_digits;
	char	frac_digits;
	char	p_cs_precedes;
	char	p_sep_by_space;
	char	n_cs_precedes;
	char	n_sep_by_space;
	char	p_sign_posn;
	char	n_sign_posn;
};

#ifdef	__cplusplus
extern "C" {
#endif

_CRTIMP  char* __cdecl __MINGW_NOTHROW setlocale (int, const char*);
_CRTIMP struct lconv* __cdecl __MINGW_NOTHROW localeconv (void);

#ifndef _WLOCALE_DEFINED  /* also declared in wchar.h */
# define __need_wchar_t
# include <stddef.h>
  _CRTIMP wchar_t* __cdecl __MINGW_NOTHROW _wsetlocale(int, const wchar_t*);
# define _WLOCALE_DEFINED
#endif /* ndef _WLOCALE_DEFINED */

#ifdef	__cplusplus
}
#endif

#endif	/* Not RC_INVOKED */

#endif	/* Not _LOCALE_H */

