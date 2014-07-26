/**
 * @file gdipluslinecaps.h
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
#ifndef __GDIPLUS_LINECAPS_H
#define __GDIPLUS_LINECAPS_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * GDI+ AdjustableArrowCap class
 */

#ifndef __cplusplus
#error "A C++ compiler is required to include gdipluslinecaps.h."
#endif

class AdjustableArrowCap: public CustomLineCap
{
public:
	AdjustableArrowCap(REAL height, REAL width, BOOL isFilled):
		CustomLineCap(NULL, Ok)
	{
		GpAdjustableArrowCap *nativeAdjustableArrowCap = NULL;
		lastStatus = DllExports::GdipCreateAdjustableArrowCap(
				height, width, isFilled,
				&nativeAdjustableArrowCap);
		nativeCustomLineCap = nativeAdjustableArrowCap;
	}
	virtual ~AdjustableArrowCap()
	{
	}
	virtual AdjustableArrowCap* Clone() const
	{
		GpCustomLineCap *cloneCustomLineCap = NULL;
		Status status = updateStatus(DllExports::GdipCloneCustomLineCap(
				nativeCustomLineCap, &cloneCustomLineCap));
		if (status == Ok) {
			AdjustableArrowCap *result = new AdjustableArrowCap(
					cloneCustomLineCap, lastStatus);
			if (!result) {
				DllExports::GdipDeleteCustomLineCap(
						cloneCustomLineCap);
				lastStatus = OutOfMemory;
			}
			return result;
		} else {
			return NULL;
		}
	}

	REAL GetHeight() const
	{
		REAL result = 0.0f;
		updateStatus(DllExports::GdipGetAdjustableArrowCapHeight(
				(GpAdjustableArrowCap*) nativeCustomLineCap,
				&result));
		return result;
	}
	REAL GetMiddleInset() const
	{
		REAL result = 0.0f;
		updateStatus(DllExports::GdipGetAdjustableArrowCapMiddleInset(
				(GpAdjustableArrowCap*) nativeCustomLineCap,
				&result));
		return result;
	}
	REAL GetWidth() const
	{
		REAL result = 0.0f;
		updateStatus(DllExports::GdipGetAdjustableArrowCapWidth(
				(GpAdjustableArrowCap*) nativeCustomLineCap,
				&result));
		return result;
	}
	BOOL IsFilled() const
	{
		BOOL result = FALSE;
		updateStatus(DllExports::GdipGetAdjustableArrowCapFillState(
				(GpAdjustableArrowCap*) nativeCustomLineCap,
				&result));
		return result;
	}
	Status SetFillState(BOOL isFilled)
	{
		return updateStatus(DllExports::GdipSetAdjustableArrowCapFillState(
				(GpAdjustableArrowCap*) nativeCustomLineCap,
				isFilled));
	}
	Status SetHeight(REAL height)
	{
		return updateStatus(DllExports::GdipSetAdjustableArrowCapHeight(
				(GpAdjustableArrowCap*) nativeCustomLineCap,
				height));
	}
	Status SetMiddleInset(REAL middleInset)
	{
		return updateStatus(DllExports::GdipSetAdjustableArrowCapMiddleInset(
				(GpAdjustableArrowCap*) nativeCustomLineCap,
				middleInset));
	}
	Status SetWidth(REAL width)
	{
		return updateStatus(DllExports::GdipSetAdjustableArrowCapWidth(
				(GpAdjustableArrowCap*) nativeCustomLineCap,
				width));
	}

private:
	AdjustableArrowCap(GpCustomLineCap *customLineCap, Status status):
		CustomLineCap(customLineCap, status) {}
	AdjustableArrowCap(const AdjustableArrowCap&);
	AdjustableArrowCap& operator=(const AdjustableArrowCap&);
};

#endif /* __GDIPLUS_LINECAPS_H */
