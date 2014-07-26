/**
 * @file mcx.h
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
#ifndef _MCX_H
#define _MCX_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DIALOPTION_BILLING 64
#define DIALOPTION_QUIET 128
#define DIALOPTION_DIALTONE 256
#define MDMVOLFLAG_LOW	1
#define MDMVOLFLAG_MEDIUM	2
#define MDMVOLFLAG_HIGH	4
#define MDMVOL_LOW	0
#define MDMVOL_MEDIUM	1
#define MDMVOL_HIGH	2
#define MDMSPKRFLAG_OFF	1
#define MDMSPKRFLAG_DIAL	2
#define MDMSPKRFLAG_ON	4
#define MDMSPKRFLAG_CALLSETUP	8
#define MDMSPKR_OFF	0
#define MDMSPKR_DIAL 1
#define MDMSPKR_ON	2
#define MDMSPKR_CALLSETUP	3
#define MDM_COMPRESSION	1
#define MDM_ERROR_CONTROL	2
#define MDM_FORCED_EC	4
#define MDM_CELLULAR	8
#define MDM_FLOWCONTROL_HARD	16
#define MDM_FLOWCONTROL_SOFT	32
#define MDM_CCITT_OVERRIDE	64
#define MDM_SPEED_ADJUST	128
#define MDM_TONE_DIAL	256
#define MDM_BLIND_DIAL	512
#define MDM_V23_OVERRIDE	1024
typedef struct _MODEMDEVCAPS {
	DWORD dwActualSize;
	DWORD dwRequiredSize;
	DWORD dwDevSpecificOffset;
	DWORD dwDevSpecificSize;
	DWORD dwModemProviderVersion;
	DWORD dwModemManufacturerOffset;
	DWORD dwModemManufacturerSize;
	DWORD dwModemModelOffset;
	DWORD dwModemModelSize;
	DWORD dwModemVersionOffset;
	DWORD dwModemVersionSize;
	DWORD dwDialOptions;
	DWORD dwCallSetupFailTimer;
	DWORD dwInactivityTimeout;
	DWORD dwSpeakerVolume;
	DWORD dwSpeakerMode;
	DWORD dwModemOptions;
	DWORD dwMaxDTERate;
	DWORD dwMaxDCERate;
	BYTE abVariablePortion[1];
} MODEMDEVCAPS,*PMODEMDEVCAPS,*LPMODEMDEVCAPS;
typedef struct _MODEMSETTINGS {
	DWORD dwActualSize;
	DWORD dwRequiredSize;
	DWORD dwDevSpecificOffset;
	DWORD dwDevSpecificSize;
	DWORD dwCallSetupFailTimer;
	DWORD dwInactivityTimeout;
	DWORD dwSpeakerVolume;
	DWORD dwSpeakerMode;
	DWORD dwPreferredModemOptions;
	DWORD dwNegotiatedModemOptions;
	DWORD dwNegotiatedDCERate;
	BYTE abVariablePortion[1];
} MODEMSETTINGS,*PMODEMSETTINGS,*LPMODEMSETTINGS;

#ifdef __cplusplus
}
#endif

#endif /* _MCX_H */
