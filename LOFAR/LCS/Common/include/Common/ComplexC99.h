//# ComplexC99.h: Wrapper for declaration of C99 complex functions
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
//# $Id: ComplexC99.h 7330 2005-12-27 10:02:33Z loose $

#ifndef LOFAR_COMMON_COMPLEXC99
#define LOFAR_COMMON_COMPLEXC99

// \file

// An include of complex.h includes backward/complex.h and
// not /usr/include/complex.h. This is very unfortunate.
// Hence we declare functions like sin ourselves in this file.

extern "C" {
  LOFAR_DCOMPLEX csin   (LOFAR_DCOMPLEX x);
  LOFAR_DCOMPLEX ccos   (LOFAR_DCOMPLEX x);
  LOFAR_DCOMPLEX ctan   (LOFAR_DCOMPLEX x);
  LOFAR_DCOMPLEX casin  (LOFAR_DCOMPLEX x);
  LOFAR_DCOMPLEX cacos  (LOFAR_DCOMPLEX x);
  LOFAR_DCOMPLEX catan  (LOFAR_DCOMPLEX x);
  LOFAR_DCOMPLEX csinh  (LOFAR_DCOMPLEX x);
  LOFAR_DCOMPLEX ccosh  (LOFAR_DCOMPLEX x);
  LOFAR_DCOMPLEX ctanh  (LOFAR_DCOMPLEX x);
  LOFAR_DCOMPLEX casinh (LOFAR_DCOMPLEX x);
  LOFAR_DCOMPLEX cacosh (LOFAR_DCOMPLEX x);
  LOFAR_DCOMPLEX catanh (LOFAR_DCOMPLEX x);
  LOFAR_DCOMPLEX csqrt  (LOFAR_DCOMPLEX x);
  LOFAR_DCOMPLEX cexp   (LOFAR_DCOMPLEX x);
  LOFAR_DCOMPLEX cpow   (LOFAR_DCOMPLEX x, LOFAR_DCOMPLEX exp);
//# PVSS puts std::clog in the global namespace.
//# Hence do not define clog in that case.
#ifndef HAVE_PVSS
  LOFAR_DCOMPLEX clog   (LOFAR_DCOMPLEX x);
#endif
  LOFAR_DCOMPLEX clog10 (LOFAR_DCOMPLEX x);
  double         cabs   (LOFAR_DCOMPLEX x);
  double         carg   (LOFAR_DCOMPLEX x);

  LOFAR_FCOMPLEX csinf   (LOFAR_FCOMPLEX x);
  LOFAR_FCOMPLEX ccosf   (LOFAR_FCOMPLEX x);
  LOFAR_FCOMPLEX ctanf   (LOFAR_FCOMPLEX x);
  LOFAR_FCOMPLEX casinf  (LOFAR_FCOMPLEX x);
  LOFAR_FCOMPLEX cacosf  (LOFAR_FCOMPLEX x);
  LOFAR_FCOMPLEX catanf  (LOFAR_FCOMPLEX x);
  LOFAR_FCOMPLEX csinhf  (LOFAR_FCOMPLEX x);
  LOFAR_FCOMPLEX ccoshf  (LOFAR_FCOMPLEX x);
  LOFAR_FCOMPLEX ctanhf  (LOFAR_FCOMPLEX x);
  LOFAR_FCOMPLEX casinhf (LOFAR_FCOMPLEX x);
  LOFAR_FCOMPLEX cacoshf (LOFAR_FCOMPLEX x);
  LOFAR_FCOMPLEX catanhf (LOFAR_FCOMPLEX x);
  LOFAR_FCOMPLEX csqrtf  (LOFAR_FCOMPLEX x);
  LOFAR_FCOMPLEX cexpf   (LOFAR_FCOMPLEX x);
  LOFAR_FCOMPLEX cpowf   (LOFAR_FCOMPLEX x, LOFAR_FCOMPLEX exp);
  LOFAR_FCOMPLEX clogf   (LOFAR_FCOMPLEX x);
  LOFAR_FCOMPLEX clog10f (LOFAR_FCOMPLEX x);
  float          cabsf   (LOFAR_FCOMPLEX x);
  float          cargf   (LOFAR_FCOMPLEX x);
}

#endif
