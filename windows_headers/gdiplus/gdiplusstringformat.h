/**
 * @file gdiplusstringformat.h
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
#ifndef __GDIPLUS_STRINGFORMAT_H
#define __GDIPLUS_STRINGFORMAT_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * GDI+ StringFormat class
 */

#ifndef __cplusplus
#error "A C++ compiler is required to include gdiplusstringformat.h."
#endif

class StringFormat: public GdiplusBase
{
	friend class Graphics;
	friend class GraphicsPath;

public:
	static const StringFormat* GenericDefault();
	static const StringFormat* GenericTypographic();

	StringFormat(INT formatFlags = 0, LANGID language = LANG_NEUTRAL):
			nativeStringFormat(NULL), lastStatus(Ok)
	{
		lastStatus = DllExports::GdipCreateStringFormat(
				formatFlags, language, &nativeStringFormat);
	}
	StringFormat(const StringFormat *format):
			nativeStringFormat(NULL), lastStatus(Ok)
	{
		lastStatus = DllExports::GdipCloneStringFormat(
				format ? format->nativeStringFormat : NULL,
				&nativeStringFormat);
	}
	~StringFormat()
	{
		DllExports::GdipDeleteStringFormat(nativeStringFormat);
	}
	StringFormat* Clone() const
	{
		GpStringFormat *cloneStringFormat = NULL;
		Status status = updateStatus(DllExports::GdipCloneStringFormat(
				nativeStringFormat, &cloneStringFormat));
		if (status == Ok) {
			StringFormat *result = new StringFormat(
					cloneStringFormat, lastStatus);
			if (!result) {
				DllExports::GdipDeleteStringFormat(cloneStringFormat);
				lastStatus = OutOfMemory;
			}
			return result;
		} else {
			return NULL;
		}
	}

	StringAlignment GetAlignment() const
	{
		StringAlignment result = StringAlignmentNear;
		updateStatus(DllExports::GdipGetStringFormatAlign(
				nativeStringFormat, &result));
		return result;
	}
	LANGID GetDigitSubstitutionLanguage() const
	{
		LANGID result = 0;
		StringDigitSubstitute method;
		updateStatus(DllExports::GdipGetStringFormatDigitSubstitution(
				nativeStringFormat, &result, &method));
		return result;
	}
	StringDigitSubstitute GetDigitSubstitutionMethod() const
	{
		LANGID language;
		StringDigitSubstitute result = StringDigitSubstituteUser;
		updateStatus(DllExports::GdipGetStringFormatDigitSubstitution(
				nativeStringFormat, &language, &result));
		return result;
	}
	INT GetFormatFlags() const
	{
		INT result = 0;
		updateStatus(DllExports::GdipGetStringFormatFlags(
				nativeStringFormat, &result));
		return result;
	}
	HotkeyPrefix GetHotkeyPrefix() const
	{
		HotkeyPrefix result = HotkeyPrefixNone;
		updateStatus(DllExports::GdipGetStringFormatHotkeyPrefix(
				nativeStringFormat, (INT*) &result));
		return result;
	}
	Status GetLastStatus() const
	{
		Status result = lastStatus;
		lastStatus = Ok;
		return result;
	}
	StringAlignment GetLineAlignment() const
	{
		StringAlignment result = StringAlignmentNear;
		updateStatus(DllExports::GdipGetStringFormatLineAlign(
				nativeStringFormat, &result));
		return result;
	}
	INT GetMeasurableCharacterRangeCount() const
	{
		INT result = 0;
		updateStatus(DllExports::GdipGetStringFormatMeasurableCharacterRangeCount(
				nativeStringFormat, &result));
		return result;
	}
	INT GetTabStopCount() const
	{
		INT result = 0;
		updateStatus(DllExports::GdipGetStringFormatTabStopCount(
				nativeStringFormat, &result));
		return result;
	}
	Status GetTabStops(INT count, REAL *firstTabOffset, REAL *tabStops)
	{
		return updateStatus(DllExports::GdipGetStringFormatTabStops(
				nativeStringFormat, count,
				firstTabOffset, tabStops));
	}
	StringTrimming GetTrimming() const
	{
		StringTrimming result = StringTrimmingNone;
		updateStatus(DllExports::GdipGetStringFormatTrimming(
				nativeStringFormat, &result));
		return result;
	}
	Status SetAlignment(StringAlignment align)
	{
		return updateStatus(DllExports::GdipSetStringFormatAlign(
				nativeStringFormat, align));
	}
	Status SetDigitSubstitution(LANGID language,
			StringDigitSubstitute substitute)
	{
		return updateStatus(DllExports::GdipSetStringFormatDigitSubstitution(
				nativeStringFormat, language, substitute));
	}
	Status SetFormatFlags(INT flags)
	{
		return updateStatus(DllExports::GdipSetStringFormatFlags(
				nativeStringFormat, flags));
	}
	Status SetHotkeyPrefix(HotkeyPrefix hotkeyPrefix)
	{
		return updateStatus(DllExports::GdipSetStringFormatHotkeyPrefix(
				nativeStringFormat, (INT) hotkeyPrefix));
	}
	Status SetLineAlignment(StringAlignment align)
	{
		return updateStatus(DllExports::GdipSetStringFormatLineAlign(
				nativeStringFormat, align));
	}
	Status SetMeasurableCharacterRanges(INT rangeCount,
			const CharacterRange *ranges)
	{
		return updateStatus(DllExports::GdipSetStringFormatMeasurableCharacterRanges(
				nativeStringFormat, rangeCount, ranges));
	}
	Status SetTabStops(REAL firstTabOffset, INT count, const REAL *tabStops)
	{
		return updateStatus(DllExports::GdipSetStringFormatTabStops(
				nativeStringFormat, firstTabOffset,
				count, tabStops));
	}
	Status SetTrimming(StringTrimming trimming)
	{
		return updateStatus(DllExports::GdipSetStringFormatTrimming(
				nativeStringFormat, trimming));
	}

private:
	StringFormat(GpStringFormat *stringFormat, Status status):
		nativeStringFormat(stringFormat), lastStatus(status) {}
	StringFormat(const StringFormat&);
	StringFormat& operator=(const StringFormat&);

	Status updateStatus(Status newStatus) const
	{
		if (newStatus != Ok) lastStatus = newStatus;
		return newStatus;
	}

	GpStringFormat *nativeStringFormat;
	mutable Status lastStatus;
};


// FIXME: do StringFormat::GenericDefault() et al. need to be thread safe?
// FIXME: maybe put this in gdiplus.c?

extern "C" void *_GdipStringFormatCachedGenericDefault;
extern "C" void *_GdipStringFormatCachedGenericTypographic;

__inline__ const StringFormat* StringFormat::GenericDefault()
{
	if (!_GdipStringFormatCachedGenericDefault) {
		GpStringFormat *nativeStringFormat = 0;
		Status status = DllExports::GdipStringFormatGetGenericDefault(
				&nativeStringFormat);
		if (status == Ok && nativeStringFormat) {
			_GdipStringFormatCachedGenericDefault = (void*)
				new StringFormat(nativeStringFormat, Ok);
		}
	}
	return (StringFormat*) _GdipStringFormatCachedGenericDefault;
}

__inline__ const StringFormat* StringFormat::GenericTypographic()
{
	if (!_GdipStringFormatCachedGenericTypographic) {
		GpStringFormat *nativeStringFormat = 0;
		Status status = DllExports::GdipStringFormatGetGenericTypographic(
				&nativeStringFormat);
		if (status == Ok && nativeStringFormat) {
			_GdipStringFormatCachedGenericTypographic = (void*)
				new StringFormat(nativeStringFormat, Ok);
		}
	}
	return (StringFormat*) _GdipStringFormatCachedGenericTypographic;
}



#endif /* __GDIPLUS_STRINGFORMAT_H */
