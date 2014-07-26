/**
 * @file newdev.h
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
/* Created by Casper S. Hornstrup <chorns@users.sourceforge.net> */
#ifndef __NEWDEV_H
#define __NEWDEV_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Driver installation DLL interface
 */

#ifdef __cplusplus
extern "C" {
#endif

/* UpdateDriverForPlugAndPlayDevices.InstallFlags constants */
#define INSTALLFLAG_FORCE                 0x00000001
#define INSTALLFLAG_READONLY              0x00000002
#define INSTALLFLAG_NONINTERACTIVE        0x00000004
#define INSTALLFLAG_BITS                  0x00000007

BOOL WINAPI
UpdateDriverForPlugAndPlayDevicesA(
  HWND  hwndParent,
  LPCSTR  HardwareId,
  LPCSTR  FullInfPath,
  DWORD  InstallFlags,
  PBOOL  bRebootRequired  /*OPTIONAL*/);

BOOL WINAPI
UpdateDriverForPlugAndPlayDevicesW(
  HWND  hwndParent,
  LPCWSTR  HardwareId,
  LPCWSTR  FullInfPath,
  DWORD  InstallFlags,
  PBOOL  bRebootRequired  /*OPTIONAL*/);

#define UpdateDriverForPlugAndPlayDevices __AW(UpdateDriverForPlugAndPlayDevices)

#ifdef __cplusplus
}
#endif

#endif /* __NEWDEV_H */
