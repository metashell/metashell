/**
 * @file il21dec.h
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
#ifndef _IL21DEC_H
#define _IL21DEC_H
#pragma GCC system_header
#include <_mingw.h>

#ifdef __cplusplus
extern "C" {
#endif

/*--- DirectShow Reference - DirectShow Enumerated Types */
typedef enum _AM_LINE21_CCLEVEL {
	AM_L21_CCLEVEL_TC2
} AM_LINE21_CCLEVEL,*PAM_LINE21_CCLEVEL;
typedef enum _AM_LINE21_CCSERVICE {
	AM_L21_CCSERVICE_None,
	AM_L21_CCSERVICE_Caption1,
	AM_L21_CCSERVICE_Caption2,
	AM_L21_CCSERVICE_Text1,
	AM_L21_CCSERVICE_Text2,
	AM_L21_CCSERVICE_XDS,
} AM_LINE21_CCSERVICE,*PAM_LINE21_CCSERVICE;
typedef enum _AM_LINE21_CCSTATE {
	AM_L21_CCSTATE_Off,
	AM_L21_CCSTATE_On
} AM_LINE21_CCSTATE,*PAM_LINE21_CCSTATE;
typedef enum _AM_LINE21_DRAWBGMODE {
	AM_L21_DRAWBGMODE_Opaque,
	AM_L21_DRAWBGMODE_Transparent
} AM_LINE21_DRAWBGMODE,*PAM_LINE21_DRAWBGMODE;

#ifdef __cplusplus
}
#endif
#endif
