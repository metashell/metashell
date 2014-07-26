/**
 * @file kbdmou.h
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
#ifndef __KBDMOU_H
#define __KBDMOU_H
#pragma GCC system_header
#include <_mingw.h>

/*
 * Structures and definitions for Keyboard/Mouse class and port drivers.
 */

#include <ddk/ntddkbd.h>
#include <ddk/ntddmou.h>

#define DD_KEYBOARD_PORT_DEVICE_NAME      "\\Device\\KeyboardPort"
#define DD_KEYBOARD_PORT_DEVICE_NAME_U    L"\\Device\\KeyboardPort"
#define DD_KEYBOARD_PORT_BASE_NAME_U      L"KeyboardPort"
#define DD_POINTER_PORT_DEVICE_NAME       "\\Device\\PointerPort"
#define DD_POINTER_PORT_DEVICE_NAME_U     L"\\Device\\PointerPort"
#define DD_POINTER_PORT_BASE_NAME_U       L"PointerPort"

#define DD_KEYBOARD_CLASS_BASE_NAME_U     L"KeyboardClass"
#define DD_POINTER_CLASS_BASE_NAME_U      L"PointerClass"

#define DD_KEYBOARD_RESOURCE_CLASS_NAME_U             L"Keyboard"
#define DD_POINTER_RESOURCE_CLASS_NAME_U              L"Pointer"
#define DD_KEYBOARD_MOUSE_COMBO_RESOURCE_CLASS_NAME_U L"Keyboard/Pointer"

#define POINTER_PORTS_MAXIMUM             8
#define KEYBOARD_PORTS_MAXIMUM            8

#define KBDMOU_COULD_NOT_SEND_COMMAND     0x0000
#define KBDMOU_COULD_NOT_SEND_PARAM       0x0001
#define KBDMOU_NO_RESPONSE                0x0002
#define KBDMOU_INCORRECT_RESPONSE         0x0004

#define I8042_ERROR_VALUE_BASE            1000
#define INPORT_ERROR_VALUE_BASE           2000
#define SERIAL_MOUSE_ERROR_VALUE_BASE     3000

#define IOCTL_INTERNAL_KEYBOARD_CONNECT \
  CTL_CODE(FILE_DEVICE_KEYBOARD, 0x0080, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_INTERNAL_KEYBOARD_DISCONNECT \
  CTL_CODE(FILE_DEVICE_KEYBOARD,0x0100, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_INTERNAL_KEYBOARD_ENABLE \
  CTL_CODE(FILE_DEVICE_KEYBOARD, 0x0200, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_INTERNAL_KEYBOARD_DISABLE \
  CTL_CODE(FILE_DEVICE_KEYBOARD, 0x0400, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_INTERNAL_MOUSE_CONNECT \
  CTL_CODE(FILE_DEVICE_MOUSE, 0x0080, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_INTERNAL_MOUSE_DISCONNECT \
  CTL_CODE(FILE_DEVICE_MOUSE, 0x0100, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_INTERNAL_MOUSE_ENABLE \
  CTL_CODE(FILE_DEVICE_MOUSE, 0x0200, METHOD_NEITHER, FILE_ANY_ACCESS)

#define IOCTL_INTERNAL_MOUSE_DISABLE \
  CTL_CODE(FILE_DEVICE_MOUSE, 0x0400, METHOD_NEITHER, FILE_ANY_ACCESS)

typedef struct _CONNECT_DATA {
	PDEVICE_OBJECT  ClassDeviceObject;
	PVOID  ClassService;
} CONNECT_DATA, *PCONNECT_DATA;

typedef VOID
(STDAPICALLTYPE *PSERVICE_CALLBACK_ROUTINE)(
  /*IN*/ PVOID NormalContext,
  /*IN*/ PVOID SystemArgument1,
  /*IN*/ PVOID SystemArgument2,
  /*IN OUT*/ PVOID SystemArgument3);

#endif /* __KBDMOU_H */
