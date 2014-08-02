/**
 * @file types.h
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
#ifndef	_TYPES_H_
#define	_TYPES_H_
#pragma GCC system_header
#include <_mingw.h>

#define __need_wchar_t
#define __need_size_t
#define __need_ptrdiff_t
#ifndef RC_INVOKED
#include <stddef.h>
#endif	/* Not RC_INVOKED */

#ifndef RC_INVOKED

/*
 * A type for storing the current time and date. This is the number of
 * seconds since midnight Jan 1, 1970.
 * NOTE: This is also defined in time.h.
 */
#ifndef _TIME32_T_DEFINED
typedef __int32 __time32_t;
#define _TIME32_T_DEFINED
#endif

#ifndef _TIME64_T_DEFINED
typedef __int64 __time64_t;
#define _TIME64_T_DEFINED
#endif

#ifndef _TIME_T_DEFINED
/* FIXME __STRICT_ANSI__ ! */

#if defined(_USE_32BIT_TIME_T)
typedef	__time32_t time_t;
#else

typedef	__time64_t time_t;
#endif /* _USE_32BIT_TIME_T */

#define _TIME_T_DEFINED
#endif

#ifndef	_OFF_T_
#define	_OFF_T_
typedef long _off_t;
#ifndef __STRICT_ANSI__
typedef _off_t	off_t;
#endif /* __STRICT_ANSI__ */
#endif	/* Not _OFF_T_ */

#ifndef _OFF64_T_
#define _OFF64_T_
typedef __int64 _off64_t;
#ifndef __STRICT_ANSI__
typedef __int64 off64_t;
#endif /* __STRICT_ANSI__ */
#endif /* ndef _OFF64_T */

#ifndef _DEV_T_
#define	_DEV_T_
typedef unsigned int _dev_t;

#ifndef	_NO_OLDNAMES
typedef _dev_t	dev_t;
#endif
#endif	/* Not _DEV_T_ */


#ifndef _INO_T_
#define	_INO_T_
typedef short _ino_t;

#ifndef	_NO_OLDNAMES
typedef _ino_t	ino_t;
#endif
#endif	/* Not _INO_T_ */


#ifndef _PID_T_
#define	_PID_T_
typedef int	_pid_t;

#ifndef	_NO_OLDNAMES
typedef _pid_t	pid_t;
#endif
#endif	/* Not _PID_T_ */


#ifndef _MODE_T_
#define	_MODE_T_
typedef unsigned short _mode_t;

#ifndef	_NO_OLDNAMES
typedef _mode_t	mode_t;
#endif
#endif	/* Not _MODE_T_ */


#ifndef _SIGSET_T_
#define	_SIGSET_T_
typedef int	_sigset_t;

#ifndef	_NO_OLDNAMES
typedef _sigset_t	sigset_t;
#endif
#endif	/* Not _SIGSET_T_ */

#ifndef _SSIZE_T_
#define _SSIZE_T_
typedef int _ssize_t;

#ifndef	_NO_OLDNAMES
typedef _ssize_t ssize_t;
#endif
#endif /* Not _SSIZE_T_ */

#ifndef _FPOS64_T_
#define _FPOS64_T_
typedef long long fpos64_t;
#endif

#ifndef _OFF64_T_
#define _OFF64_T_
typedef long long off64_t;
#endif

#if !defined __NO_ISOCEXT
typedef unsigned int useconds_t;
#endif  /* Not __NO_ISOCEXT */

#endif	/* Not RC_INVOKED */

#endif	/* Not _TYPES_H_ */
