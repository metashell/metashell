/**
 * @file cderr.h
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
#ifndef _CDERR_H
#define _CDERR_H
#pragma GCC system_header
#include <_mingw.h>

#define CDERR_DIALOGFAILURE	0xFFFF
#define CDERR_GENERALCODES	0x0000
#define CDERR_STRUCTSIZE	0x0001
#define CDERR_INITIALIZATION	0x0002
#define CDERR_NOTEMPLATE	0x0003
#define CDERR_NOHINSTANCE	0x0004
#define CDERR_LOADSTRFAILURE	0x0005
#define CDERR_FINDRESFAILURE	0x0006
#define CDERR_LOADRESFAILURE	0x0007
#define CDERR_LOCKRESFAILURE	0x0008
#define CDERR_MEMALLOCFAILURE	0x0009
#define CDERR_MEMLOCKFAILURE	0x000A
#define CDERR_NOHOOK	0x000B
#define CDERR_REGISTERMSGFAIL	0x000C
#define PDERR_PRINTERCODES	0x1000
#define PDERR_SETUPFAILURE	0x1001
#define PDERR_PARSEFAILURE	0x1002
#define PDERR_RETDEFFAILURE	0x1003
#define PDERR_LOADDRVFAILURE	0x1004
#define PDERR_GETDEVMODEFAIL	0x1005
#define PDERR_INITFAILURE	0x1006
#define PDERR_NODEVICES	0x1007
#define PDERR_NODEFAULTPRN	0x1008
#define PDERR_DNDMMISMATCH	0x1009
#define PDERR_CREATEICFAILURE	0x100A
#define PDERR_PRINTERNOTFOUND	0x100B
#define PDERR_DEFAULTDIFFERENT	0x100C
#define CFERR_CHOOSEFONTCODES	0x2000
#define CFERR_NOFONTS	0x2001
#define CFERR_MAXLESSTHANMIN	0x2002
#define FNERR_FILENAMECODES	0x3000
#define FNERR_SUBCLASSFAILURE	0x3001
#define FNERR_INVALIDFILENAME	0x3002
#define FNERR_BUFFERTOOSMALL	0x3003
#define FRERR_FINDREPLACECODES	0x4000
#define FRERR_BUFFERLENGTHZERO	0x4001
#define CCERR_CHOOSECOLORCODES	0x5000
#endif
