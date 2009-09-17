//#  Numeric.cc: 
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
//#  $Id: Numeric.cc 9487 2006-10-24 02:23:03Z diepen $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

//# Includes
#include <Common/Numeric.h>
#include <Common/LofarLogger.h>
#include <cstdlib>

namespace LOFAR
{

  bool Numeric::isFinite(float f)
  {
    floatMask_t exponent = *(floatMask_t*)&f & floatExponentMask;
    return exponent != floatExponentMask;
  }

  bool Numeric::isFinite(double d)
  {
    doubleMask_t exponent = *(doubleMask_t*)&d & doubleExponentMask;
    return exponent != doubleExponentMask;
  }

  bool Numeric::isNegative(float f)
  {
    floatMask_t sign = *(floatMask_t*)&f & floatNegativeMask;
    return sign == floatNegativeMask;
  }

  bool Numeric::isNegative(double d)
  {
    doubleMask_t sign = *(doubleMask_t*)&d & doubleNegativeMask;
    return sign == doubleNegativeMask;
  }

  bool Numeric::isInf(float f)
  {
    floatMask_t mantissa = *(floatMask_t*)&f & floatMantissaMask;
    return (!isFinite(f) && mantissa == 0L);
  }

  bool Numeric::isInf(double d)
  {
    doubleMask_t mantissa = *(doubleMask_t*)&d & doubleMantissaMask;
    return (!isFinite(d) && mantissa == 0LL);
  }

  bool Numeric::isNan(float f)
  {
    floatMask_t mantissa = *(floatMask_t*)&f & floatMantissaMask;
    return (!isFinite(f) && mantissa != 0L);
  }

  bool Numeric::isNan(double d)
  {
    doubleMask_t mantissa = *(doubleMask_t*)&d & doubleMantissaMask;
    return (!isFinite(d) && mantissa != 0LL);
  }


  bool Numeric::compare(float lhs, float rhs, floatMask_t maxUlps)
  {

#ifdef USE_INFINITYCHECK
    // If either \a lhs or \a rhs is infinite, then return true if they're
    // exactly equal, i.e. they have the same sign.
    if (isInf(lhs) || isInf(rhs)) return lhs == rhs;
#endif

#ifdef USE_NANCHECK
    // Consider \a lhs and \a rhs unequal when either is NaN. 
    if (isNan(lhs) || isNan(rhs)) return false;
#endif

#ifdef USE_SIGNCHECK
    // Consider \a lhs and \a rhs unequal when they have a different
    // sign, even if they both are very close to zero.
    // \note The check for equality is needed because zero and negative zero
    // have different signs but are equal to each other.
    if (isNegative(lhs) != isNegative(rhs)) return lhs == rhs;
#endif

    floatMask_t ilhs = *(floatMask_t*)&lhs;
    floatMask_t irhs = *(floatMask_t*)&rhs;

    // Make \a ilhs and \a irhs lexicographically ordered as twos-complement
    // long.
    if (ilhs < 0) ilhs = floatNegativeMask - ilhs;
    if (irhs < 0) irhs = floatNegativeMask - irhs;

    // If \a ilhs and \a irhs are less than \a maxUlps apart, then \a lhs and
    // \a rhs are considered equal.
    return std::abs(ilhs - irhs) <= maxUlps;

  }


  bool Numeric::compare(double lhs, double rhs, doubleMask_t maxUlps)
  {

#ifdef USE_INFINITYCHECK
    // If either \a lhs or \a rhs is infinite, then return true if they're
    // exactly equal, i.e. they have the same sign.
    if (isInf(lhs) || isInf(rhs)) return lhs == rhs;
#endif

#ifdef USE_NANCHECK
    // Consider \a lhs and \a rhs unequal when either is NaN. 
    if (isNan(lhs) || isNan(rhs)) return false;
#endif

#ifdef USE_SIGNCHECK
    // Consider \a lhs and \a rhs unequal when they have a different
    // sign, even if they both are very close to zero.
    // \note The check for equality is needed because zero and negative zero
    // have different signs but are equal to each other.
    if (isNegative(lhs) != isNegative(rhs)) return lhs == rhs;
#endif

    doubleMask_t ilhs = *(doubleMask_t*)&lhs;
    doubleMask_t irhs = *(doubleMask_t*)&rhs;

    // Make \a ilhs and \a irhs lexicographically ordered as twos-complement
    // long.
    if (ilhs < 0) ilhs = doubleNegativeMask - ilhs;
    if (irhs < 0) irhs = doubleNegativeMask - irhs;

    // If \a ilhs and \a irhs are less than \a maxUlps apart, then \a lhs and
    // \a rhs are considered equal.
    ilhs -= irhs;
    if (ilhs < 0) ilhs = -ilhs;
    return ilhs <= maxUlps;

  }
  

} // namespace LOFAR
