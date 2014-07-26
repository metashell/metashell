/**
 * @file gdiplusmetafile.h
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
#ifndef __GDIPLUS_METAFILE_H
#define __GDIPLUS_METAFILE_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * GDI+ Metafile class
 */

#ifndef __cplusplus
#error "A C++ compiler is required to include gdiplusmetafile.h."
#endif

class Metafile: public Image
{
public:
	static UINT EmfToWmfBits(HENHMETAFILE hEmf,
			UINT cbData16, LPBYTE pData16,
			INT iMapMode = MM_ANISOTROPIC,
			EmfToWmfBitsFlags eFlags = EmfToWmfBitsFlagsDefault)
	{
		return DllExports::GdipEmfToWmfBits(hEmf,
				cbData16, pData16, iMapMode, eFlags);
	}
	static Status GetMetafileHeader(const WCHAR *filename,
			MetafileHeader *header)
	{
		return DllExports::GdipGetMetafileHeaderFromFile(
				filename, header);
	}
	static Status GetMetafileHeader(IStream *stream, MetafileHeader *header)
	{
		return DllExports::GdipGetMetafileHeaderFromStream(
				stream, header);
	}
	////TODO: Metafile::GetMetafileHeader
	//static Status GetMetafileHeader(HMETAFILE hWmf,
	//		const WmfPlaceableFileHeader *wmfPlaceableFileHeader,
	//		MetafileHeader *header)
	//{
	//	// WTF: No flat API to do this.
	//	return NotImplemented;
	//}
	static Status GetMetafileHeader(HENHMETAFILE hEmf,
			MetafileHeader *header)
	{
		return DllExports::GdipGetMetafileHeaderFromEmf(hEmf, header);
	}

	Metafile(HMETAFILE hWmf,
			const WmfPlaceableFileHeader *wmfPlaceableFileHeader,
			BOOL deleteWmf = FALSE): Image(NULL, Ok)
	{
		GpMetafile *nativeMetafile = NULL;
		lastStatus = DllExports::GdipCreateMetafileFromWmf(
				hWmf, deleteWmf, wmfPlaceableFileHeader,
				&nativeMetafile);
		nativeImage = nativeMetafile;
	}
	Metafile(HENHMETAFILE hEmf, BOOL deleteEmf = FALSE): Image(NULL, Ok)
	{
		GpMetafile *nativeMetafile = NULL;
		lastStatus = DllExports::GdipCreateMetafileFromEmf(
				hEmf, deleteEmf, &nativeMetafile);
		nativeImage = nativeMetafile;
	}
	Metafile(const WCHAR *filename): Image(NULL, Ok)
	{
		GpMetafile *nativeMetafile = NULL;
		lastStatus = DllExports::GdipCreateMetafileFromFile(
				filename, &nativeMetafile);
		nativeImage = nativeMetafile;
	}
	Metafile(const WCHAR *filename,
			const WmfPlaceableFileHeader *wmfPlaceableFileHeader):
			Image(NULL, Ok)
	{
		GpMetafile *nativeMetafile = NULL;
		lastStatus = DllExports::GdipCreateMetafileFromWmfFile(
				filename, wmfPlaceableFileHeader,
				&nativeMetafile);
		nativeImage = nativeMetafile;
	}
	Metafile(IStream *stream): Image(NULL, Ok)
	{
		GpMetafile *nativeMetafile = NULL;
		lastStatus = DllExports::GdipCreateMetafileFromStream(
				stream, &nativeMetafile);
		nativeImage = nativeMetafile;
	}
	Metafile(HDC referenceHdc, EmfType type = EmfTypeEmfPlusDual,
			const WCHAR *description = NULL): Image(NULL, Ok)
	{
		GpMetafile *nativeMetafile = NULL;
		lastStatus = DllExports::GdipRecordMetafile(
				referenceHdc, type, NULL, MetafileFrameUnitGdi,
				description, &nativeMetafile);
		nativeImage = nativeMetafile;
	}
	Metafile(HDC referenceHdc, const RectF& frameRect,
			MetafileFrameUnit frameUnit = MetafileFrameUnitGdi,
			EmfType type = EmfTypeEmfPlusDual,
			const WCHAR *description = NULL): Image(NULL, Ok)
	{
		GpMetafile *nativeMetafile = NULL;
		lastStatus = DllExports::GdipRecordMetafile(
				referenceHdc, type, &frameRect, frameUnit,
				description, &nativeMetafile);
		nativeImage = nativeMetafile;
	}
	Metafile(HDC referenceHdc, const Rect& frameRect,
			MetafileFrameUnit frameUnit = MetafileFrameUnitGdi,
			EmfType type = EmfTypeEmfPlusDual,
			const WCHAR *description = NULL): Image(NULL, Ok)
	{
		GpMetafile *nativeMetafile = NULL;
		lastStatus = DllExports::GdipRecordMetafileI(
				referenceHdc, type, &frameRect, frameUnit,
				description, &nativeMetafile);
		nativeImage = nativeMetafile;
	}
	Metafile(const WCHAR *filename, HDC referenceHdc,
			EmfType type = EmfTypeEmfPlusDual,
			const WCHAR *description = NULL): Image(NULL, Ok)
	{
		GpMetafile *nativeMetafile = NULL;
		lastStatus = DllExports::GdipRecordMetafileFileName(
				filename, referenceHdc, type, NULL,
				MetafileFrameUnitGdi, description,
				&nativeMetafile);
		nativeImage = nativeMetafile;
	}
	Metafile(const WCHAR *filename, HDC referenceHdc,
			const RectF& frameRect,
			MetafileFrameUnit frameUnit = MetafileFrameUnitGdi,
			EmfType type = EmfTypeEmfPlusDual,
			const WCHAR *description = NULL): Image(NULL, Ok)
	{
		GpMetafile *nativeMetafile = NULL;
		lastStatus = DllExports::GdipRecordMetafileFileName(
				filename, referenceHdc, type, &frameRect,
				frameUnit, description, &nativeMetafile);
		nativeImage = nativeMetafile;
	}
	Metafile(const WCHAR *filename, HDC referenceHdc,
			const Rect& frameRect,
			MetafileFrameUnit frameUnit = MetafileFrameUnitGdi,
			EmfType type = EmfTypeEmfPlusDual,
			const WCHAR *description = NULL): Image(NULL, Ok)
	{
		GpMetafile *nativeMetafile = NULL;
		lastStatus = DllExports::GdipRecordMetafileFileNameI(
				filename, referenceHdc, type, &frameRect,
				frameUnit, description, &nativeMetafile);
		nativeImage = nativeMetafile;
	}
	Metafile(IStream *stream, HDC referenceHdc,
			EmfType type = EmfTypeEmfPlusDual,
			const WCHAR *description = NULL): Image(NULL, Ok)
	{
		GpMetafile *nativeMetafile = NULL;
		lastStatus = DllExports::GdipRecordMetafileStream(
				stream, referenceHdc, type, NULL,
				MetafileFrameUnitGdi, description,
				&nativeMetafile);
		nativeImage = nativeMetafile;
	}
	Metafile(IStream *stream, HDC referenceHdc, const RectF& frameRect,
			MetafileFrameUnit frameUnit = MetafileFrameUnitGdi,
			EmfType type = EmfTypeEmfPlusDual,
			const WCHAR *description = NULL): Image(NULL, Ok)
	{
		GpMetafile *nativeMetafile = NULL;
		lastStatus = DllExports::GdipRecordMetafileStream(
				stream, referenceHdc, type, &frameRect,
				frameUnit, description, &nativeMetafile);
		nativeImage = nativeMetafile;
	}
	Metafile(IStream *stream, HDC referenceHdc, const Rect& frameRect,
			MetafileFrameUnit frameUnit = MetafileFrameUnitGdi,
			EmfType type = EmfTypeEmfPlusDual,
			const WCHAR *description = NULL): Image(NULL, Ok)
	{
		GpMetafile *nativeMetafile = NULL;
		lastStatus = DllExports::GdipRecordMetafileStreamI(
				stream, referenceHdc, type, &frameRect,
				frameUnit, description, &nativeMetafile);
		nativeImage = nativeMetafile;
	}
	virtual ~Metafile()
	{
	}
	virtual Metafile* Clone() const
	{
		GpImage *cloneImage = NULL;
		Status status = updateStatus(DllExports::GdipCloneImage(
				nativeImage, &cloneImage));
		if (status == Ok) {
			Metafile *result = new Metafile(cloneImage, lastStatus);
			if (!result) {
				DllExports::GdipDisposeImage(cloneImage);
				lastStatus = OutOfMemory;
			}
			return result;
		} else {
			return NULL;
		}
	}

	////TODO: [GDI+ 1.1] Metafile::ConvertToEmfPlus
	//Status ConvertToEmfPlus(const Graphics *refGraphics,
	//		BOOL *conversionSuccess = NULL,
	//		EmfType emfType = EmfTypeEmfPlusOnly,
	//		const WCHAR *description = NULL)
	//{
	//	// FIXME: can't test GdipConvertToEmfPlus because it isn't exported in 1.0
	//	return updateStatus(DllExports::GdipConvertToEmfPlus(
	//			refGraphics ? refGraphics->nativeGraphics : NULL,
	//			(GpMetafile*) nativeImage,
	//			conversionSuccess, emfType, description, ???));
	//}
	////TODO: [GDI+ 1.1] Metafile::ConvertToEmfPlus
	//Status ConvertToEmfPlus(const Graphics *refGraphics,
	//		const WCHAR *filename,
	//		BOOL *conversionSuccess = NULL,
	//		EmfType emfType = EmfTypeEmfPlusOnly,
	//		const WCHAR *description = NULL)
	//{
	//	// FIXME: can't test GdipConvertToEmfPlusToFile because it isn't exported in 1.0
	//	return updateStatus(DllExports::GdipConvertToEmfPlusToFile(
	//			refGraphics ? refGraphics->nativeGraphics : NULL,
	//			(GpMetafile*) nativeImage, conversionSuccess,
	//			filename, emfType, description, ???));
	//}
	////TODO: [GDI+ 1.1] Metafile::ConvertToEmfPlus
	//Status ConvertToEmfPlus(const Graphics *refGraphics,
	//		IStream *stream,
	//		BOOL *conversionSuccess = NULL,
	//		EmfType emfType = EmfTypeEmfPlusOnly,
	//		const WCHAR *description = NULL)
	//{
	//	// FIXME: can't test GdipConvertToEmfPlusToStream because it isn't exported in 1.0
	//	return updateStatus(DllExports::GdipConvertToEmfPlusToStream(
	//			refGraphics ? refGraphics->nativeGraphics : NULL,
	//			(GpMetafile*) nativeImage, conversionSuccess,
	//			stream, emfType, description, ???));
	//}
	UINT GetDownLevelRasterizationLimit() const
	{
		UINT result = 0;
		updateStatus(DllExports::GdipGetMetafileDownLevelRasterizationLimit(
				(GpMetafile*) nativeImage, &result));
		return result;
	}
	HENHMETAFILE GetHENHMETAFILE()
	{
		HENHMETAFILE result = NULL;
		updateStatus(DllExports::GdipGetHemfFromMetafile(
				(GpMetafile*) nativeImage, &result));
		return result;
	}
	Status GetMetafileHeader(MetafileHeader *header) const
	{
		return updateStatus(DllExports::GdipGetMetafileHeaderFromMetafile(
				(GpMetafile*) nativeImage, header));
	}
	Status PlayRecord(EmfPlusRecordType recordType, UINT flags,
			UINT dataSize, const BYTE *data) const
	{
		return updateStatus(DllExports::GdipPlayMetafileRecord(
				(GpMetafile*) nativeImage,
				recordType, flags, dataSize, data));
	}
	Status SetDownLevelRasterizationLimit(UINT limitDpi)
	{
		return updateStatus(DllExports::GdipSetMetafileDownLevelRasterizationLimit(
				(GpMetafile*) nativeImage, limitDpi));
	}

private:
	Metafile(GpImage *image, Status status): Image(image, status) {}
	Metafile(const Metafile&);
	Metafile& operator=(const Metafile&);
};

#endif /* __GDIPLUS_METAFILE_H */
