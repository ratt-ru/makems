//# lofar_complex.h:
//#
//# Copyright (C) 2002
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
//# $Id: lofar_complex.h 11436 2008-07-15 12:16:29Z romein $

#ifndef LOFAR_COMMON_COMPLEX_H
#define LOFAR_COMMON_COMPLEX_H

// \file

//# Put sin, etc. in LOFAR namespace.
#include <Common/lofar_math.h>

//# ICC supports builtin complex for floating point types only.
//# GCC supports it for both floating point and integer types.
//# Both use __complex__ as the complex type specifier.
//# Other compilers might use _Complex, so define LOFAR_BUILTIN_COMPLEX
//# as such to use it.
//# Note that ComplexBuiltin.h uses __real__ and __imag__. Maybe they
//# also need different names for _Complex.
//# Note: include StdInt and BuiltinFP in this order, otherwise the gcc
//# compiler complains that conj already exists in LOFAR namespace.
#if defined __INTEL_COMPILER || defined HAVE_BGL
# define LOFAR_BUILTIN_COMPLEXFP  __complex__
# include <Common/ComplexStdInt.h>
# include <Common/ComplexBuiltinFP.h>

#elif defined __GNUC__ && __GNUC__ < 4 && !defined __INSURE__
# define LOFAR_BUILTIN_COMPLEXFP  __complex__
# define LOFAR_BUILTIN_COMPLEXINT __complex__
# include <Common/ComplexBuiltinFP.h>
# include <Common/ComplexBuiltinInt.h>

#else
# include <Common/ComplexStdFP.h>
# include <Common/ComplexStdInt.h>

#endif

#include <Common/i4complex.h>

namespace LOFAR
{
  // Define complex types in LOFAR namespace.
  using TYPES::i4complex;
  using TYPES::i8complex;
  using TYPES::i16complex;
  using TYPES::u16complex;
  using TYPES::fcomplex;
  using TYPES::dcomplex;

  inline static i4complex makei4complex(i4complex &z) {
    return z;
  }

  inline static i4complex makei4complex(i8complex &z) {
    return makei4complex(real(z) - .5, imag(z) - .5);
  }

  inline static i4complex makei4complex(i16complex &z) {
    return makei4complex(real(z) - .5, imag(z) - .5);
  }

  inline static i4complex makei4complex(u16complex &z) {
    return makei4complex(real(z) - .5, imag(z) - .5);
  }

  inline static i4complex makei4complex(fcomplex &z) {
    return makei4complex(real(z), imag(z));
  }

  inline static i4complex makei4complex(dcomplex &z) {
    return makei4complex(real(z), imag(z));
  }

  inline static i8complex makei8complex(i4complex &z) {
    return makei8complex((int) real(z), (int) imag(z));
  }

  inline static i8complex makei8complex(i8complex &z) {
    return z;
  }

  inline static i8complex makei8complex(i16complex &z) {
    return makei8complex(real(z), imag(z));
  }

  inline static i8complex makei8complex(u16complex &z) {
    return makei8complex(real(z), imag(z));
  }

  inline static i8complex makei8complex(fcomplex &z) {
    return makei8complex((int) real(z), (int) imag(z));
  }

  inline static i8complex makei8complex(dcomplex &z) {
    return makei8complex((int) real(z), (int) imag(z));
  }

  inline static i16complex makei16complex(i4complex &z) {
    return makei16complex((int) real(z), (int) imag(z));
  }

  inline static i16complex makei16complex(i8complex &z) {
    return makei16complex(real(z), imag(z));
  }

  inline static i16complex makei16complex(i16complex &z) {
    return z;
  }

  inline static i16complex makei16complex(u16complex &z) {
    return makei16complex(real(z), imag(z));
  }

  inline static i16complex makei16complex(fcomplex &z) {
    return makei16complex((int) real(z), (int) imag(z));
  }

  inline static i16complex makei16complex(dcomplex &z) {
    return makei16complex((int) real(z), (int) imag(z));
  }

  inline static u16complex makeu16complex(i4complex &z) {
    return makeu16complex((int) real(z), (int) imag(z));
  }

  inline static u16complex makeu16complex(i8complex &z) {
    return makeu16complex(real(z), imag(z));
  }

  inline static u16complex makeu16complex(i16complex &z) {
    return makeu16complex(real(z), imag(z));
  }

  inline static u16complex makeu16complex(u16complex &z) {
    return z;
  }

  inline static u16complex makeu16complex(fcomplex &z) {
    return makeu16complex((unsigned) real(z), (unsigned) imag(z));
  }

  inline static u16complex makeu16complex(dcomplex &z) {
    return makeu16complex((unsigned) real(z), (unsigned) imag(z));
  }

  inline static fcomplex makefcomplex(i4complex &z) {
    return makefcomplex(real(z), imag(z));
  }

  inline static fcomplex makefcomplex(i8complex &z) {
    return makefcomplex((float) real(z), (float) imag(z));
  }

  inline static fcomplex makefcomplex(i16complex &z) {
    return makefcomplex((float) real(z), (float) imag(z));
  }

  inline static fcomplex makefcomplex(u16complex &z) {
    return makefcomplex((float) real(z), (float) imag(z));
  }

  inline static fcomplex makefcomplex(fcomplex &z) {
    return z;
  }

  inline static fcomplex makefcomplex(dcomplex &z) {
    return makefcomplex((float) real(z), (float) imag(z));
  }

  inline static dcomplex makedcomplex(i4complex &z) {
    return makedcomplex(real(z), imag(z));
  }

  inline static dcomplex makedcomplex(i8complex &z) {
    return makedcomplex((double) real(z), (double) imag(z));
  }

  inline static dcomplex makedcomplex(i16complex &z) {
    return makedcomplex((double) real(z), (double) imag(z));
  }

  inline static dcomplex makedcomplex(u16complex &z) {
    return makedcomplex((double) real(z), (double) imag(z));
  }

  inline static dcomplex makedcomplex(fcomplex &z) {
    return makedcomplex((double) real(z), (double) imag(z));
  }

  inline static dcomplex makedcomplex(dcomplex &z) {
    return z;
  }
}


#endif
