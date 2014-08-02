/**
 * @file gdiplusmatrix.h
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
#ifndef __GDIPLUS_MATRIX_H
#define __GDIPLUS_MATRIX_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * GDI+ Matrix class
 */

#ifndef __cplusplus
#error "A C++ compiler is required to include gdiplusmatrix.h."
#endif

#define GDIP_MATRIX_PI \
	3.1415926535897932384626433832795028841971693993751058209749445923078164

class Matrix: public GdiplusBase
{
	friend class Graphics;
	friend class GraphicsPath;
	friend class LinearGradientBrush;
	friend class PathGradientBrush;
	friend class Pen;
	friend class Region;
	friend class TextureBrush;

public:
	Matrix(): nativeMatrix(NULL), lastStatus(Ok)
	{
		lastStatus = DllExports::GdipCreateMatrix(&nativeMatrix);
	}
	Matrix(REAL m11, REAL m12, REAL m21, REAL m22, REAL dx, REAL dy):
			nativeMatrix(NULL), lastStatus(Ok)
	{
		lastStatus = DllExports::GdipCreateMatrix2(
				m11, m12, m21, m22, dx, dy,
				&nativeMatrix);
	}
	Matrix(const RectF& rect, const PointF *dstplg):
			nativeMatrix(NULL), lastStatus(Ok)
	{
		lastStatus = DllExports::GdipCreateMatrix3(
				&rect, dstplg, &nativeMatrix);
	}
	Matrix(const Rect& rect, const Point *dstplg):
			nativeMatrix(NULL), lastStatus(Ok)
	{
		lastStatus = DllExports::GdipCreateMatrix3I(
				&rect, dstplg, &nativeMatrix);
	}
	~Matrix()
	{
		DllExports::GdipDeleteMatrix(nativeMatrix);
	}
	Matrix* Clone() const
	{
		GpMatrix *cloneMatrix = NULL;
		Status status = updateStatus(DllExports::GdipCloneMatrix(
				nativeMatrix, &cloneMatrix));
		if (status == Ok) {
			Matrix *result = new Matrix(cloneMatrix, lastStatus);
			if (!result) {
				DllExports::GdipDeleteMatrix(cloneMatrix);
				lastStatus = OutOfMemory;
			}
			return result;
		} else {
			return NULL;
		}
	}

	BOOL Equals(const Matrix *matrix) const
	{
		BOOL result;
		updateStatus(DllExports::GdipIsMatrixEqual(
				nativeMatrix,
				matrix ? matrix->nativeMatrix : NULL, &result));
		return result;
	}
	Status GetElements(REAL *m) const
	{
		return updateStatus(DllExports::GdipGetMatrixElements(
				nativeMatrix, m));
	}
	Status GetLastStatus() const
	{
		Status result = lastStatus;
		lastStatus = Ok;
		return result;
	}
	Status Invert()
	{
		return updateStatus(DllExports::GdipInvertMatrix(nativeMatrix));
	}
	BOOL IsIdentity() const
	{
		BOOL result;
		updateStatus(DllExports::GdipIsMatrixIdentity(
				nativeMatrix, &result));
		return result;
	}
	BOOL IsInvertible() const
	{
		BOOL result;
		updateStatus(DllExports::GdipIsMatrixInvertible(
				nativeMatrix, &result));
		return result;
	}
	Status Multiply(const Matrix *matrix,
			MatrixOrder order = MatrixOrderPrepend)
	{
		return updateStatus(DllExports::GdipMultiplyMatrix(
				nativeMatrix,
				matrix ? matrix->nativeMatrix : NULL, order));
	}
	REAL OffsetX() const
	{
		REAL m[6];
		updateStatus(DllExports::GdipGetMatrixElements(nativeMatrix, m));
		return m[4];
	}
	REAL OffsetY() const
	{
		REAL m[6];
		updateStatus(DllExports::GdipGetMatrixElements(nativeMatrix, m));
		return m[5];
	}
	Status Reset()
	{
		return updateStatus(DllExports::GdipSetMatrixElements(
				nativeMatrix,
				1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f));
	}
	Status Rotate(REAL angle, MatrixOrder order = MatrixOrderPrepend)
	{
		return updateStatus(DllExports::GdipRotateMatrix(
				nativeMatrix, angle, order));
	}
	Status RotateAt(REAL angle, const PointF& center,
			MatrixOrder order = MatrixOrderPrepend)
	{
		REAL angleRadian = angle * GDIP_MATRIX_PI / 180.0f;
		REAL cosAngle = ::cos(angleRadian);
		REAL sinAngle = ::sin(angleRadian);
		REAL x = center.X;
		REAL y = center.Y;

		Matrix matrix2(cosAngle, sinAngle, -sinAngle, cosAngle,
				x * (1.0f-cosAngle) + y * sinAngle,
				-x * sinAngle + y * (1.0f-cosAngle));
		Status status = matrix2.GetLastStatus();
		if (status == Ok) {
			return Multiply(&matrix2, order);
		} else {
			return lastStatus = status;
		}
	}
	Status Scale(REAL scaleX, REAL scaleY,
			MatrixOrder order = MatrixOrderPrepend)
	{
		return updateStatus(DllExports::GdipScaleMatrix(
				nativeMatrix, scaleX, scaleY, order));
	}
	Status SetElements(REAL m11, REAL m12, REAL m21, REAL m22,
			REAL dx, REAL dy)
	{
		return updateStatus(DllExports::GdipSetMatrixElements(
				nativeMatrix, m11, m12, m21, m22, dx, dy));
	}
	Status Shear(REAL shearX, REAL shearY,
			MatrixOrder order = MatrixOrderPrepend)
	{
		return updateStatus(DllExports::GdipShearMatrix(
				nativeMatrix, shearX, shearY, order));
	}
	Status TransformPoints(PointF *pts, INT count = 1) const
	{
		return updateStatus(DllExports::GdipTransformMatrixPoints(
				nativeMatrix, pts, count));
	}
	Status TransformPoints(Point *pts, INT count = 1) const
	{
		return updateStatus(DllExports::GdipTransformMatrixPointsI(
				nativeMatrix, pts, count));
	}
	Status TransformVectors(PointF *pts, INT count = 1) const
	{
		return updateStatus(DllExports::GdipVectorTransformMatrixPoints(
				nativeMatrix, pts, count));
	}
	Status TransformVectors(Point *pts, INT count = 1) const
	{
		return updateStatus(DllExports::GdipVectorTransformMatrixPointsI(
				nativeMatrix, pts, count));
	}
	Status Translate(REAL offsetX, REAL offsetY,
			MatrixOrder order = MatrixOrderPrepend)
	{
		return updateStatus(DllExports::GdipTranslateMatrix(
				nativeMatrix, offsetX, offsetY, order));
	}

private:
	Matrix(GpMatrix *matrix, Status status):
		nativeMatrix(matrix), lastStatus(status) {}
	Matrix(const Matrix&);
	Matrix& operator=(const Matrix&);

	Status updateStatus(Status newStatus) const
	{
		if (newStatus != Ok) lastStatus = newStatus;
		return newStatus;
	}

	GpMatrix *nativeMatrix;
	mutable Status lastStatus;
};

#undef GDIP_MATRIX_PI

#endif /* __GDIPLUS_MATRIX_H */
