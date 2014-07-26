/**
 * @file gdipluscolormatrix.h
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
#ifndef __GDIPLUS_COLORMATRIX_H
#define __GDIPLUS_COLORMATRIX_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * GDI+ color mappings
 */

typedef enum ColorAdjustType {
	ColorAdjustTypeDefault = 0,
	ColorAdjustTypeBitmap = 1,
	ColorAdjustTypeBrush = 2,
	ColorAdjustTypePen = 3,
	ColorAdjustTypeText = 4,
	ColorAdjustTypeCount = 5,
	ColorAdjustTypeAny = 6
} ColorAdjustType;

typedef enum ColorMatrixFlags {
	ColorMatrixFlagsDefault = 0,
	ColorMatrixFlagsSkipGrays = 1,
	ColorMatrixFlagsAltGray = 2
} ColorMatrixFlags;

typedef enum HistogramFormat {
	HistogramFormatARGB = 0,
	HistogramFormatPARGB = 1,
	HistogramFormatRGB = 2,
	HistogramFormatGray = 3,
	HistogramFormatB = 4,
	HistogramFormatG = 5,
	HistogramFormatR = 6,
	HistogramFormatA = 7
} HistogramFormat;

typedef struct ColorMap {
	Color oldColor;
	Color newColor;
} ColorMap;

typedef struct ColorMatrix {
	REAL m[5][5];
} ColorMatrix;

typedef BYTE ColorChannelLUT[256];

#endif /* __GDIPLUS_COLORMATRIX_H */
