/**
 * @file qedit.h
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
#ifndef _QEDIT_H
#define _QEDIT_H
#pragma GCC system_header
#include <_mingw.h>

#include <strmif.h>

#ifdef __cplusplus
extern "C" {
#endif

/*--- DirectShow Reference - Constants and GUIDs - Error and Success Codes */
#define S_WARN_OUTPUTRESET 0x00009DD4
#define E_NOTINTREE 0x80040400
#define E_RENDER_ENGINE_IS_BROKEN 0x80040401
#define E_MUST_INIT_RENDERER 0x80040402
#define E_NOTDETERMINED 0x80040403
#define E_NO_TIMELINE 0x80040404
/*--- DirectShow Editing Services - C++ Reference - Structures */
typedef struct {
	BSTR Name;
	DISPID dispID;
	LONG nValues;
} DEXTER_PARAM;
typedef struct {
	VARIANT v;
	REFERENCE_TIME rt;
	DWORD dwInterp;
} DEXTER_VALUE;
typedef struct _SCompFmt0 {
	long nFormatId;
	AM_MEDIA_TYPE MediaType;
} SCompFmt0;
/*--- DirectShow Editing Services - C++ Reference - Enumerated Types */
typedef enum {
	DEXTERF_BOUNDING = -1,
	DEXTERF_EXACTLY_AT = 0,
	DEXTERF_FORWARDS = 1
} DEXTERF_TRACK_SEARCH_FLAGS;
typedef enum {
	TIMELINE_MAJOR_TYPE_COMPOSITE = 1,
	TIMELINE_MAJOR_TYPE_TRACK = 2,
	TIMELINE_MAJOR_TYPE_SOURCE = 4,
	TIMELINE_MAJOR_TYPE_TRANSITION = 8,
	TIMELINE_MAJOR_TYPE_EFFECT = 16,
	TIMELINE_MAJOR_TYPE_GROUP = 128
} TIMELINE_MAJOR_TYPE;
/*--- DirectShow Editing Services - C++ Reference - Constants */
#define CONNECTF_DYNAMIC_NONE 0x00
#define CONNECTF_DYNAMIC_SOURCES 0x01
#define CONNECTF_DYNAMIC_EFFECTS 0x02
#define SFN_VALIDATEF_CHECK 0x01
#define SFN_VALIDATEF_POPUP 0x02
#define SFN_VALIDATEF_TELLME 0x04
#define SFN_VALIDATEF_REPLACE 0x08
#define SFN_VALIDATEF_USELOCAL 0x10
#define SFN_VALIDATEF_NOFIND 0x20
#define SFN_VALIDATEF_IGNOREMUTED 0x40
#define RESIZEF_STRETCH 0
#define RESIZEF_CROP 1
#define RESIZEF_PRESERVEASPECTRATIO 2
#define RESIZEF_PRESERVEASPECTRATIO_NOLETTERBOX 3

#ifdef __cplusplus
}
#endif

#endif
