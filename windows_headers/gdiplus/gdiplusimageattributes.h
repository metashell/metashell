/**
 * @file gdiplusimageattributes.h
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
#ifndef __GDIPLUS_IMAGEATTRIBUTES_H
#define __GDIPLUS_IMAGEATTRIBUTES_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * GDI+ ImageAttributes class
 */

#ifndef __cplusplus
#error "A C++ compiler is required to include gdiplusimageattributes.h."
#endif

class ImageAttributes: public GdiplusBase
{
	friend class Graphics;
	friend class TextureBrush;

public:
	ImageAttributes(): nativeImageAttributes(NULL), lastStatus(Ok)
	{
		lastStatus = DllExports::GdipCreateImageAttributes(
				&nativeImageAttributes);
	}
	~ImageAttributes()
	{
		DllExports::GdipDisposeImageAttributes(nativeImageAttributes);
	}
	ImageAttributes* Clone() const
	{
		GpImageAttributes *cloneImageAttributes = NULL;
		Status status = updateStatus(DllExports::GdipCloneImageAttributes(
				nativeImageAttributes, &cloneImageAttributes));
		if (status == Ok) {
			ImageAttributes *result = new ImageAttributes(
					cloneImageAttributes, lastStatus);
			if (!result) {
				DllExports::GdipDisposeImageAttributes(cloneImageAttributes);
				lastStatus = OutOfMemory;
			}
			return result;
		} else {
			return NULL;
		}
	}

	Status ClearBrushRemapTable()
	{
		return updateStatus(DllExports::GdipSetImageAttributesRemapTable(
				nativeImageAttributes, ColorAdjustTypeBrush,
				FALSE, 0, NULL));
	}
	Status ClearColorKey(ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesColorKeys(
				nativeImageAttributes, type, FALSE, 0, 0));
	}
	Status ClearColorMatrices(ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesColorMatrix(
				nativeImageAttributes, type, FALSE,
				NULL, NULL, ColorMatrixFlagsDefault));
	}
	Status ClearColorMatrix(ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesColorMatrix(
				nativeImageAttributes, type, FALSE,
				NULL, NULL, ColorMatrixFlagsDefault));
	}
	Status ClearGamma(ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesGamma(
				nativeImageAttributes, type, FALSE, 1.0f));
	}
	Status ClearNoOp(ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesNoOp(
				nativeImageAttributes, type, FALSE));
	}
	Status ClearOutputChannel(ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesOutputChannel(
				nativeImageAttributes, type, FALSE,
				ColorChannelFlagsC));
	}
	Status ClearOutputChannelColorProfile(
			ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesOutputChannelColorProfile(
				nativeImageAttributes, type, FALSE, NULL));
	}
	Status ClearRemapTable(ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesRemapTable(
				nativeImageAttributes, type, FALSE, 0, NULL));
	}
	Status ClearThreshold(ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesThreshold(
				nativeImageAttributes, type, FALSE, 0.0));
	}
	Status GetAdjustedPalette(ColorPalette *colorPalette,
			ColorAdjustType type) const
	{
		return updateStatus(DllExports::GdipGetImageAttributesAdjustedPalette(
				nativeImageAttributes, colorPalette, type));
	}
	Status GetLastStatus() const
	{
		Status result = lastStatus;
		lastStatus = Ok;
		return result;
	}
	Status Reset(ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipResetImageAttributes(
				nativeImageAttributes, type));
	}
	Status SetBrushRemapTable(UINT mapSize, ColorMap *map)
	{
		return updateStatus(DllExports::GdipSetImageAttributesRemapTable(
				nativeImageAttributes, ColorAdjustTypeBrush,
				TRUE, mapSize, map));
	}
	Status SetColorKey(const Color& colorLow, const Color& colorHigh,
			ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesColorKeys(
				nativeImageAttributes, type, TRUE,
				colorLow.GetValue(), colorHigh.GetValue()));
	}
	Status SetColorMatrices(const ColorMatrix *colorMatrix,
			const ColorMatrix *grayMatrix,
			ColorMatrixFlags mode = ColorMatrixFlagsDefault,
			ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesColorMatrix(
				nativeImageAttributes, type, TRUE,
				colorMatrix, grayMatrix, mode));
	}
	Status SetColorMatrix(const ColorMatrix *colorMatrix,
			ColorMatrixFlags mode = ColorMatrixFlagsDefault,
			ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesColorMatrix(
				nativeImageAttributes, type, TRUE,
				colorMatrix, NULL, mode));
	}
	Status SetGamma(REAL gamma,
			ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesGamma(
				nativeImageAttributes, type, TRUE, gamma));
	}
	Status SetNoOp(ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesNoOp(
				nativeImageAttributes, type, TRUE));
	}
	Status SetOutputChannel(ColorChannelFlags channelFlags,
			ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesOutputChannel(
				nativeImageAttributes, type, TRUE,
				channelFlags));
	}
	Status SetOutputChannelColorProfile(const WCHAR *colorProfileFilename,
			ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesOutputChannelColorProfile(
				nativeImageAttributes, type, TRUE,
				colorProfileFilename));
	}
	Status SetRemapTable(UINT mapSize, const ColorMap *map,
			ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesRemapTable(
				nativeImageAttributes, type, TRUE,
				mapSize, map));
	}
	Status SetThreshold(REAL threshold,
			ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesThreshold(
				nativeImageAttributes, type, TRUE, threshold));
	}
	Status SetToIdentity(ColorAdjustType type = ColorAdjustTypeDefault)
	{
		return updateStatus(DllExports::GdipSetImageAttributesToIdentity(
				nativeImageAttributes, type));
	}
	Status SetWrapMode(WrapMode wrap, const Color& color = Color(),
			BOOL clamp = FALSE)
	{
		return updateStatus(DllExports::GdipSetImageAttributesWrapMode(
				nativeImageAttributes, wrap,
				color.GetValue(), clamp));
	}

private:
	ImageAttributes(GpImageAttributes *imageAttributes, Status status):
		nativeImageAttributes(imageAttributes), lastStatus(status) {}
	ImageAttributes(const ImageAttributes&);
	ImageAttributes& operator=(const ImageAttributes&);

	Status updateStatus(Status newStatus) const
	{
		if (newStatus != Ok) lastStatus = newStatus;
		return newStatus;
	}

	GpImageAttributes *nativeImageAttributes;
	mutable Status lastStatus;
};


#endif /* __GDIPLUS_IMAGEATTRIBUTES_H */
