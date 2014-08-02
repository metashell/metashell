/**
 * @file gdipluspixelformats.h
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
/* Created by Markus Koenig <markus@stber-koenig.de> */
#ifndef __GDIPLUS_PIXELFORMATS_H
#define __GDIPLUS_PIXELFORMATS_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * GDI+ pixel formats
 */

typedef DWORD ARGB;
typedef INT PixelFormat;

#define PixelFormatIndexed ((INT) 0x00010000)
#define PixelFormatGDI ((INT) 0x00020000)
#define PixelFormatAlpha ((INT) 0x00040000)
#define PixelFormatPAlpha ((INT) 0x00080000)
#define PixelFormatExtended ((INT) 0x00100000)
#define PixelFormatCanonical ((INT) 0x00200000)
#define PixelFormatUndefined ((INT) 0)
#define PixelFormatDontCare ((INT) 0)
#define PixelFormat1bppIndexed ((INT) \
	(1 | (1<<8) | PixelFormatIndexed | PixelFormatGDI))
#define PixelFormat4bppIndexed ((INT) \
	(2 | (4<<8) | PixelFormatIndexed | PixelFormatGDI))
#define PixelFormat8bppIndexed ((INT) \
	(3 | (8<<8) | PixelFormatIndexed | PixelFormatGDI))
#define PixelFormat16bppGrayScale ((INT) \
	(4 | (16<<8) | PixelFormatExtended))
#define PixelFormat16bppRGB555 ((INT) \
	(5 | (16<<8) | PixelFormatGDI))
#define PixelFormat16bppRGB565 ((INT) \
	(6 | (16<<8) | PixelFormatGDI))
#define PixelFormat16bppARGB1555 ((INT) \
	(7 | (16<<8) | PixelFormatAlpha | PixelFormatGDI))
#define PixelFormat24bppRGB ((INT) \
	(8 | (24<<8) | PixelFormatGDI))
#define PixelFormat32bppRGB ((INT) \
	(9 | (32<<8) | PixelFormatGDI))
#define PixelFormat32bppARGB ((INT) \
	(10 | (32<<8) | PixelFormatAlpha | PixelFormatGDI | PixelFormatCanonical))
#define PixelFormat32bppPARGB ((INT) \
	(11 | (32<<8) | PixelFormatAlpha | PixelFormatPAlpha | PixelFormatGDI))
#define PixelFormat48bppRGB ((INT) \
	(12 | (48<<8) | PixelFormatExtended))
#define PixelFormat64bppARGB ((INT) \
	(13 | (64<<8) | PixelFormatAlpha | PixelFormatCanonical | PixelFormatExtended))
#define PixelFormat64bppPARGB ((INT) \
	(14 | (64<<8) | PixelFormatAlpha | PixelFormatPAlpha | PixelFormatExtended))
#define PixelFormatMax ((INT) 15)

typedef enum PaletteFlags {
	PaletteFlagsHasAlpha = 1,
	PaletteFlagsGrayScale = 2,
	PaletteFlagsHalftone = 4
} PaletteFlags;

typedef enum PaletteType {
	PaletteTypeCustom = 0,
	PaletteTypeOptimal = 1,
	PaletteTypeFixedBW = 2,
	PaletteTypeFixedHalftone8 = 3,
	PaletteTypeFixedHalftone27 = 4,
	PaletteTypeFixedHalftone64 = 5,
	PaletteTypeFixedHalftone125 = 6,
	PaletteTypeFixedHalftone216 = 7,
	PaletteTypeFixedHalftone252 = 8,
	PaletteTypeFixedHalftone256 = 9 
} PaletteType;

typedef struct ColorPalette {
	UINT Flags;
	UINT Count;
	ARGB Entries[1];
} ColorPalette;

static __inline__ UINT GetPixelFormatSize(PixelFormat pixfmt)
{
	return (((UINT) pixfmt) & 0xff00U) >> 8;
}

static __inline__ BOOL IsAlphaPixelFormat(PixelFormat pixfmt)
{
	return (pixfmt & PixelFormatAlpha) != 0;
}

static __inline__ BOOL IsCanonicalPixelFormat(PixelFormat pixfmt)
{
	return (pixfmt & PixelFormatCanonical) != 0;
}

static __inline__ BOOL IsExtendedPixelFormat(PixelFormat pixfmt)
{
	return (pixfmt & PixelFormatExtended) != 0;
}

static __inline__ BOOL IsIndexedPixelFormat(PixelFormat pixfmt)
{
	return (pixfmt & PixelFormatIndexed) != 0;
}

#endif /* __GDIPLUS_PIXELFORMATS_H */
