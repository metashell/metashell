/**
 * @file gdipluseffects.h
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
#ifndef __GDIPLUS_EFFECTS_H
#define __GDIPLUS_EFFECTS_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * GDI+ filters and effects
 */

typedef enum CurveAdjustments {
	AdjustExposure = 0,
	AdjustDensity = 1,
	AdjustContrast = 2,
	AdjustHighlight = 3,
	AdjustShadow = 4,
	AdjustMidtone = 5,
	AdjustWhiteSaturation = 6,
	AdjustBlackSaturation = 7
} CurveAdjustments;

typedef enum CurveChannel {
	CurveChannelAll = 0,
	CurveChannelRed = 1,
	CurveChannelGreen = 2,
	CurveChannelBlue = 3
} CurveChannel;

typedef struct BlurParams {
	REAL radius;
	BOOL expandEdge;
} BlurParams;

typedef struct BrightnessContrastParams {
	INT brightnessLevel;
	INT contrastLevel;
} BrightnessContrastParams;

typedef struct ColorBalanceParams {
	INT cyanRed;
	INT magentaGreen;
	INT yellowBlue;
} ColorBalanceParams;

typedef struct ColorCurveParams {
	CurveAdjustments adjustment;
	CurveChannel channel;
	INT adjustValue;
} ColorCurveParams;

typedef struct ColorLUTParams {
	ColorChannelLUT lutB;
	ColorChannelLUT lutG;
	ColorChannelLUT lutR;
	ColorChannelLUT lutA;
} ColorLUTParams;

typedef struct HueSaturationLightnessParams {
	INT hueLevel;
	INT saturationLevel;
	INT lightnessLevel;
} HueSaturationLightnessParams;

typedef struct LevelsParams {
	INT highlight;
	INT midtone;
	INT shadow;
} LevelsParams;

typedef struct RedEyeCorrectionParams {
	UINT numberOfAreas;
	RECT *areas;
} RedEyeCorrectionParams;

typedef struct SharpenParams {
	REAL radius;
	REAL amount;
} SharpenParams;

typedef struct TintParams {
	INT hue;
	INT amount;
} TintParams;

extern const GUID BlurEffectGuid;                    /* ? */
extern const GUID BrightnessContrastEffectGuid;      /* ? */
extern const GUID ColorBalanceEffectGuid;            /* ? */
extern const GUID ColorCurveEffectGuid;              /* ? */
extern const GUID ColorLUTEffectGuid;                /* ? */
extern const GUID ColorMatrixEffectGuid;             /* ? */
extern const GUID HueSaturationLightnessEffectGuid;  /* ? */
extern const GUID LevelsEffectGuid;                  /* ? */
extern const GUID RedEyeCorrectionEffectGuid;        /* ? */
extern const GUID SharpenEffectGuid;                 /* ? */
extern const GUID TintEffectGuid;                    /* ? */


#endif /* __GDIPLUS_EFFECTS_H */
