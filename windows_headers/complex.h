/**
 * @file complex.h
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
 /* Created by Danny Smith <dannysmith@users.sourceforge.net> */
#ifndef _COMPLEX_H_
#define _COMPLEX_H_
#pragma GCC system_header
#include <_mingw.h>

/* These macros are specified by C99 standard */

#ifndef __cplusplus
#define complex _Complex
#endif

#define _Complex_I  (0.0F +  1.0iF)

/* GCC doesn't support _Imaginary type yet, so we don't
   define _Imaginary_I */

#define I _Complex_I

#ifdef __cplusplus
extern "C" {
#endif 

#ifndef RC_INVOKED

double __MINGW_ATTRIB_CONST creal (double _Complex);
double __MINGW_ATTRIB_CONST cimag (double _Complex);
double __MINGW_ATTRIB_CONST carg (double _Complex);
double __MINGW_ATTRIB_CONST cabs (double _Complex);
double _Complex __MINGW_ATTRIB_CONST conj (double _Complex);
double _Complex  cacos (double _Complex);
double _Complex  casin (double _Complex);
double _Complex  catan (double _Complex);
double _Complex  ccos (double _Complex);
double _Complex  csin (double _Complex);
double _Complex  ctan (double _Complex);
double _Complex  cacosh (double _Complex);
double _Complex  casinh (double _Complex);
double _Complex  catanh (double _Complex);
double _Complex  ccosh (double _Complex);
double _Complex  csinh (double _Complex);
double _Complex  ctanh (double _Complex);
double _Complex  cexp (double _Complex);
double _Complex  clog (double _Complex);
double _Complex  cpow (double _Complex, double _Complex);
double _Complex  csqrt (double _Complex);
double _Complex __MINGW_ATTRIB_CONST cproj (double _Complex);

float __MINGW_ATTRIB_CONST crealf (float _Complex);
float __MINGW_ATTRIB_CONST cimagf (float _Complex);
float __MINGW_ATTRIB_CONST cargf (float _Complex);
float __MINGW_ATTRIB_CONST cabsf (float _Complex);
float _Complex __MINGW_ATTRIB_CONST conjf (float _Complex);
float _Complex  cacosf (float _Complex);
float _Complex  casinf (float _Complex);
float _Complex  catanf (float _Complex);
float _Complex  ccosf (float _Complex);
float _Complex  csinf (float _Complex);
float _Complex  ctanf (float _Complex);
float _Complex  cacoshf (float _Complex);
float _Complex  casinhf (float _Complex);
float _Complex  catanhf (float _Complex);
float _Complex  ccoshf (float _Complex);
float _Complex  csinhf (float _Complex);
float _Complex  ctanhf (float _Complex);
float _Complex  cexpf (float _Complex);
float _Complex  clogf (float _Complex);
float _Complex  cpowf (float _Complex, float _Complex);
float _Complex  csqrtf (float _Complex);
float _Complex __MINGW_ATTRIB_CONST cprojf (float _Complex);

long double __MINGW_ATTRIB_CONST creall (long double _Complex);
long double __MINGW_ATTRIB_CONST cimagl (long double _Complex);
long double __MINGW_ATTRIB_CONST cargl (long double _Complex);
long double __MINGW_ATTRIB_CONST cabsl (long double _Complex);
long double _Complex __MINGW_ATTRIB_CONST conjl (long double _Complex);
long double _Complex  cacosl (long double _Complex);
long double _Complex  casinl (long double _Complex);
long double _Complex  catanl (long double _Complex);
long double _Complex  ccosl (long double _Complex);
long double _Complex  csinl (long double _Complex);
long double _Complex  ctanl (long double _Complex);
long double _Complex  cacoshl (long double _Complex);
long double _Complex  casinhl (long double _Complex);
long double _Complex  catanhl (long double _Complex);
long double _Complex  ccoshl (long double _Complex);
long double _Complex  csinhl (long double _Complex);
long double _Complex  ctanhl (long double _Complex);
long double _Complex  cexpl (long double _Complex);
long double _Complex  clogl (long double _Complex);
long double _Complex  cpowl (long double _Complex, long double _Complex);
long double _Complex  csqrtl (long double _Complex);
long double _Complex __MINGW_ATTRIB_CONST cprojl (long double _Complex);

/* double */
__CRT_INLINE double __MINGW_ATTRIB_CONST creal (double _Complex _Z)
{
  return __real__ _Z;
}

__CRT_INLINE double __MINGW_ATTRIB_CONST cimag (double _Complex _Z)
{
  return __imag__ _Z;
}

__CRT_INLINE double _Complex __MINGW_ATTRIB_CONST conj (double _Complex _Z)
{
  return __extension__ ~_Z;
}

__CRT_INLINE  double __MINGW_ATTRIB_CONST carg (double _Complex _Z)
{
  double res;
  __asm__  ("fpatan;"
	   : "=t" (res) : "0" (__real__ _Z), "u" (__imag__ _Z) : "st(1)");
  return res;
}


/* float */
__CRT_INLINE float __MINGW_ATTRIB_CONST crealf (float _Complex _Z)
{
  return __real__ _Z;
}

__CRT_INLINE float __MINGW_ATTRIB_CONST cimagf (float _Complex _Z)
{
  return __imag__ _Z;
}

__CRT_INLINE float _Complex __MINGW_ATTRIB_CONST conjf (float _Complex _Z)
{
  return __extension__ ~_Z;
}

__CRT_INLINE  float __MINGW_ATTRIB_CONST cargf (float _Complex _Z)
{
  float res;
  __asm__  ("fpatan;"
	   : "=t" (res) : "0" (__real__ _Z), "u" (__imag__ _Z) : "st(1)");
  return res;
}

/* long double */
__CRT_INLINE long double __MINGW_ATTRIB_CONST creall (long double _Complex _Z)
{
  return __real__ _Z;
}

__CRT_INLINE long double __MINGW_ATTRIB_CONST cimagl (long double _Complex _Z)
{
  return __imag__ _Z;
}

__CRT_INLINE long double _Complex __MINGW_ATTRIB_CONST conjl (long double _Complex _Z)
{
  return __extension__ ~_Z;
}

__CRT_INLINE  long double __MINGW_ATTRIB_CONST cargl (long double _Complex _Z)
{
  long double res;
  __asm__  ("fpatan;"
	   : "=t" (res) : "0" (__real__ _Z), "u" (__imag__ _Z) : "st(1)");
  return res;
}

#endif /* RC_INVOKED */

#ifdef __cplusplus
}
#endif 


#endif /* _COMPLEX_H */
