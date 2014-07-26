/**
 * @file rpcnterr.h
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
#ifndef _RPCNTERR_H
#define _RPCNTERR_H
#pragma GCC system_header
#include <_mingw.h>

#define RPC_S_OK ERROR_SUCCESS
#define RPC_S_INVALID_ARG ERROR_INVALID_PARAMETER
#define RPC_S_OUT_OF_MEMORY ERROR_OUTOFMEMORY
#define RPC_S_OUT_OF_THREADS ERROR_MAX_THRDS_REACHED
#define RPC_S_INVALID_LEVEL ERROR_INVALID_PARAMETER
#define RPC_S_BUFFER_TOO_SMALL ERROR_INSUFFICIENT_BUFFER
#define RPC_S_INVALID_SECURITY_DESC ERROR_INVALID_SECURITY_DESCR
#define RPC_S_ACCESS_DENIED ERROR_ACCESS_DENIED
#define RPC_S_SERVER_OUT_OF_MEMORY ERROR_NOT_ENOUGH_SERVER_MEMORY
#define RPC_X_NO_MEMORY RPC_S_OUT_OF_MEMORY
#define RPC_X_INVALID_BOUND RPC_S_INVALID_BOUND
#define RPC_X_INVALID_TAG RPC_S_INVALID_TAG
#define RPC_X_ENUM_VALUE_TOO_LARGE RPC_X_ENUM_VALUE_OUT_OF_RANGE
#define RPC_X_SS_CONTEXT_MISMATCH ERROR_INVALID_HANDLE
#define RPC_X_INVALID_BUFFER ERROR_INVALID_USER_BUFFER
#define RPC_X_INVALID_PIPE_OPERATION RPC_X_WRONG_PIPE_ORDER

#endif
