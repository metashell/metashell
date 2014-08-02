/**
 * @file process.h
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
#ifndef	_PROCESS_H_
#define	_PROCESS_H_
#pragma GCC system_header
#include <_mingw.h>

/* 
 * Function calls for spawning child processes.
 */

#include <sys/types.h> /* For _pid_t and pid_t. */
#include <stdint.h>  /* For intptr_t. */
/*
 * Constants for cwait actions.
 * Obsolete for Win32.
 */
#define	_WAIT_CHILD		0
#define	_WAIT_GRANDCHILD	1

#ifndef	_NO_OLDNAMES
#define	WAIT_CHILD		_WAIT_CHILD
#define	WAIT_GRANDCHILD		_WAIT_GRANDCHILD
#endif	/* Not _NO_OLDNAMES */

/*
 * Mode constants for spawn functions.
 */
#define	_P_WAIT		0
#define	_P_NOWAIT	1
#define	_P_OVERLAY	2
#define	_OLD_P_OVERLAY	_P_OVERLAY
#define	_P_NOWAITO	3
#define	_P_DETACH	4

#ifndef	_NO_OLDNAMES
#define	P_WAIT		_P_WAIT
#define	P_NOWAIT	_P_NOWAIT
#define	P_OVERLAY	_P_OVERLAY
#define	OLD_P_OVERLAY	_OLD_P_OVERLAY
#define	P_NOWAITO	_P_NOWAITO
#define	P_DETACH	_P_DETACH
#endif	/* Not _NO_OLDNAMES */


#ifndef RC_INVOKED

#ifdef	__cplusplus
extern "C" {
#endif

_CRTIMP void __cdecl __MINGW_NOTHROW _cexit(void);
_CRTIMP void __cdecl __MINGW_NOTHROW _c_exit(void);

_CRTIMP int __cdecl __MINGW_NOTHROW _cwait (int*, _pid_t, int);

_CRTIMP _pid_t __cdecl __MINGW_NOTHROW _getpid(void);

_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _execl	(const char*, const char*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _execle	(const char*, const char*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _execlp	(const char*, const char*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _execlpe	(const char*, const char*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _execv	(const char*, const char* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _execve	(const char*, const char* const*, const char* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _execvp	(const char*, const char* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _execvpe	(const char*, const char* const*, const char* const*);

_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _spawnl	(int, const char*, const char*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _spawnle	(int, const char*, const char*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _spawnlp	(int, const char*, const char*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _spawnlpe	(int, const char*, const char*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _spawnv	(int, const char*, const char* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _spawnve	(int, const char*, const char* const*, const char* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _spawnvp	(int, const char*, const char* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _spawnvpe	(int, const char*, const char* const*, const char* const*);

#ifndef _WPROCESS_DEFINED
/* Also in wchar.h - keep in sync */
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wexecl	(const wchar_t*, const wchar_t*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wexecle	(const wchar_t*, const wchar_t*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wexeclp	(const wchar_t*, const wchar_t*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wexeclpe	(const wchar_t*, const wchar_t*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wexecv	(const wchar_t*, const wchar_t* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wexecve	(const wchar_t*, const wchar_t* const*, const wchar_t* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wexecvp	(const wchar_t*, const wchar_t* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wexecvpe	(const wchar_t*, const wchar_t* const*, const wchar_t* const*);

_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wspawnl	(int, const wchar_t*, const wchar_t*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wspawnle	(int, const wchar_t*, const wchar_t*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wspawnlp	(int, const wchar_t*, const wchar_t*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wspawnlpe	(int, const wchar_t*, const wchar_t*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wspawnv	(int, const wchar_t*, const wchar_t* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wspawnve	(int, const wchar_t*, const wchar_t* const*, const wchar_t* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wspawnvp	(int, const wchar_t*, const wchar_t* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW _wspawnvpe	(int, const wchar_t*, const wchar_t* const*, const wchar_t* const*);

#define _WPROCESS_DEFINED
#endif

/*
 * NOTE: Apparently _endthread calls CloseHandle on the handle of the thread,
 * making for race conditions if you are not careful. Basically you have to
 * make sure that no-one is going to do *anything* with the thread handle
 * after the thread calls _endthread or returns from the thread function.
 *
 * NOTE: No old names for these functions. Use the underscore.
 */
_CRTIMP unsigned long __cdecl __MINGW_NOTHROW
	_beginthread	(void (*)(void *), unsigned, void*);
_CRTIMP void __cdecl __MINGW_NOTHROW _endthread	(void);

_CRTIMP unsigned long __cdecl __MINGW_NOTHROW
	_beginthreadex	(void *, unsigned, unsigned (__stdcall *) (void *), 
			 void*, unsigned, unsigned*);
_CRTIMP void __cdecl __MINGW_NOTHROW _endthreadex (unsigned);

#ifndef	_NO_OLDNAMES
/*
 * Functions without the leading underscore, for portability. These functions
 * live in liboldnames.a.
 */
_CRTIMP int  __cdecl __MINGW_NOTHROW cwait (int*, pid_t, int);
_CRTIMP pid_t __cdecl __MINGW_NOTHROW getpid (void);

_CRTIMP intptr_t __cdecl __MINGW_NOTHROW execl	(const char*, const char*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW execle	(const char*, const char*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW execlp	(const char*, const char*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW execlpe	(const char*, const char*,...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW execv	(const char*, const char* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW execve	(const char*, const char* const*, const char* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW execvp	(const char*, const char* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW execvpe	(const char*, const char* const*, const char* const*);

_CRTIMP intptr_t __cdecl __MINGW_NOTHROW spawnl	(int, const char*, const char*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW spawnle	(int, const char*, const char*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW spawnlp	(int, const char*, const char*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW spawnlpe	(int, const char*, const char*, ...);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW spawnv	(int, const char*, const char* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW spawnve	(int, const char*, const char* const*, const char* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW spawnvp	(int, const char*, const char* const*);
_CRTIMP intptr_t __cdecl __MINGW_NOTHROW spawnvpe	(int, const char*, const char* const*, const char* const*);
#endif	/* Not _NO_OLDNAMES */

#ifdef	__cplusplus
}
#endif

#endif	/* Not RC_INVOKED */

#endif	/* _PROCESS_H_ not defined */
