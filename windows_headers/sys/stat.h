/**
 * @file stat.h
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
#ifndef _STAT_H_
#define _STAT_H_
#ifndef __CRT_TESTING__
#pragma GCC system_header
#endif
#include <_mingw.h>

#define __need_size_t
#define __need_wchar_t
#ifndef RC_INVOKED
#include <stddef.h>
#endif /* Not RC_INVOKED */

#include <sys/types.h>
#include <string.h> /* Need memset declaration */

/*
 * Constants for the stat st_mode member.
 */
#define	_S_IFIFO	0x1000	/* FIFO */
#define	_S_IFCHR	0x2000	/* Character */
#define	_S_IFBLK	0x3000	/* Block: Is this ever set under w32? */
#define	_S_IFDIR	0x4000	/* Directory */
#define	_S_IFREG	0x8000	/* Regular */

#define	_S_IFMT		0xF000	/* File type mask */

#define	_S_IEXEC	0x0040
#define	_S_IWRITE	0x0080
#define	_S_IREAD	0x0100

#define	_S_IRWXU	(_S_IREAD | _S_IWRITE | _S_IEXEC)
#define	_S_IXUSR	_S_IEXEC
#define	_S_IWUSR	_S_IWRITE
#define	_S_IRUSR	_S_IREAD

#define	_S_ISDIR(m)	(((m) & _S_IFMT) == _S_IFDIR)
#define	_S_ISFIFO(m)	(((m) & _S_IFMT) == _S_IFIFO)
#define	_S_ISCHR(m)	(((m) & _S_IFMT) == _S_IFCHR)
#define	_S_ISBLK(m)	(((m) & _S_IFMT) == _S_IFBLK)
#define	_S_ISREG(m)	(((m) & _S_IFMT) == _S_IFREG)

#ifndef _NO_OLDNAMES

#define	S_IFIFO		_S_IFIFO
#define	S_IFCHR		_S_IFCHR
#define	S_IFBLK		_S_IFBLK
#define	S_IFDIR		_S_IFDIR
#define	S_IFREG		_S_IFREG
#define	S_IFMT		_S_IFMT
#define	S_IEXEC		_S_IEXEC
#define	S_IWRITE	_S_IWRITE
#define	S_IREAD		_S_IREAD
#define	S_IRWXU		_S_IRWXU
#define	S_IXUSR		_S_IXUSR
#define	S_IWUSR		_S_IWUSR
#define	S_IRUSR		_S_IRUSR

#define	S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define	S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)
#define	S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define	S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
#define	S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)

#endif	/* Not _NO_OLDNAMES */

#ifndef RC_INVOKED

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef _STAT_DEFINED
/*
 * The structure manipulated and returned by stat and fstat.
 *
 * NOTE: If called on a directory the values in the time fields are not only
 * invalid, they will cause localtime et. al. to return NULL. And calling
 * asctime with a NULL pointer causes an Invalid Page Fault. So watch it!
 */
struct _stat32
{
	_dev_t	   st_dev;	/* Equivalent to drive number 0=A 1=B ... */
	_ino_t	   st_ino;	/* Always zero ? */
	_mode_t	   st_mode;	/* See above constants */
	short	   st_nlink;	/* Number of links. */
	short	   st_uid;	/* User: Maybe significant on NT ? */
	short	   st_gid;	/* Group: Ditto */
	_dev_t	   st_rdev;	/* Seems useless (not even filled in) */
	_off_t	   st_size;	/* File size in bytes */
	__time32_t st_atime;	/* Accessed date (always 00:00 hrs local
				 * on FAT) */
	__time32_t st_mtime;	/* Modified time */
	__time32_t st_ctime;	/* Creation time */
};

#ifndef __STRICT_ANSI__
struct stat {
	_dev_t	   st_dev;	/* Equivalent to drive number 0=A 1=B ... */
	_ino_t	   st_ino;	/* Always zero ? */
	_mode_t    st_mode;	/* See above constants */
	short	   st_nlink;	/* Number of links. */
	short	   st_uid;	/* User: Maybe significant on NT ? */
	short	   st_gid;	/* Group: Ditto */
	_dev_t	   st_rdev;	/* Seems useless (not even filled in) */
	_off_t	   st_size;	/* File size in bytes */
	time_t	   st_atime;	/* Accessed date (always 00:00 hrs local
				 * on FAT) */
	time_t	   st_mtime;	/* Modified time */
	time_t	   st_ctime;	/* Creation time */
};
#endif /* __STRICT_ANSI__ */

struct _stat64 {
	dev_t	   st_dev;	/* Equivalent to drive number 0=A 1=B ... */
	ino_t	   st_ino;	/* Always zero ? */
	mode_t	   st_mode;	/* See above constants */
	short	   st_nlink;	/* Number of links. */
	short	   st_uid;	/* User: Maybe significant on NT ? */
	short	   st_gid;	/* Group: Ditto */
	dev_t	   st_rdev;	/* Seems useless (not even filled in) */
	_off64_t   st_size;	/* File size in bytes */
	__time64_t st_atime;	/* Accessed date (always 00:00 hrs local
				 * on FAT) */
	__time64_t st_mtime;	/* Modified time */
	__time64_t st_ctime;	/* Creation time */
};

struct _stat32i64 {
	_dev_t		st_dev;
	_ino_t		st_ino;
	_mode_t		st_mode;
	short		st_nlink;
	short		st_uid;
	short		st_gid;
	_dev_t		st_rdev;
	_off64_t	st_size;
	__time32_t	st_atime;
	__time32_t	st_mtime;
	__time32_t	st_ctime;
};

struct _stat64i32 {
	_dev_t		st_dev;
	_ino_t		st_ino;
	_mode_t		st_mode;
	short		st_nlink;
	short		st_uid;
	short		st_gid;
	_dev_t		st_rdev;
	_off_t		st_size;
	__time64_t	st_atime;
	__time64_t	st_mtime;
	__time64_t	st_ctime;
};

/* _stat32 does not exist in MSVCRT.DLL */
_CRTIMP int __cdecl __MINGW_NOTHROW _stat (const char*, struct _stat32*);
_CRTALIAS int __cdecl __MINGW_NOTHROW _stat32 (const char* _v1, struct _stat32* _v2) {
  return _stat(_v1, _v2);
}

_CRTIMP int __cdecl __MINGW_NOTHROW _stat64 (const char*, struct _stat64*);
int __cdecl __MINGW_NOTHROW _stat32i64 (const char*, struct _stat32i64*);
int __cdecl __MINGW_NOTHROW _stat64i32 (const char*, struct _stat64i32*);

/* _fstat32 does not exist in MSVCRT.DLL */
_CRTIMP int __cdecl __MINGW_NOTHROW _fstat (int, struct _stat32*);
_CRTALIAS int __cdecl __MINGW_NOTHROW _fstat32 (int _v1, struct _stat32* _v2) {
    return _fstat(_v1, _v2);
}

_CRTIMP int __cdecl __MINGW_NOTHROW _fstat64 (int, struct _stat64*);
int __cdecl __MINGW_NOTHROW _fstat32i64 (int, struct _stat32i64*);
int __cdecl __MINGW_NOTHROW _fstat64i32 (int, struct _stat64i32*);

__CRT_MAYBE_INLINE int __cdecl _fstat64i32(int desc, struct _stat64i32 *_stat) {
  struct _stat64 st;
  int ret = _fstat64(desc, &st);
  if (ret == -1) {
    memset(_stat, 0, sizeof(struct _stat64i32));
    return -1;
  }
  _stat->st_dev = st.st_dev;
  _stat->st_ino = st.st_ino;
  _stat->st_mode = st.st_mode;
  _stat->st_nlink = st.st_nlink;
  _stat->st_uid = st.st_uid;
  _stat->st_gid = st.st_gid;
  _stat->st_rdev = st.st_rdev;
  _stat->st_size = (_off_t) st.st_size;
  _stat->st_atime = st.st_atime;
  _stat->st_mtime = st.st_mtime;
  _stat->st_ctime = st.st_ctime;
  return ret;
}
__CRT_MAYBE_INLINE int __cdecl _fstat32i64(int desc, struct _stat32i64 *_stat) {
  struct _stat32 st;
  int ret = _fstat32(desc, &st);
  if (ret == -1) {
    memset(_stat, 0, sizeof(struct _stat32i64));
    return -1;
  }
  _stat->st_dev = st.st_dev;
  _stat->st_ino = st.st_ino;
  _stat->st_mode = st.st_mode;
  _stat->st_nlink = st.st_nlink;
  _stat->st_uid = st.st_uid;
  _stat->st_gid = st.st_gid;
  _stat->st_rdev = st.st_rdev;
  _stat->st_size = (_off_t) st.st_size;
  _stat->st_atime = st.st_atime;
  _stat->st_mtime = st.st_mtime;
  _stat->st_ctime = st.st_ctime;
  return ret;
}
__CRT_MAYBE_INLINE int __cdecl _stat64i32(const char *fname, struct _stat64i32 *_stat) {
  struct _stat64 st;
  int ret = _stat64(fname, &st);
  if (ret == -1) {
    memset(_stat, 0, sizeof(struct _stat64i32));
    return -1;
  }
  _stat->st_dev = st.st_dev;
  _stat->st_ino = st.st_ino;
  _stat->st_mode = st.st_mode;
  _stat->st_nlink = st.st_nlink;
  _stat->st_uid = st.st_uid;
  _stat->st_gid = st.st_gid;
  _stat->st_rdev = st.st_rdev;
  _stat->st_size = (_off_t) st.st_size;
  _stat->st_atime = st.st_atime;
  _stat->st_mtime = st.st_mtime;
  _stat->st_ctime = st.st_ctime;
  return ret;
}
__CRT_MAYBE_INLINE int __cdecl _stat32i64(const char *fname, struct _stat32i64 *_stat) {
  struct _stat32 st;
  int ret = _stat32(fname, &st);
  if (ret == -1) {
    memset(_stat, 0, sizeof(struct _stat32i64));
    return -1;
  }
  _stat->st_dev = st.st_dev;
  _stat->st_ino = st.st_ino;
  _stat->st_mode = st.st_mode;
  _stat->st_nlink = st.st_nlink;
  _stat->st_uid = st.st_uid;
  _stat->st_gid = st.st_gid;
  _stat->st_rdev = st.st_rdev;
  _stat->st_size = (_off_t) st.st_size;
  _stat->st_atime = st.st_atime;
  _stat->st_mtime = st.st_mtime;
  _stat->st_ctime = st.st_ctime;
  return ret;
}

#define __stat64 _stat64
#if defined(_USE_32BIT_TIME_T)
#define _fstat      _fstat32
#define _fstati64   _fstat32i64
#define _stat       _stat32
#define _stati64    _stat32i64

#else  /* !_USE_32BIT_TIME_T */
#define _fstat      _fstat64i32
#define _fstati64   _fstat64
#define _stat       _stat64i32
#define _stati64    _stat64

#endif /* _USE_32BIT_TIME_T */
#define _STAT_DEFINED

#endif /* _STAT_DEFINED */

#if !defined(_NO_OLDNAMES) && !defined(__STRICT_ANSI__)
#define stat _stat
#define fstat _fstat
#endif /* !defined(_NO_OLDNAMES) && !defined(__STRICT_ANSI__) */

#if !defined ( _WSTAT_DEFINED) /* also declared in wchar.h */
/* _wstat32 does not exist in MSVCRT.DLL */
_CRTIMP int __cdecl __MINGW_NOTHROW _wstat (const wchar_t*, struct _stat32*);
_CRTALIAS int __cdecl __MINGW_NOTHROW _wstat32 (const wchar_t* _v1, struct _stat32* _v2) {
    return _wstat(_v1, _v2);
}

_CRTIMP int __cdecl __MINGW_NOTHROW _wstat64 (const wchar_t*, struct _stat64*);
_CRTIMP int __cdecl __MINGW_NOTHROW _wstat32i64 (const wchar_t*, struct _stat32i64*);
int __cdecl __MINGW_NOTHROW _wstat64i32 (const wchar_t*, struct _stat64i32*);
__CRT_MAYBE_INLINE int __cdecl _wstat64i32(const wchar_t *fname, struct _stat64i32 *_stat) {
  struct _stat64 st;
  int ret = _wstat64(fname, &st);
  if (ret == -1) {
    memset(_stat, 0, sizeof(struct _stat64i32));
    return -1;
  }
  _stat->st_dev = st.st_dev;
  _stat->st_ino = st.st_ino;
  _stat->st_mode = st.st_mode;
  _stat->st_nlink = st.st_nlink;
  _stat->st_uid = st.st_uid;
  _stat->st_gid = st.st_gid;
  _stat->st_rdev = st.st_rdev;
  _stat->st_size = (_off_t) st.st_size;
  _stat->st_atime = st.st_atime;
  _stat->st_mtime = st.st_mtime;
  _stat->st_ctime = st.st_ctime;
  return ret;
}
__CRT_MAYBE_INLINE int __cdecl _wstat32i64(const wchar_t *fname, struct _stat32i64 *_stat) {
  struct _stat32 st;
  int ret = _wstat32(fname, &st);
  if (ret == -1) {
    memset(_stat, 0, sizeof(struct _stat32i64));
    return -1;
  }
  _stat->st_dev = st.st_dev;
  _stat->st_ino = st.st_ino;
  _stat->st_mode = st.st_mode;
  _stat->st_nlink = st.st_nlink;
  _stat->st_uid = st.st_uid;
  _stat->st_gid = st.st_gid;
  _stat->st_rdev = st.st_rdev;
  _stat->st_size = (_off_t) st.st_size;
  _stat->st_atime = st.st_atime;
  _stat->st_mtime = st.st_mtime;
  _stat->st_ctime = st.st_ctime;
  return ret;
}

#if defined(_USE_32BIT_TIME_T)
#define _wstat      _wstat32
#define _wstati64   _wstat32i64

#else /* ! _USE_32BIT_TIME_T */
#define _wstat      _wstat64i32
#define _wstati64   _wstat64

#endif /* _USE_32BIT_TIME_T */

#define _WSTAT_DEFINED
#endif /* ! _WSTAT_DEFIND */


#ifdef	__cplusplus
}
#endif

#endif	/* Not RC_INVOKED */

#endif	/* Not _STAT_H_ */
