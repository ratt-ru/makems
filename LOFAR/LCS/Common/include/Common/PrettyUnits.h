//# PrettyUnits.h - Print units in a human-readable way
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
//# $Id: PrettyUnits.h 10816 2008-01-14 08:55:17Z romein $

#ifndef LOFAR_COMMON_PRETTY_UNITS_H
#define LOFAR_COMMON_PRETTY_UNITS_H

#include <string>


namespace LOFAR {

  class PrettyUnits : public std::string
  {
    protected:
      PrettyUnits(double value, const char *unit, unsigned precision);
  };

  class PrettyTime : public PrettyUnits
  {
    public:
      PrettyTime(double seconds = 0, unsigned precision = 3)
      :
	PrettyUnits(seconds, "s", precision)
      {
      }
  };

  class PrettyFrequency : public PrettyUnits
  {
    public:
      PrettyFrequency(double frequency = 0, unsigned precision = 3)
      :
	PrettyUnits(frequency, "Hz", precision)
      {
      }
  };

}  // end namespace LOFAR


#endif
