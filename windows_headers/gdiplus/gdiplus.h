/**
 * @file gdiplus.h
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
#ifndef __GDIPLUS_H
#define __GDIPLUS_H
#pragma GCC system_header
#include <_mingw.h>

#ifndef RC_INVOKED

#include <stddef.h>
#include <math.h>
#include <windef.h>
#include <wingdi.h>

#include <basetyps.h>
typedef _COM_interface IStream IStream;
typedef _COM_interface IDirectDrawSurface7 IDirectDrawSurface7;

#ifdef __cplusplus
namespace Gdiplus {
#endif

typedef float REAL;
typedef SHORT INT16;
typedef WORD UINT16;

#include "gdiplusenums.h"
#include "gdiplustypes.h"
#include "gdiplusgpstubs.h"
#include "gdiplusimaging.h"
#include "gdiplusinit.h"
#include "gdiplusmem.h"
#include "gdiplusmetaheader.h"
#include "gdipluspixelformats.h"
#include "gdipluscolor.h"
#include "gdipluscolormatrix.h"
#include "gdiplusflat.h"
#include "gdipluseffects.h"
#include "gdiplusimagecodec.h"

#ifdef __cplusplus
#include "gdiplusbase.h"
#include "gdiplusheaders.h"
#include "gdiplusimageattributes.h"
#include "gdiplusmatrix.h"
#include "gdiplusbrush.h"
#include "gdiplusmetafile.h"
#include "gdipluspen.h"
#include "gdiplusstringformat.h"
#include "gdipluspath.h"
#include "gdiplusgraphics.h"
#include "gdipluslinecaps.h"
#include "gdiplusimpl.h"

}  /* namespace Gdiplus */
#endif /* __cplusplus */

#endif /* !RC_INVOKED */

#endif /* __GDIPLUS_H */
