//# PrettyUnits.cc - Print units in a human-readable way
//#
//# Copyright (C) 2008
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
//# $Id: PrettyUnits.cc 11104 2008-03-31 11:11:42Z romein $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <Common/PrettyUnits.h>

#include <sstream>
#include <iomanip>
#include <cmath>


namespace LOFAR {

PrettyUnits::PrettyUnits(double value, const char *unit, unsigned precision)
{
  static const char *prefixes = "yzafpnum kMGTPEZY";
  const char	    *prefix;

  if (value == 0.0)
    prefix = " ";
  else
    for (value *= 1e24, prefix = prefixes; fabs(value) >= 999.5 && prefix[1] != '\0'; prefix ++)
      value /= 1000.0;

  std::stringstream stream;
  stream << std::setprecision(precision) << std::setw(precision + 1) << value;
  *static_cast<std::string *>(this) = stream.str() + ' ' + *prefix + unit;
}

}  // end namespace LOFAR
