//# i4complex.h: complex 2x4-bit integer type
//#
//# Copyright (C) 2006
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
//# $Id: i4complex.h 11436 2008-07-15 12:16:29Z romein $

#ifndef LOFAR_COMMON_I4COMPLEX_H
#define LOFAR_COMMON_I4COMPLEX_H

#include <Common/lofar_iostream.h>


namespace LOFAR {
  namespace TYPES {
    class i4complex {
      public:
        i4complex() {}

	i4complex(double real, double imag) {
	  value = ((int) rint(real - .5) & 0xF) | (((int) rint(imag - .5) & 0xF) << 4);
	}

	double real() const {
	  return ((signed char) (value << 4) >> 4) + .5; // extend sign
	}

	double imag() const {
	  return (value >> 4) + .5;
	}

	i4complex conj() const {
	  return i4complex(value ^ 0xF0);
	}

      private:
	i4complex(unsigned char value)
	: value(value) {}
	    
	// do not use bitfields for the real and imaginary parts, since the
	// allocation order is not portable between different compilers
	signed char value;
    };
  }

  inline TYPES::i4complex makei4complex(double real, double imag)
    { return TYPES::i4complex(real, imag); }

  inline double real(TYPES::i4complex v)
    { return v.real(); }

  inline double imag(TYPES::i4complex v)
    { return v.imag(); }

  inline TYPES::i4complex conj(TYPES::i4complex x)
    { return x.conj(); }

  inline ostream& operator<< (ostream& os, TYPES::i4complex x)
    { os << '(' << real(x) << ',' << imag(x) << ')'; return os; }
}

#endif
