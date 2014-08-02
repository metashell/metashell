/**
 * @file ntddkbd.h
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
/* Created by Casper S. Hornstrup <chorns@users.sourceforge.net> */
#ifndef __NTDDKBD_H
#define __NTDDKBD_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Keyboard IOCTL interface
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ntddk.h"

#define DD_KEYBOARD_DEVICE_NAME           "\\Device\\KeyboardClass"
#define DD_KEYBOARD_DEVICE_NAME_U         L"\\Device\\KeyboardClass"

#define IOCTL_KEYBOARD_QUERY_ATTRIBUTES \
  CTL_CODE(FILE_DEVICE_KEYBOARD, 0x0000, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_KEYBOARD_QUERY_INDICATORS \
  CTL_CODE(FILE_DEVICE_KEYBOARD, 0x0010, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_KEYBOARD_QUERY_INDICATOR_TRANSLATION \
  CTL_CODE(FILE_DEVICE_KEYBOARD, 0x0020, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_KEYBOARD_QUERY_TYPEMATIC \
  CTL_CODE(FILE_DEVICE_KEYBOARD, 0x0008, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_KEYBOARD_SET_TYPEMATIC \
  CTL_CODE(FILE_DEVICE_KEYBOARD, 0x0001, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_KEYBOARD_SET_INDICATORS \
  CTL_CODE(FILE_DEVICE_KEYBOARD, 0x0002, METHOD_BUFFERED, FILE_ANY_ACCESS)


DEFINE_GUID(GUID_DEVINTERFACE_KEYBOARD, \
  0x884b96c3, 0x56ef, 0x11d1, 0xbc, 0x8c, 0x00, 0xa0, 0xc9, 0x14, 0x05, 0xdd);

#define KEYBOARD_ERROR_VALUE_BASE         10000

/* KEYBOARD_INPUT_DATA.MakeCode constants */
#define KEYBOARD_OVERRUN_MAKE_CODE        0xFF

/* KEYBOARD_INPUT_DATA.Flags constants */
#define KEY_MAKE                          0
#define KEY_BREAK                         1
#define KEY_E0                            2
#define KEY_E1                            4

typedef struct _KEYBOARD_INPUT_DATA {
  USHORT  UnitId;
  USHORT  MakeCode;
  USHORT  Flags;
  USHORT  Reserved;
  ULONG  ExtraInformation;
} KEYBOARD_INPUT_DATA, *PKEYBOARD_INPUT_DATA;


typedef struct _KEYBOARD_TYPEMATIC_PARAMETERS {
	USHORT  UnitId;
	USHORT  Rate;
	USHORT  Delay;
} KEYBOARD_TYPEMATIC_PARAMETERS, *PKEYBOARD_TYPEMATIC_PARAMETERS;

typedef struct _KEYBOARD_ID {
	UCHAR  Type;
	UCHAR  Subtype;
} KEYBOARD_ID, *PKEYBOARD_ID;

#define ENHANCED_KEYBOARD(Id) ((Id).Type == 2 || (Id).Type == 4 || FAREAST_KEYBOARD(Id))
#define FAREAST_KEYBOARD(Id)  ((Id).Type == 7 || (Id).Type == 8)

typedef struct _KEYBOARD_INDICATOR_PARAMETERS {
  USHORT  UnitId;
  USHORT  LedFlags;
} KEYBOARD_INDICATOR_PARAMETERS, *PKEYBOARD_INDICATOR_PARAMETERS;

typedef struct _INDICATOR_LIST {
  USHORT  MakeCode;
  USHORT  IndicatorFlags;
} INDICATOR_LIST, *PINDICATOR_LIST;

typedef struct _KEYBOARD_INDICATOR_TRANSLATION {
  USHORT  NumberOfIndicatorKeys;
  INDICATOR_LIST  IndicatorList[1];
} KEYBOARD_INDICATOR_TRANSLATION, *PKEYBOARD_INDICATOR_TRANSLATION;

typedef struct _KEYBOARD_ATTRIBUTES {
	KEYBOARD_ID  KeyboardIdentifier;
	USHORT  KeyboardMode;
	USHORT  NumberOfFunctionKeys;
	USHORT  NumberOfIndicators;
	USHORT  NumberOfKeysTotal;
	ULONG  InputDataQueueLength;
	KEYBOARD_TYPEMATIC_PARAMETERS  KeyRepeatMinimum;
	KEYBOARD_TYPEMATIC_PARAMETERS  KeyRepeatMaximum;
} KEYBOARD_ATTRIBUTES, *PKEYBOARD_ATTRIBUTES;

typedef struct _KEYBOARD_UNIT_ID_PARAMETER {
  USHORT  UnitId;
} KEYBOARD_UNIT_ID_PARAMETER, *PKEYBOARD_UNIT_ID_PARAMETER;

typedef struct _KEYBOARD_IME_STATUS {
	USHORT  UnitId;
	ULONG  ImeOpen;
	ULONG  ImeConvMode;
} KEYBOARD_IME_STATUS, *PKEYBOARD_IME_STATUS;

#ifdef __cplusplus
}
#endif

#endif /* __NTDDKBD_H */
