/**
 * @file gdiplusmetaheader.h
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
#ifndef __GDIPLUS_METAHEADER_H
#define __GDIPLUS_METAHEADER_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * GDI+ metafile header structure
 */

/*
 * FIXME: is 1 the correct value for GDIP_EMFPLUSFLAGS_DISPLAY? This number
 * has been determined by calling Metafile::GetMetafileHeader() on a EMF+
 * metafile which was recorded on a display device context (SampleMetafile.emf).
 */
#ifdef __cplusplus
const UINT GDIP_EMFPLUSFLAGS_DISPLAY = 1;
#else
#define GDIP_EMFPLUSFLAGS_DISPLAY ((UINT) 1)
#endif

typedef struct tagENHMETAHEADER3 {
	DWORD iType;
	DWORD nSize;
	RECTL rclBounds;
	RECTL rclFrame;
	DWORD dSignature;
	DWORD nVersion;
	DWORD nBytes;
	DWORD nRecords;
	WORD nHandles;
	WORD sReserved;
	DWORD nDescription;
	DWORD offDescription;
	DWORD nPalEntries;
	SIZEL szlDevice;
	SIZEL szlMillimeters;
} ENHMETAHEADER3,*LPENHMETAHEADER3;

typedef struct PWMFRect16 {
	INT16 Left;
	INT16 Top;
	INT16 Right;
	INT16 Bottom;
} PWMFRect16;

typedef struct WmfPlaceableFileHeader {
	UINT32 Key;
	INT16 Hmf;
	PWMFRect16 BoundingBox;
	INT16 Inch;
	UINT32 Reserved;
	INT16 Checksum;
} WmfPlaceableFileHeader;

typedef struct MetafileHeader {
	MetafileType Type;
	UINT Size;
	UINT Version;
	UINT EmfPlusFlags;
	REAL DpiX;
	REAL DpiY;
	INT X;
	INT Y;
	INT Width;
	INT Height;
	__extension__ union {
		METAHEADER WmfHeader;
		ENHMETAHEADER3 EmfHeader;
	};
	INT EmfPlusHeaderSize;
	INT LogicalDpiX;
	INT LogicalDpiY;

	#ifdef __cplusplus
	public:
	void GetBounds(Rect *rect) const
	{
		if (rect)
		{
			rect->X = X;
			rect->Y = Y;
			rect->Width = Width;
			rect->Height = Height;
		}
	}
	REAL GetDpiX() const
	{
		return DpiX;
	}
	REAL GetDpiY() const
	{
		return DpiY;
	}
	const ENHMETAHEADER3* GetEmfHeader() const
	{
		if (Type == MetafileTypeEmf
				|| Type == MetafileTypeEmfPlusOnly
				|| Type == MetafileTypeEmfPlusDual)
		{
			return &EmfHeader;
		}
		else
		{
			return NULL;
		}
	}
	UINT GetEmfPlusFlags() const
	{
		return EmfPlusFlags;
	}
	UINT GetMetafileSize() const
	{
		return Size;
	}
	MetafileType GetType() const
	{
		return Type;
	}
	UINT GetVersion() const
	{
		return Version;
	}
	const METAHEADER* GetWmfHeader() const
	{
		if (Type == MetafileTypeWmf || Type == MetafileTypeWmfPlaceable)
		{
			return &WmfHeader;
		}
		else
		{
			return NULL;
		}
	}
	BOOL IsDisplay() const
	{
		return EmfPlusFlags == GDIP_EMFPLUSFLAGS_DISPLAY;
	}
	BOOL IsEmf() const
	{
		return Type == MetafileTypeEmf;
	}
	BOOL IsEmfOrEmfPlus() const
	{
		return Type == MetafileTypeEmf
			|| Type == MetafileTypeEmfPlusOnly
			|| Type == MetafileTypeEmfPlusDual;
	}
	BOOL IsEmfPlus() const
	{
		return Type == MetafileTypeEmfPlusOnly
			|| Type == MetafileTypeEmfPlusDual;
	}
	BOOL IsEmfPlusDual() const
	{
		return Type == MetafileTypeEmfPlusDual;
	}
	BOOL IsEmfPlusOnly() const
	{
		return Type == MetafileTypeEmfPlusOnly;
	}
	BOOL IsWmf() const
	{
		return Type == MetafileTypeWmf
			|| Type == MetafileTypeWmfPlaceable;
	}
	BOOL IsWmfPlaceable() const
	{
		return Type == MetafileTypeWmfPlaceable;
	}
	#endif
} MetafileHeader;

#endif /* __GDIPLUS_METAHEADER_H */
