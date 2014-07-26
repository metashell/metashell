/**
 * @file userenv.h
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
#ifndef _USERENV_H
#define _USERENV_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PI_NOUI (1)
#define PI_APPLYPOLICY (2)

typedef struct _PROFILEINFOA {
  DWORD dwSize;
  DWORD dwFlags;
  LPSTR lpUserName;
  LPSTR lpProfilePath;
  LPSTR lpDefaultPath;
  LPSTR lpServerName;
  LPSTR lpPolicyPath;
  HANDLE hProfile;
} PROFILEINFOA, *LPPROFILEINFOA;
typedef struct _PROFILEINFOW {
  DWORD dwSize;
  DWORD dwFlags;
  LPWSTR lpUserName;
  LPWSTR lpProfilePath;
  LPWSTR lpDefaultPath;
  LPWSTR lpServerName;
  LPWSTR lpPolicyPath;
  HANDLE hProfile;
} PROFILEINFOW, *LPPROFILEINFOW;
BOOL WINAPI LoadUserProfileA(HANDLE,LPPROFILEINFOA);
BOOL WINAPI LoadUserProfileW(HANDLE,LPPROFILEINFOW);
BOOL WINAPI UnloadUserProfile(HANDLE,HANDLE);
BOOL WINAPI GetProfilesDirectoryA(LPSTR,LPDWORD);
BOOL WINAPI GetProfilesDirectoryW(LPWSTR,LPDWORD);
BOOL WINAPI GetUserProfileDirectoryA(HANDLE,LPSTR,LPDWORD);
BOOL WINAPI GetUserProfileDirectoryW(HANDLE,LPWSTR,LPDWORD);
BOOL WINAPI CreateEnvironmentBlock(LPVOID*,HANDLE,BOOL);
BOOL WINAPI DestroyEnvironmentBlock(LPVOID);

typedef __AW(PROFILEINFO) PROFILEINFO;
typedef __AW(LPPROFILEINFO) LPPROFILEINFO;
#define LoadUserProfile  __AW(LoadUserProfile)
#define GetProfilesDirectory  __AW(GetProfilesDirectory)
#define GetUserProfileDirectory  __AW(GetUserProfileDirectory)

#if (_WIN32_WINNT >= _WIN32_WINNT_WIN2K)
#define PT_TEMPORARY (1)
#define PT_ROAMING (2)
#define PT_MANDATORY (4)
BOOL WINAPI DeleteProfileA(LPCSTR,LPCSTR,LPCSTR);
BOOL WINAPI DeleteProfileW(LPCWSTR,LPCWSTR,LPCWSTR);
BOOL WINAPI GetProfileType(DWORD *);
BOOL WINAPI GetAllUsersProfileDirectoryA(LPSTR,LPDWORD);
BOOL WINAPI GetAllUsersProfileDirectoryW(LPWSTR,LPDWORD);
BOOL WINAPI GetDefaultUserProfileDirectoryA(LPSTR,LPDWORD);
BOOL WINAPI GetDefaultUserProfileDirectoryW(LPWSTR,LPDWORD);
BOOL WINAPI ExpandEnvironmentStringsForUserA(HANDLE,LPCSTR,LPSTR,DWORD);
BOOL WINAPI ExpandEnvironmentStringsForUserW(HANDLE,LPCWSTR,LPWSTR,DWORD);
#define DeleteProfile  __AW(DeleteProfile)
#define GetAllUsersProfileDirectory  __AW(GetAllUsersProfileDirectory)
#define GetDefaultUserProfileDirectory  __AW(GetDefaultUserProfileDirectory)
#define ExpandEnvironmentStringsForUser  __AW(ExpandEnvironmentStringsForUser)
#endif /* (_WIN32_WINNT >= _WIN32_WINNT_WIN2K) */

#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
HRESULT WINAPI CreateProfile(LPCWSTR,LPCWSTR,LPWSTR,DWORD);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _USERENV_H */
