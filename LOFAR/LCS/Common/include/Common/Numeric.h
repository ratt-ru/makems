//#  Numeric.h: compare floating point numbers.
//#
//#  Copyright (C) 2002-2004
//#  ASTRON (Netherlands Foundation for Research in Astronomy)
//#  P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
//#
//#  This program is free software; you can redistribute it and/or modify
//#  it under the terms of the GNU General Public License as published by
//#  the Free Software Foundation; either version 2 of the License, or
//#  (at your option) any later version.
//#
//#  This program is distributed in the hope that it will be useful,
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//#  GNU General Public License for more details.
//#
//#  You should have received a copy of the GNU General Public License
//#  along with this program; if not, write to the Free Software
//#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//#
//#  $Id: Numeric.h 8240 2006-04-28 12:04:29Z loose $

#ifndef LOFAR_COMMON_NUMERIC_H
#define LOFAR_COMMON_NUMERIC_H

//# Never #include <config.h> or #include <lofar_config.h> in a header file!

// \file
// Compare floating point numbers.

// \def USE_INFINITYCHECK
// Add check for infinity. You will need this check if you don't want infinity
// to be treated as "close to" \c FLT_MAX.
#define USE_INFINITYCHECK

// \def USE_NANCHECK
// Add check for Not-a-Number. You will need this check if you want to make
// sure that two NANs are not treated as equal or "close to" one another; or
// if you want to use very large values for maxUlps.
#define USE_NANCHECK

//# \def USE_SIGNCHECK
//# Add check for sign. You will need this check if you don't want a very small
//# positive number to compare as "close to" a very small negative number.
/* #define USE_SIGNCHECK */
#undef USE_SIGNCHECK

namespace LOFAR
{
  // \addtogroup Common
  // @{

  // Numeric operations.
  // \attention These operations are only valid for machines that support the
  // IEEE 754 floating-point standard. Unfortunately, there is no easy way to
  // check this at compile-time.
  class Numeric
  {
  public:
    // Mask type used for floats is a (32-bit) int.
    typedef int floatMask_t;
    // Mask type used for doubles is a (64-bit) long long.
    typedef long long doubleMask_t;

    // \return \c true if f < 0; otherwise \c false.
    static bool isNegative(float f);
    // \return \c true if d < 0; otherwise \c false.
    static bool isNegative(double d);

    // \return \c true if \a f is neither inifinite nor "not-a-number" (NaN);
    // otherwise \c false.
    static bool isFinite(float f);
    // \return \c true if \a d is neither inifinite nor "not-a-number" (NaN);
    // otherwise \c false.
    static bool isFinite(double d);

    // \return \c true if \a f is infinite; otherwise \c false.
    static bool isInf(float f);
    // \return \c true if \a d is infinite; otherwise \c false.
    static bool isInf(double d);

    // \return \c true if \a f is "not-a-number" (NaN); otherwise \c false.
    static bool isNan(float f);
    // \return \c true if \a f is "not-a-number" (NaN); otherwise \c false.
    static bool isNan(double d);

    // \name Compare 
    // Compare two numeric values. \a lhs and \a rhs are considered
    // equal when the distance, measured in ULPs (Units in Last Place), between
    // them is less than \a maxUlps.
    //@{
    static bool compare(float lhs, float rhs, floatMask_t maxUlps = 8);
    static bool compare(double lhs, double rhs, doubleMask_t maxUlps = 32);
    // @}

  private:

    // @{
    // These constants are for IEEE754 floating point numbers.
    static const floatMask_t floatNegativeMask = 0x80000000L;
    static const floatMask_t floatExponentMask = 0x7F800000L;
    static const floatMask_t floatMantissaMask = 0x007FFFFFL;

    static const doubleMask_t doubleNegativeMask = 0x8000000000000000LL;
    static const doubleMask_t doubleExponentMask = 0x7FF0000000000000LL;
    static const doubleMask_t doubleMantissaMask = 0x000FFFFFFFFFFFFFLL;
    // @}

  };

} // namespace LOFAR

#endif
