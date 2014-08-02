/**
 * @file excpt.h
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
#ifndef	_EXCPT_H_
#define	_EXCPT_H_
#pragma GCC system_header
#include <_mingw.h>

/*
 * Support for operating system level structured exception handling.
 *
 * NOTE: This is very preliminary stuff. I am also pretty sure it is
 *       completely Intel specific.
 */

#include <windef.h>

/*
 * NOTE: The constants structs and typedefs below should be defined in the
 *       Win32 API headers.
 */
#define	EH_NONCONTINUABLE	0x01
#define	EH_UNWINDING		0x02
#define	EH_EXIT_UNWIND		0x04
#define	EH_STACK_INVALID	0x08
#define	EH_NESTED_CALL		0x10

#ifndef	RC_INVOKED

typedef enum {
	ExceptionContinueExecution,
	ExceptionContinueSearch,
	ExceptionNestedException,
	ExceptionCollidedUnwind
} EXCEPTION_DISPOSITION;


/*
 * End of stuff that should be in the Win32 API files.
 */


#ifdef	__cplusplus
extern "C" {
#endif

/*
 * The type of function that is expected as an exception handler to be
 * installed with __try1.
 */
typedef EXCEPTION_DISPOSITION (*PEXCEPTION_HANDLER)
		(struct _EXCEPTION_RECORD*, void*, struct _CONTEXT*, void*);

/*
 * This is not entirely necessary, but it is the structure installed by
 * the __try1 primitive below.
 */
typedef struct _EXCEPTION_REGISTRATION
{
	struct _EXCEPTION_REGISTRATION*	prev;
	PEXCEPTION_HANDLER		handler;
} EXCEPTION_REGISTRATION, *PEXCEPTION_REGISTRATION;

typedef EXCEPTION_REGISTRATION EXCEPTION_REGISTRATION_RECORD;
typedef PEXCEPTION_REGISTRATION PEXCEPTION_REGISTRATION_RECORD;

/*
 * A macro which installs the supplied exception handler.
 * Push the pointer to the new handler onto the stack,
 * then push the pointer to the old registration structure (at fs:0)
 * onto the stack, then put a pointer to the new registration
 * structure (i.e. the current stack pointer) at fs:0.
 */
#ifdef _WIN64
# define __try1(pHandler) \
	__asm__ __volatile__ ("pushq %0;pushq %%gs:0;movq %%rsp,%%gs:0;" : : \
	"g" (pHandler));
#else
# define __try1(pHandler) \
	__asm__ __volatile__ ("pushl %0;pushl %%fs:0;movl %%esp,%%fs:0;" : : \
	"g" (pHandler));
#endif

/*
 * A macro which (despite its name) *removes* an installed
 * exception handler. Should be used only in conjunction with the above
 * install routine __try1.
 * Move the pointer to the old reg. struct (at the current stack
 * position) to fs:0, replacing the pointer we installed above,
 * then add 8 to the stack pointer to get rid of the space we
 * used when we pushed on our new reg. struct above. Notice that
 * the stack must be in the exact state at this point that it was
 * after we did __try1 or this will smash things.
 */
#ifdef _WIN64
# define	__except1	\
	__asm__ __volatile__ ("movq (%%rsp),%%rax;movq %%rax,%%gs:0;addq \
	$16,%%rsp;" : : : "%rax");
#else
# define __except1	\
	__asm__ __volatile__ ("movl (%%esp),%%eax;movl %%eax,%%fs:0;addl \
	$8,%%esp;" : : : "%eax");
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* Not RC_INVOKED */

#endif	/* _EXCPT_H_ not defined */
