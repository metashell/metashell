/**
 * @file audevcod.h
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
#ifndef _AUDEVCOD_H
#define _AUDEVCOD_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _tagSND_DEVICE_ERROR {
	SNDDEV_ERROR_Open = 1,       
	SNDDEV_ERROR_Close = 2,       
	SNDDEV_ERROR_GetCaps = 3,       
	SNDDEV_ERROR_PrepareHeader = 4,
	SNDDEV_ERROR_UnprepareHeader = 5,       
	SNDDEV_ERROR_Reset = 6,
	SNDDEV_ERROR_Restart = 7,       
	SNDDEV_ERROR_GetPosition = 8,       
	SNDDEV_ERROR_Write = 9,
	SNDDEV_ERROR_Pause = 10,       
	SNDDEV_ERROR_Stop = 11,       
	SNDDEV_ERROR_Start = 12,       
	SNDDEV_ERROR_AddBuffer = 13,
	SNDDEV_ERROR_Query = 14
} SNDDEV_ERR;

#ifdef __cplusplus
}
#endif
#endif
