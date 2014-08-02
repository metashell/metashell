/**
 * @file vptype.h
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
#ifndef _VPTYPE_H
#define _VPTYPE_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

/*--- DirectShow Reference - DirectShow Enumerated Types */
typedef enum _AMVP_MODE {
	AMVP_MODE_WEAVE,
	AMVP_MODE_BOBINTERLEAVED,
	AMVP_MODE_BOBNONINTERLEAVED,
	AMVP_MODE_SKIPEVEN,
	AMVP_MODE_SKIPODD
} AMVP_MODE;
typedef enum _AMVP_SELECT_FORMAT_BY {
	AMVP_DO_NOT_CARE,
	AMVP_BEST_BANDWIDTH,
	AMVP_INPUT_SAME_AS_OUTPUT
} AMVP_SELECT_FORMAT_BY;
/*--- DirectShow Reference - DirectShow Structures */
typedef struct _AMVPDIMINFO{
	DWORD dwFieldWidth;
	DWORD dwFieldHeight;
	DWORD dwVBIWidth;
	DWORD dwVBIHeight;
	RECT rcValidRegion;
} AMVPDIMINFO,*LPAMVPDIMINFO;
typedef struct _AMVPDATAINFO{
	DWORD dwSize;
	DWORD dwMicrosecondsPerField;
	AMVPDIMINFO amvpDimInfo;
	DWORD dwPictAspectRatioX;
	DWORD dwPictAspectRatioY;
	BOOL bEnableDoubleClock;
	BOOL bEnableVACT;
	BOOL bDataIsInterlaced;
	LONG lHalfLinesOdd;
	BOOL bFieldPolarityInverted;
	DWORD dwNumLinesInVREF;
	LONG lHalfLinesEven;
	DWORD dwReserved1;
} AMVPDATAINFO,*LPAMVPDATAINFO;
typedef struct _AMVPSIZE{
	DWORD dwWidth;
	DWORD dwHeight;
} AMVPSIZE,*LPAMVPSIZE;

#ifdef __cplusplus
}
#endif

#endif
