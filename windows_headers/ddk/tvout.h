/**
 * @file tvout.h
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
#ifndef __TVOUT_H
#define __TVOUT_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Definitions for TV-out support
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"

/* VIDEOPARAMETERS.dwCommand constants */
#define VP_COMMAND_GET                    0x00000001
#define VP_COMMAND_SET                    0x00000002

/* VIDEOPARAMETERS.dwFlags constants */
#define VP_FLAGS_TV_MODE                  0x00000001
#define VP_FLAGS_TV_STANDARD              0x00000002
#define VP_FLAGS_FLICKER                  0x00000004
#define VP_FLAGS_OVERSCAN                 0x00000008
#define VP_FLAGS_MAX_UNSCALED             0x00000010
#define VP_FLAGS_POSITION                 0x00000020
#define VP_FLAGS_BRIGHTNESS               0x00000040
#define VP_FLAGS_CONTRAST                 0x00000080
#define VP_FLAGS_COPYPROTECT              0x00000100

/* VIDEOPARAMETERS.dwMode constants */
#define VP_MODE_WIN_GRAPHICS              0x00000001
#define VP_MODE_TV_PLAYBACK               0x00000002

/* VIDEOPARAMETERS.dwTVStandard/dwAvailableTVStandard constants */
#define VP_TV_STANDARD_NTSC_M             0x00000001
#define VP_TV_STANDARD_NTSC_M_J           0x00000002
#define VP_TV_STANDARD_PAL_B              0x00000004
#define VP_TV_STANDARD_PAL_D              0x00000008
#define VP_TV_STANDARD_PAL_H              0x00000010
#define VP_TV_STANDARD_PAL_I              0x00000020
#define VP_TV_STANDARD_PAL_M              0x00000040
#define VP_TV_STANDARD_PAL_N              0x00000080
#define VP_TV_STANDARD_SECAM_B            0x00000100
#define VP_TV_STANDARD_SECAM_D            0x00000200
#define VP_TV_STANDARD_SECAM_G            0x00000400
#define VP_TV_STANDARD_SECAM_H            0x00000800
#define VP_TV_STANDARD_SECAM_K            0x00001000
#define VP_TV_STANDARD_SECAM_K1           0x00002000
#define VP_TV_STANDARD_SECAM_L            0x00004000
#define VP_TV_STANDARD_WIN_VGA            0x00008000
#define VP_TV_STANDARD_NTSC_433           0x00010000
#define VP_TV_STANDARD_PAL_G              0x00020000
#define VP_TV_STANDARD_PAL_60             0x00040000
#define VP_TV_STANDARD_SECAM_L1           0x00080000

/* VIDEOPARAMETERS.dwMode constants */
#define VP_CP_TYPE_APS_TRIGGER            0x00000001
#define VP_CP_TYPE_MACROVISION            0x00000002

/* VIDEOPARAMETERS.dwCPCommand constants */
#define VP_CP_CMD_ACTIVATE                0x00000001
#define VP_CP_CMD_DEACTIVATE              0x00000002
#define VP_CP_CMD_CHANGE                  0x00000004

typedef struct _VIDEOPARAMETERS {
  GUID  Guid;
  DWORD  dwOffset;
  DWORD  dwCommand;
  DWORD  dwFlags;
  DWORD  dwMode;
  DWORD  dwTVStandard;
  DWORD  dwAvailableModes;
  DWORD  dwAvailableTVStandard;
  DWORD  dwFlickerFilter;
  DWORD  dwOverScanX;
  DWORD  dwOverScanY;
  DWORD  dwMaxUnscaledX;
  DWORD  dwMaxUnscaledY;
  DWORD  dwPositionX;
  DWORD  dwPositionY;
  DWORD  dwBrightness;
  DWORD  dwContrast;
  DWORD  dwCPType;
  DWORD  dwCPCommand;
  DWORD  dwCPStandard;
  DWORD  dwCPKey;
  BYTE  bCP_APSTriggerBits;
  BYTE  bOEMCopyProtection[256];
} VIDEOPARAMETERS, *PVIDEOPARAMETERS, FAR *LPVIDEOPARAMETERS;

#ifdef __cplusplus
}
#endif

#endif /* __TVOUT_H */
