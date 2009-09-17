//# tTimer.cc: Test program for class NSTimer
//#
//# Copyright (C) 2004
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
//# $Id: tPrettyUnits.cc 11156 2008-04-16 10:18:30Z romein $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

#include <cmath>
#include <iostream>
#include <unistd.h>
#include <Common/PrettyUnits.h>

double a = 1;

using namespace LOFAR;

int main()
{
    std::cout << PrettyTime(1) << std::endl;
    std::cout << PrettyTime(1.2345) << std::endl;
    std::cout << PrettyTime(1.23456789, 9) << std::endl;
    std::cout << PrettyTime(1.2345e-6) << std::endl;
    std::cout << PrettyTime(0) << std::endl;
    std::cout << PrettyTime(-1.2345e-6) << std::endl;
    return 0;
}
