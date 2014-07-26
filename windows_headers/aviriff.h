/**
 * @file aviriff.h
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
#ifndef _AVIRIFF_H
#define _AVIRIFF_H
#pragma GCC system_header
#include <_mingw.h>

#include <mmsystem.h>
#include <wingdi.h>

#ifdef __cplusplus
extern "C" {
#endif

/*--- DirectShow Reference - DirectShow Structures */
typedef struct _avimainheader {
	FOURCC fcc;
	DWORD cb;
	DWORD dwMicroSecPerFrame;
	DWORD dwMaxBytesPerSec;
	DWORD dwPaddingGranularity;
	DWORD dwFlags;
	DWORD dwTotalFrames;
	DWORD dwInitialFrames;
	DWORD dwStreams;
	DWORD dwSuggestedBufferSize;
	DWORD dwWidth;
	DWORD dwHeight;
	DWORD dwReserved[4];
} AVIMAINHEADER;
#define AVIF_COPYRIGHTED 0x00020000
#define AVIF_HASINDEX 0x00000010
#define AVIF_ISINTERLEAVED 0x00000100
#define AVIF_MUSTUSEINDEX 0x00000020
#define AVIF_WASCAPTUREFILE 0x00010000
typedef struct _avioldindex {
	FOURCC fcc;
	DWORD cb;
	struct _avioldindex_entry {
		DWORD dwChunkId;
		DWORD dwFlags;
		DWORD dwOffset;
		DWORD dwSize;
	} aIndex[];
} AVIOLDINDEX;
#define AVIIF_LIST 0x00000001
#define AVIIF_KEYFRAME 0x00000010
#define AVIIF_NO_TIME 0x00000100
typedef struct {
	BYTE bFirstEntry;
	BYTE bNumEntries;
	WORD wFlags;
	PALETTEENTRY peNew[];
} AVIPALCHANGE;
typedef struct _avistreamheader {
	FOURCC fcc;
	DWORD cb;
	FOURCC fccType;
	FOURCC fccHandler;
	DWORD dwFlags;
	WORD wPriority;
	WORD wLanguage;
	DWORD dwInitialFrames;
	DWORD dwScale;
	DWORD dwRate;
	DWORD dwStart;
	DWORD dwLength;
	DWORD dwSuggestedBufferSize;
	DWORD dwQuality;
	DWORD dwSampleSize;
	struct {
		short int left;
		short int top;
		short int right;
		short int bottom;
	} rcFrame;
} AVISTREAMHEADER;
#define AVISF_DISABLED 0x00000001
#define AVISF_VIDEO_PALCHANGES 0x00010000

#ifdef __cplusplus
}
#endif
#endif
