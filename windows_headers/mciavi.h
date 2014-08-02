/**
 * @file mciavi.h
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
#ifndef _MCIAVI_H
#define _MCIAVI_H
#pragma GCC system_header
#include <_mingw.h>

#define MCI_MCIAVI_PLAY_WINDOW 0x1000000
#define	MCI_MCIAVI_PLAY_FULLSCREEN 0x2000000
#define MCI_MCIAVI_PLAY_FULLBY2	0x4000000
#define MCI_AVI_STATUS_FRAMES_SKIPPED 0x8001
#define MCI_AVI_STATUS_LAST_PLAY_SPEED	0x8002
#define MCI_AVI_STATUS_AUDIO_BREAKS	0x8003
#define MCI_AVI_SETVIDEO_DRAW_PROCEDURE	0x8000
#define MCI_AVI_SETVIDEO_PALETTE_COLOR	0x8100
#define MCI_AVI_SETVIDEO_PALETTE_HALFTONE 0x0000FFFF
#define MCIERR_AVI_OLDAVIFORMAT	(MCIERR_CUSTOM_DRIVER_BASE+100)
#define MCIERR_AVI_NOTINTERLEAVED	(MCIERR_CUSTOM_DRIVER_BASE+101)
#define MCIERR_AVI_NODISPDIB	(MCIERR_CUSTOM_DRIVER_BASE+102)
#define MCIERR_AVI_CANTPLAYFULLSCREEN	(MCIERR_CUSTOM_DRIVER_BASE+103)
#define MCIERR_AVI_TOOBIGFORVGA	(MCIERR_CUSTOM_DRIVER_BASE+104)
#define MCIERR_AVI_NOCOMPRESSOR (MCIERR_CUSTOM_DRIVER_BASE+105)
#define MCIERR_AVI_DISPLAYERROR (MCIERR_CUSTOM_DRIVER_BASE+106)
#define MCIERR_AVI_AUDIOERROR	(MCIERR_CUSTOM_DRIVER_BASE+107)
#define MCIERR_AVI_BADPALETTE	(MCIERR_CUSTOM_DRIVER_BASE+108)
#endif
