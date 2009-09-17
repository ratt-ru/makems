//# ComplexBuiltinFP.h: Wrapper for efficient C99 floating point complex types
//#
//# Copyright (C) 2005
//# ASTRON (Netherlands Foundation for Research in Astronomy)
//# P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
//#
//# This program is free software; you can redistribute it and/or modify
//# it under the terms of the GNU General Public License as published by
//# the Free Software Foundation; either version 2 of the License, or
//# (at your option) any later version.
//#
//# This program is distributed in the hope that it will be useful,
//# but WITHOUT ANY WARRANTY; without even the implied warranty of
//# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//# GNU General Public License for more details.
//#
//# You should have received a copy of the GNU General Public License
//# along with this program; if not, write to the Free Software
//# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//#
//# $Id: ComplexBuiltinFP.h 7330 2005-12-27 10:02:33Z loose $

#ifndef LOFAR_COMMON_COMPLEXBUILTINFP_H
#define LOFAR_COMMON_COMPLEXBUILTINFP_H

// \file


#define LOFAR_FCOMPLEX LOFAR_BUILTIN_COMPLEXFP float
#define LOFAR_DCOMPLEX LOFAR_BUILTIN_COMPLEXFP double

//# For C++ an include of complex.h includes backward/complex.h and
//# not /usr/include/complex.h. This is very unfortunate.
//# Hence we declare functions like sin ourselves in ComplexC99.h.
#include <Common/ComplexC99.h>
#include <Common/lofar_iostream.h>


namespace LOFAR {
  namespace TYPES {
    // Define the complex types.
    typedef LOFAR_FCOMPLEX fcomplex;
    typedef LOFAR_DCOMPLEX dcomplex;
  }

  inline TYPES::fcomplex makefcomplex (float re, float im)
    { return (re + im*1.i); }
  inline TYPES::dcomplex makedcomplex (double re, double im)
    { return (re + im*1.i); }

  // Functions operating on single precision complex numbers.
  // <group>
  inline float real (TYPES::fcomplex x) { return __real__(x); }
  inline float imag (TYPES::fcomplex x) { return __imag__(x); }
  inline TYPES::fcomplex conj  (TYPES::fcomplex x) { return ~(x); }
  inline TYPES::fcomplex sin   (TYPES::fcomplex x) { return ::csinf(x); }
  inline TYPES::fcomplex cos   (TYPES::fcomplex x) { return ::ccosf(x); }
  inline TYPES::fcomplex tan   (TYPES::fcomplex x) { return ::ctanf(x); }
  inline TYPES::fcomplex asin  (TYPES::fcomplex x) { return ::casinf(x); }
  inline TYPES::fcomplex acos  (TYPES::fcomplex x) { return ::cacosf(x); }
  inline TYPES::fcomplex atan  (TYPES::fcomplex x) { return ::catanf(x); }
  inline TYPES::fcomplex sinh  (TYPES::fcomplex x) { return ::csinhf(x); }
  inline TYPES::fcomplex cosh  (TYPES::fcomplex x) { return ::ccoshf(x); }
  inline TYPES::fcomplex tanh  (TYPES::fcomplex x) { return ::ctanhf(x); }
  inline TYPES::fcomplex asinh (TYPES::fcomplex x) { return ::casinhf(x); }
  inline TYPES::fcomplex acosh (TYPES::fcomplex x) { return ::cacoshf(x); }
  inline TYPES::fcomplex atanh (TYPES::fcomplex x) { return ::catanhf(x); }
  inline TYPES::fcomplex sqrt  (TYPES::fcomplex x) { return ::csqrtf(x); }
  inline TYPES::fcomplex exp   (TYPES::fcomplex x) { return ::cexpf(x); }
  inline TYPES::fcomplex log   (TYPES::fcomplex x) { return ::clogf(x); }
  inline TYPES::fcomplex log10 (TYPES::fcomplex x) { return ::clog10f(x); }
  inline float           abs   (TYPES::fcomplex x) { return ::cabsf(x); }
  inline float           arg   (TYPES::fcomplex x) { return ::cargf(x); }
  inline TYPES::fcomplex pow   (TYPES::fcomplex x, TYPES::fcomplex exp)
    { return ::cpowf(x,exp); }
  // </group>

  // Functions operating on double precision complex numbers.
  // <group>
  inline double real (TYPES::dcomplex x) { return __real__(x); }
  inline double imag (TYPES::dcomplex x) { return __imag__(x); }
  inline TYPES::dcomplex conj  (TYPES::dcomplex x) { return ~(x); }
  inline TYPES::dcomplex sin   (TYPES::dcomplex x) { return ::csin(x); }
  inline TYPES::dcomplex cos   (TYPES::dcomplex x) { return ::ccos(x); }
  inline TYPES::dcomplex tan   (TYPES::dcomplex x) { return ::ctan(x); }
  inline TYPES::dcomplex asin  (TYPES::dcomplex x) { return ::casin(x); }
  inline TYPES::dcomplex acos  (TYPES::dcomplex x) { return ::cacos(x); }
  inline TYPES::dcomplex atan  (TYPES::dcomplex x) { return ::catan(x); }
  inline TYPES::dcomplex sinh  (TYPES::dcomplex x) { return ::csinh(x); }
  inline TYPES::dcomplex cosh  (TYPES::dcomplex x) { return ::ccosh(x); }
  inline TYPES::dcomplex tanh  (TYPES::dcomplex x) { return ::ctanh(x); }
  inline TYPES::dcomplex asinh (TYPES::dcomplex x) { return ::casinh(x); }
  inline TYPES::dcomplex acosh (TYPES::dcomplex x) { return ::cacosh(x); }
  inline TYPES::dcomplex atanh (TYPES::dcomplex x) { return ::catanh(x); }
  inline TYPES::dcomplex sqrt  (TYPES::dcomplex x) { return ::csqrt(x); }
  inline TYPES::dcomplex exp   (TYPES::dcomplex x) { return ::cexp(x); }
//# PVSS puts std::clog in the global namespace.
//# Hence in that case clog is not defined and cannot be used.
#ifndef HAVE_PVSS
  inline TYPES::dcomplex log   (TYPES::dcomplex x) { return ::clog(x); }
#endif
  inline TYPES::dcomplex log10 (TYPES::dcomplex x) { return ::clog10(x); }
  inline double          abs   (TYPES::dcomplex x) { return ::cabs(x); }
  inline double          arg   (TYPES::dcomplex x) { return ::carg(x); }
  inline TYPES::dcomplex pow   (TYPES::dcomplex x, TYPES::dcomplex exp)
    { return ::cpow(x,exp); }
  // </group>

  // Show the complex numbers.
  // <group>
  inline ostream& operator<< (ostream& os, TYPES::fcomplex x)
    { os << '(' << real(x) << ',' << imag(x) << ')'; return os; }
  inline ostream& operator<< (ostream& os, TYPES::dcomplex x)
    { os << '(' << real(x) << ',' << imag(x) << ')'; return os; }
  // </group>
}

#undef LOFAR_FCOMPLEX
#undef LOFAR_DCOMPLEX

#endif
